/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "SmartLongShortStrategy.h"

#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/StringUtils.h"
#include "../MarketData/RealTimeMarketData.h"
#include "../MarketData/SynchronousMarketData.h"
#include "../StaticData/StaticDataManager.h"
#include "../UserAccount/FutureTrader.h"
#include "../ComplianceNRegulatory/BinanceTradingRules.h"
#include "../ComplianceNRegulatory/BinanceExchangeProfile.h"
#include "../QuantitativeModel/QuantOrderParammeter.h"
#include "../QuantitativeModel/MarketDataAnalyzer.h"
#include "../QuantitativeModel/QuantMarketDataAnalyzer.h"
#include "../RiskManagement/FutureRiskEngine.h"	
#include "../LibraryUtils/PathUtils.h"
#include "../LibraryUtils/FileUtils.h"


using namespace TradingStrategies;
using namespace QuantitativeModel;
using namespace MarketData;
using namespace UserAccount;
using namespace ComplianceNRegulatory;
using namespace LibraryUtils;
using namespace tinyxml2;
using namespace RiskManagement;

SmartLongShortStrategy::SmartLongShortStrategy(
	const std::string& strategyCfgPath,
	RealTimeMarketData* marketData,
	Trader* trader,
	BinanceTradingRules* tradingRules)
	: TradingStrategyBase("SmartLongShortStrategy", "Create future smart orders...",
		strategyCfgPath, marketData, trader, tradingRules),
	  AlarmSystem(LibraryUtils::DefaultAlarmInterval, AlarmSystem::AlarmMode::REPEAT)
{
	SetStrategyType(StrategyType::FULL_AUTO);
	InitializeParameters(strategyCfgPath);
	m_logger->Info("Completed initialization for the strategy.");
}

SmartLongShortStrategy::~SmartLongShortStrategy()
{
	m_marketData->UnRegisterDataListener(this); // I earn enough money, leave the market now!
}

bool SmartLongShortStrategy::OnIndividualBookTickerChange(MarketDataSubject* marketData, const std::string& symbol)
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

void SmartLongShortStrategy::ReportTradeResults(const std::string& symbol)
{
	m_futureTrader->ReportTradeResults(symbol);
}

void SmartLongShortStrategy::InitializeParameters(const std::string& strategyCfgPath)
{
	m_strategyCfgXml = std::make_unique<XMLDocument>();
	const auto errLoadFileXml = m_strategyCfgXml->LoadFile(strategyCfgPath.c_str());
	if (errLoadFileXml != XML_SUCCESS)
	{
		throw std::runtime_error("SmartLongShortStrategy: Load file Xml error="
			+ std::string(XMLDocument::ErrorIDToName(errLoadFileXml)) + ", error path:" + strategyCfgPath);
	}
	SetupStrategyLifeTime(m_strategyCfgXml.get());
	// when we use alarm system, we need to set up the order scheduler
	SetupOrderScheduler();
}

void SmartLongShortStrategy::InitializeMarketDataAnalyzer()
{
	m_marketDataAnalyzer = std::make_unique<QuantitativeModel::MarketDataAnalyzer>(m_targetFutureTradeSymbols, m_logger.get());
}

void SmartLongShortStrategy::SetupOrderScheduler()
{
	m_logger->Info("Setting up alarm system for order sending interval.");
	const XMLElement* generalConfigXml = m_strategyCfgXml->FirstChildElement("OrderScheduler");
	assert(generalConfigXml);
	const XMLElement* orderSendingInvervalXml = generalConfigXml->FirstChildElement("OrderSendingInverval");
	assert(orderSendingInvervalXml);
	const int64_t alarmIntervalSecond = orderSendingInvervalXml->Int64Attribute("AlarmIntervalSecond");
	if (alarmIntervalSecond <= 0)
	{
		throw std::runtime_error("SmartLongShortStrategy: Invalid alarm interval second="
			+ std::to_string(alarmIntervalSecond) + ", must be greater than 0.");
	}
	AlarmSystem::SetRepeatInterval(alarmIntervalSecond);
	m_strategyOrderScheduler = StrategyOrderScheduler::ALARM_BASED;
}

void SmartLongShortStrategy::StartLive()
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
	// Create risk management engine
	m_logger->Info("Create risk management engine.");
	CreateRiskManagementEngine();
	// Subscribe target symbols to receive real time market data
	m_logger->Info("Subscribe target symbols.");
	SubscribeTargetSymbols();
	// Start alarm system to send orders
	m_logger->Info("Starting live and trade.");
	AlarmSystem::Start();
}

void SmartLongShortStrategy::StopLive()
{
	m_strategyRunStatus = StrategyRunStatus::STOP;
	// Unsubscribe target symbols to stop receiving real time market data
	m_logger->Info("Unsubscribe target symbols.");
	UnsubscribeTargetSymbols();
}

