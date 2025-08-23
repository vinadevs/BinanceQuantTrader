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

bool VWAPStrategy::OnIndividualBookTickerChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisIndividualBookTicker(syncedData->m_individualBookTickerData);
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
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
		
	}
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
