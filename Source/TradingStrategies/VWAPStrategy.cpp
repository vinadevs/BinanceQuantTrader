/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include "VWAPStrategy.h"
#include "VWAPVolumeProfile.h"

#include "../RiskManagement/VWAPOrderExecutionRiskMetrics.h"
#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/StringUtils.h"
#include "../MarketData/RealTimeMarketData.h"
#include "../MarketData/SynchronousMarketData.h"
#include "../StaticData/StaticDataManager.h"
#include "../UserAccount/BinanceTrader.h"
#include "../ComplianceNRegulatory/BinanceTradingRules.h"
#include "../ComplianceNRegulatory/BinanceExchangeProfile.h"
#include "../QuantitativeModel/QuantOrderParammeter.h"
#include "../QuantitativeModel/MarketDataAnalyzer.h"
#include "../QuantitativeModel/QuantMarketDataAnalyzer.h"
#include "../LibraryUtils/PathUtils.h"
#include "../LibraryUtils/FileUtils.h"

using namespace TradingStrategies;
using namespace QuantitativeModel;
using namespace MarketData;
using namespace UserAccount;
using namespace ComplianceNRegulatory;
using namespace LibraryUtils;
using namespace tinyxml2;

#undef max

VWAPStrategy::VWAPStrategy(
	const std::string& strategyCfgPath,
	RealTimeMarketData* marketData,
	Trader* trader,
	BinanceTradingRules* tradingRules)
	: TradingStrategyBase("VWAPStrategy", "Create future smart orders...",
		strategyCfgPath, marketData, trader, tradingRules),
	  AlarmSystem(LibraryUtils::DefaultAlarmInterval, AlarmSystem::AlarmMode::REPEAT)
{
	SetStrategyType(StrategyType::FULL_AUTO);
	InitializeParameters(strategyCfgPath);
	m_logger->Info("Completed initialization for the strategy.");
}

VWAPStrategy::~VWAPStrategy()
{
	m_marketData->UnRegisterDataListener(this); // I earn enough money, leave the market now!
}