void SmartLongShortStrategy::OnAlarmTriggered(const int passToDerived)
{
	for (const auto& symbol : m_targetFutureTradeSymbols)
	{
		auto* marketDataAnalyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol);
		std::unique_lock<std::mutex> lock(marketDataAnalyzer->m_mutex);

		// Test future orders
		QuantOrderParammeter futureOrder;
		futureOrder.m_symbol = symbol;
		futureOrder.m_type = binapi::e_type::limit; // default to market order
		futureOrder.m_time = binapi::e_time::GTC;// default to GTC
		futureOrder.m_amount = 1; // default to 1 coin
		futureOrder.m_tradeType = OrderManagement::BinanceNewOrderTradingType::FUTURE; // set to future trading type
		futureOrder.m_stableCurrency = "USDT"; // default stable currency is USDT
		// Set leverage ratio
		futureOrder.m_leverageRatio = 5; // default leverage ratio is x50

		QuantitativeModel::PriceTickerTrend priceTickerTrend = marketDataAnalyzer->GetMarketDataSignals().m_priceTickerTrend;

		if (priceTickerTrend == QuantitativeModel::PriceTickerTrend::DOWN_TREND)
		{
			// If the price ticker trend is down, we will create a short position
			futureOrder.m_side = binapi::e_side::sell;
			futureOrder.m_price = marketDataAnalyzer->GetMarketDataSignals().m_lastBestAskPrice.convert_to<double>(); // use last price as default price
		}
		else if (priceTickerTrend == QuantitativeModel::PriceTickerTrend::UP_TREND)
		{
			// If the price ticker trend is UP_TREND, we will create a long position
			futureOrder.m_side = binapi::e_side::buy;
			futureOrder.m_price = marketDataAnalyzer->GetMarketDataSignals().m_lastBestBidPrice.convert_to<double>(); // use last price as default price
		}
		else
		{
			m_logger->Warning("No valid price ticker trend for symbol=" + symbol + ", skipping order creation.");
			continue; // skip this symbol if no valid trend
		}		

		std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // simulate some delay

		const auto newFutureOrder = m_futureTrader->CreateNewPosition(futureOrder);
		if (newFutureOrder.first)
		{
			if (futureOrder.m_side == binapi::e_side::buy)
			{
				m_logger->Info("Created a new [Long Position] for symbol=" + symbol);
			}
			else if (futureOrder.m_side == binapi::e_side::sell)
			{
				m_logger->Info("Created a new [Short Position] for symbol=" + symbol);
			}
			IncreaseComplianceRestAPIRequestCounter(1); // register a sent http request to ComplianceNRegulatory
			ReportTradeResults(symbol);
			IncreaseComplianceRestAPIRequestCounter(2); // register a sent http request to ComplianceNRegulatory

			// test cancel order
			//const auto newFutureCancelOrder = m_futureTrader->CancelOpenPosition(newFutureOrder.second);
			//if (newFutureCancelOrder.first)
			//{
			//	m_logger->Info("Canceled the open position for symbol=" + symbol);
			//}
			//else
			//{
			//	m_logger->Error("Failed to cancel the open position for symbol=" + symbol);
			//}
			//IncreaseComplianceRestAPIRequestCounter(1); // register a sent http request to ComplianceNRegulatory
		}
	}
}

void SmartLongShortStrategy::CreateBinanceExchangeProfile()
{
	for (const auto& symbol : m_targetFutureTradeSymbols)
	{
		m_tradingRules->GetExchangeProfileMgr()->UpdateRemoteExchangeProfiles(symbol, true);
		IncreaseComplianceRestAPIRequestCounter(1); // register a sent http request to ComplianceNRegulatory
	}
}

void SmartLongShortStrategy::CreatePortfolioManagement()
{
	m_futureTrader->CreatePortfolioManagement(m_targetFutureTradeSymbols);
	IncreaseComplianceRestAPIRequestCounter(1); // register a sent http request to ComplianceNRegulatory
}

void SmartLongShortStrategy::CreateRiskManagementEngine()
{
	m_futureRiskEngine = std::make_unique<RiskManagement::FutureRiskEngine>(
		m_futureTrader->GetPortfolio(),
		m_futureTrader->GetRiskManager(),
		m_futureTrader->GetBinanceAccountInfo(),
		m_logger.get());
}

void SmartLongShortStrategy::PrepareTargetMonitorSymbols()
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

void SmartLongShortStrategy::SubscribeTargetSymbols()
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

void SmartLongShortStrategy::UnsubscribeTargetSymbols()
{
	for (const auto& symbol : m_targetFutureTradeSymbols)
	{
		m_marketData->UnsubscribeSymbol(symbol);
	}
}