bool VWAPStrategy::OnTradeChange(MarketData::MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		const auto price = syncedData->GetSingleFeed(TradeID::PRICE)->GetDoubleData();
		const auto volume = syncedData->GetSingleFeed(TradeID::QUANTITY)->GetDoubleData();
		const auto time = syncedData->GetSingleFeed(TradeID::TRADE_TIME)->GetUnsignedIntData();
		m_vwapVolumeProfilier->AddNewBucketVolume(volume, time);
		m_cumPriceVolume += price * volume;
		m_totalMarketVolume += volume;
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

double VWAPStrategy::CalculateCurrentVWAP() const 
{
	return (m_totalMarketVolume > 0.0) ? (m_cumPriceVolume / m_totalMarketVolume) : 0.0;
}

double VWAPStrategy::GetOrderSizeForCurrentBucket(std::chrono::system_clock::time_point ts) 
{
	const auto bucketId = GetBucketVWAPId(ts);
	const auto profile = m_vwapVolumeProfilier->GetVolumeProfiles();

	double pct = 0.0;
	for (auto& p : profile)
	{
		if (p.first == bucketId)
		{
			pct = p.second;
			break;
		}
	}

	const double targetForBucket = m_targetVWAPAmount * pct;
	const double alreadyBought = m_executedVolume[bucketId];
	return std::max(0.0, targetForBucket - alreadyBought);
}

void VWAPStrategy::RecordTradeExecution(double volume, std::chrono::system_clock::time_point ts) 
{
	m_executedVolume[GetBucketVWAPId(ts)] += volume;
}

size_t VWAPStrategy::GetBucketVWAPId(std::chrono::system_clock::time_point ts) const 
{
	auto epochSec = std::chrono::duration_cast<std::chrono::seconds>(
		ts.time_since_epoch()).count();
	return epochSec / m_profileBucketSeconds;
}

void VWAPStrategy::ReportTradeResults(const std::string& symbol)
{
	m_spotTrader->ReportTradeResults(symbol);
}

void VWAPStrategy::InitializeParameters(const std::string& strategyCfgPath)
{
	m_strategyCfgXml = std::make_unique<XMLDocument>();
	const auto errLoadFileXml = m_strategyCfgXml->LoadFile(strategyCfgPath.c_str());
	if (errLoadFileXml != XML_SUCCESS)
	{
		throw std::runtime_error("VWAPStrategy: Load file Xml error="
			+ std::string(XMLDocument::ErrorIDToName(errLoadFileXml)) + ", error path:" + strategyCfgPath);
	}
	SetupStrategyLifeTime(m_strategyCfgXml.get());
	// when we use alarm system, we need to set up the order scheduler
	SetupOrderScheduler();
	SetupVWAPVolumeProfile();
}

void VWAPStrategy::InitializeMarketDataAnalyzer()
{
	m_marketDataAnalyzer = std::make_unique<QuantitativeModel::MarketDataAnalyzer>(m_targetFutureTradeSymbols, m_logger.get());
}

void VWAPStrategy::SetupOrderScheduler()
{
	m_logger->Info("Setting up alarm system for order sending interval.");
	const XMLElement* generalConfigXml = m_strategyCfgXml->FirstChildElement("OrderScheduler");
	assert(generalConfigXml);
	const XMLElement* orderSendingInvervalXml = generalConfigXml->FirstChildElement("OrderSendingInverval");
	assert(orderSendingInvervalXml);
	const int64_t alarmIntervalSecond = orderSendingInvervalXml->Int64Attribute("AlarmIntervalSecond");
	if (alarmIntervalSecond <= 0)
	{
		throw std::runtime_error("VWAPStrategy: Invalid alarm interval second="
			+ std::to_string(alarmIntervalSecond) + ", must be greater than 0.");
	}
	AlarmSystem::SetRepeatInterval(alarmIntervalSecond);
	m_strategyOrderScheduler = StrategyOrderScheduler::ALARM_BASED;
}

void VWAPStrategy::SetupVWAPVolumeProfile()
{
	m_logger->Info("Setting up VWAP volume profile.");
	const XMLElement* vwapVolumeProfileXml = m_strategyCfgXml->FirstChildElement("VWAPVolumeProfile");
	assert(vwapVolumeProfileXml);
	const int bucketSeconds = vwapVolumeProfileXml->IntAttribute("BucketSeconds");
	if (bucketSeconds <= 0)
	{
		throw std::runtime_error("VWAPStrategy: Invalid bucket seconds="
			+ std::to_string(bucketSeconds) + ", must be greater than 0.");
	}
	m_vwapVolumeProfilier = std::make_unique<VWAPVolumeProfile>(bucketSeconds);
}

void VWAPStrategy::StartLive()
{
	// Change Strategy state to live
	m_strategyRunStatus = StrategyRunStatus::LIVE;
	// Prepare target symbols list
	m_logger->Info("Prepare target symbols list.");
	PrepareTargetMonitorSymbols();
	// Create Market Data Analyzer
	m_logger->Info("Create market data analyzer.");
	InitializeMarketDataAnalyzer();
	// Create exchange filter profile
	m_logger->Info("Create binance exchange profile.");
	CreateBinanceExchangeProfile();
	// Create portfolio management
	m_logger->Info("Create portfolio management.");
	CreatePortfolioManagement();
	// Subscribe target symbols to receive real time market data
	m_logger->Info("Subscribe target symbols.");
	SubscribeTargetSymbols();
	// Start alarm system to send orders
	m_logger->Info("Starting live and trade.");
	AlarmSystem::Start();
}

void VWAPStrategy::StopLive()
{
	m_strategyRunStatus = StrategyRunStatus::STOP;
	// Unsubscribe target symbols to stop receiving real time market data
	m_logger->Info("Unsubscribe target symbols.");
	UnsubscribeTargetSymbols();
}

void VWAPStrategy::OnAlarmTriggered(const int passToDerived)
{
	for (const auto& symbol : m_targetFutureTradeSymbols)
	{
		// Calculate next VWAP volume size at current time bucket
		const auto now = std::chrono::system_clock::now();
		const double orderSize = GetOrderSizeForCurrentBucket(now);
		const double marketVWAP = CalculateCurrentVWAP();

		if (orderSize > 0.0)
		{
			SendOrderToExchange(orderSize, limitPrice);
			RecordTradeExecution(orderSize, ts);
			m_executedPrices.push_back(executedPrice);
			m_vwapPrices.push_back(marketVWAP);
			m_pnlSeries.push_back(currentPnL());
			m_slippageSeries.push_back(executedPrice - limitPrice);
		}

		// Risk management checks
		if (m_executedPrices.size() > 5)
		{
			const double avgSlip = RiskManagement::VWAPOrderExecutionRiskMetrics::computeAverageSlippage(m_executedPrices, m_vwapPrices);
			const double volSlip = RiskManagement::VWAPOrderExecutionRiskMetrics::computeStdDevSlippage(m_executedPrices, m_vwapPrices);
			const double mdd = RiskManagement::VWAPOrderExecutionRiskMetrics::computeMaxDrawdown(m_pnlSeries);
			const double skew = RiskManagement::VWAPOrderExecutionRiskMetrics::computeSkewness(m_slippageSeries);

			if (avgSlip > 5.0 || mdd > 100.0) 
			{
				HaltExecution();
			}
		}
	}
}

void VWAPStrategy::SendOrderToExchange(const double orderSize, const double limitPrice)
{

}

void VWAPStrategy::HaltExecution()
{

}

void VWAPStrategy::CreateBinanceExchangeProfile()
{
	for (const auto& symbol : m_targetFutureTradeSymbols)
	{
		m_tradingRules->GetExchangeProfileMgr()->UpdateRemoteExchangeProfiles(symbol, true);
		IncreaseComplianceRestAPIRequestCounter(1); // register a sent http request to ComplianceNRegulatory
	}
}

void VWAPStrategy::CreatePortfolioManagement()
{
	m_spotTrader->CreatePortfolioManagement(m_targetFutureTradeSymbols);
	IncreaseComplianceRestAPIRequestCounter(1); // register a sent http request to ComplianceNRegulatory
}

void VWAPStrategy::PrepareTargetMonitorSymbols()
{
	const auto* targetSymbolXml = m_strategyCfgXml->FirstChildElement("TargetSymbol");
	assert(targetSymbolXml);
	const XMLElement* symbolsXml = targetSymbolXml->FirstChildElement("AllSymbols");
	assert(symbolsXml);
	const bool useRemoteExchangeList = symbolsXml->BoolAttribute("UseRemoteExchangeList");
	if (useRemoteExchangeList)
	{
		m_logger->Info("Querying remote binance exchange listing symbols info...");
		//m_targetFutureTradeSymbols = StaticDataMgr->GetAllRemoteListingSymbols(true);
		m_targetFutureTradeSymbols.emplace_back("BTCUSDT");
		m_targetFutureTradeSymbols.emplace_back("ETHUSDT");
		m_targetFutureTradeSymbols.emplace_back("BNBUSDT");
#ifdef SAVE_BINANCE_LISTINGS // remove this macro to saving binance listings
		FileUtils::FromVectorStringToFile(m_targetFutureTradeSymbols, PathUtils::GetApplicationFolderPath()
			+ "\\Configurations\\Common\\BinanceListings.txt");
#endif // DEBUG
	}
	else
	{
		std::string localListingFile(symbolsXml->Attribute("LocalListingFile"));
		PathUtils::ReplaceSubString(localListingFile, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());
		m_targetFutureTradeSymbols = FileUtils::ReadFileContentToLines(localListingFile, true);
	}
}

void VWAPStrategy::SubscribeTargetSymbols()
{
	if (m_targetFutureTradeSymbols.empty())
	{
		throw std::runtime_error("No target symbols to subscribe market data.");
	}
	// register this class with market data to receive real time data
	m_marketData->RegisterDataListener(this);
	// subscibe all target symbols
	for (const auto& symbol : m_targetFutureTradeSymbols)
	{
		m_marketData->SubscribeSymbol(symbol);
	}
	m_marketData->StartIOContext();
}

void VWAPStrategy::UnsubscribeTargetSymbols()
{
	for (const auto& symbol : m_targetFutureTradeSymbols)
	{
		m_marketData->UnsubscribeSymbol(symbol);
	}
}
