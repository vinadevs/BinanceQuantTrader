/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "DerivativesArbitrageStrategy.h"

#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/StringUtils.h"
#include "../MarketData/RealTimeMarketData.h"
#include "../MarketData/SynchronousMarketData.h"
#include "../StaticData/StaticDataManager.h"
#include "../UserAccount/HybridTrader.h"
#include "../ComplianceNRegulatory/BinanceTradingRules.h"
#include "../ComplianceNRegulatory/BinanceExchangeProfile.h"
#include "../QuantitativeModel/QuantOrderParammeter.h"
#include "../QuantitativeModel/MarketDataAnalyzer.h"
#include "../QuantitativeModel/QuantMarketDataAnalyzer.h"
#include "../RiskManagement/DerivativesRiskModel.h"
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

DerivativesArbitrageStrategy::DerivativesArbitrageStrategy(
	const std::string& strategyCfgPath,
	RealTimeMarketData* marketData,
	Trader* trader,
	BinanceTradingRules* tradingRules)
	: TradingStrategyBase("DerivativesArbitrageStrategy", "Model Cash & Carry arbitrage principles...",
		strategyCfgPath, marketData, trader, tradingRules),
	AlarmSystem(LibraryUtils::DefaultAlarmInterval, AlarmSystem::AlarmMode::REPEAT)
{
	START_STRATEGY_INITIALIZATION_SECTION

	SetStrategyType(StrategyType::FULL_AUTO);
	InitializeParameters(strategyCfgPath);
	m_logger->Info("Completed initialization for the strategy.");

	END_STRATEGY_INITIALIZATION_SECTION
}

DerivativesArbitrageStrategy::~DerivativesArbitrageStrategy()
{
	m_marketData->UnRegisterDataListener(this); // I earn enough money, leave the market now!
}

bool DerivativesArbitrageStrategy::OnIndividualBookTickerChange(MarketDataSubject* marketData, const std::string& symbol)
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

bool DerivativesArbitrageStrategy::OnTradeChange(MarketDataSubject* marketData, const std::string& symbol)
{
	return false;
}

bool DerivativesArbitrageStrategy::OnBookDataFutureChange(MarketDataSubject* marketData, const std::string& symbol)
{
	return false;
}

bool DerivativesArbitrageStrategy::OnTradeDataFutureChange(MarketDataSubject* marketData, const std::string& symbol)
{
	return false;
}

void DerivativesArbitrageStrategy::ReportTradeResults(const std::string& symbol)
{
	m_hybridTrader->ReportTradeResults(symbol);
}

void DerivativesArbitrageStrategy::InitializeParameters(const std::string& strategyCfgPath)
{
	m_strategyCfgXml = std::make_unique<XMLDocument>();
	const auto errLoadFileXml = m_strategyCfgXml->LoadFile(strategyCfgPath.c_str());
	if (errLoadFileXml != XML_SUCCESS)
	{
		throw std::runtime_error("DerivativesArbitrageStrategy: Load file Xml error="
			+ std::string(XMLDocument::ErrorIDToName(errLoadFileXml)) + ", error path:" + strategyCfgPath);
	}
	SetupStrategyLifeTime(m_strategyCfgXml.get());
	// when we use alarm system, we need to set up the order scheduler
	SetupOrderScheduler();
}

void DerivativesArbitrageStrategy::InitializeMarketDataAnalyzer()
{
	m_marketDataAnalyzer = std::make_unique<QuantitativeModel::MarketDataAnalyzer>(m_targetTradeSymbols, m_logger.get());
}

void DerivativesArbitrageStrategy::SetupOrderScheduler()
{
	m_logger->Info("Setting up alarm system for order sending interval.");
	const XMLElement* generalConfigXml = m_strategyCfgXml->FirstChildElement("OrderScheduler");
	assert(generalConfigXml);
	const XMLElement* orderSendingInvervalXml = generalConfigXml->FirstChildElement("OrderSendingInverval");
	assert(orderSendingInvervalXml);
	const int64_t alarmIntervalSecond = orderSendingInvervalXml->Int64Attribute("AlarmIntervalSecond");
	if (alarmIntervalSecond <= 0)
	{
		throw std::runtime_error("DerivativesArbitrageStrategy: Invalid alarm interval second="
			+ std::to_string(alarmIntervalSecond) + ", must be greater than 0.");
	}
	AlarmSystem::SetRepeatInterval(alarmIntervalSecond);
	m_strategyOrderScheduler = StrategyOrderScheduler::ALARM_BASED;
}

void DerivativesArbitrageStrategy::StartTrade()
{
	try
	{
		// Change Strategy state to live
		m_strategyRunStatus.store(StrategyRunStatus::LIVE, std::memory_order_release);
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
		CreateRiskManagementEngines();
		// Subscribe target symbols to receive real time market data
		m_logger->Info("Subscribe target symbols.");
		SubscribeTargetSymbols();
		// Start alarm system to send orders
		m_logger->Info("Starting live and trade.");
		AlarmSystem::Start();
	}
	catch (const std::exception& e)
	{
		m_logger->Exception(std::string(e.what()));
	}
	catch (...)
	{
		m_logger->Exception("Unknown exception occurred.");
	}
}

void DerivativesArbitrageStrategy::StopTrade()
{
	m_strategyRunStatus.store(StrategyRunStatus::STOP, std::memory_order_release);
	// Unsubscribe target symbols to stop receiving real time market data
	m_logger->Info("Unsubscribe target symbols.");
	UnsubscribeTargetSymbols();
}

void DerivativesArbitrageStrategy::OnAlarmTriggered(const int passToDerived)
{
}

void DerivativesArbitrageStrategy::CreateBinanceExchangeProfile()
{
	for (const auto& symbol : m_targetTradeSymbols)
	{
		m_tradingRules->GetExchangeProfileMgr()->UpdateRemoteExchangeProfiles(symbol, true);
		IncreaseComplianceRestAPIRequestCounter(BinanceTradingRules::SINGLE_REQUEST); // register a sent http request to ComplianceNRegulatory
	}
}

void DerivativesArbitrageStrategy::CreatePortfolioManagement()
{
	m_hybridTrader->GetSpotTrader()->CreatePortfolioManagement(m_targetTradeSymbols);
	IncreaseComplianceRestAPIRequestCounter(BinanceTradingRules::SINGLE_REQUEST); // register a sent http request to ComplianceNRegulatory
	m_hybridTrader->GetFutureTrader()->CreatePortfolioManagement(m_targetTradeSymbols);
	IncreaseComplianceRestAPIRequestCounter(BinanceTradingRules::SINGLE_REQUEST); // register a sent http request to ComplianceNRegulatory
}

void DerivativesArbitrageStrategy::CreateRiskManagementEngines()
{
	const auto* riskModelXml = m_strategyCfgXml->FirstChildElement("RiskTradingLimits");
	assert(riskModelXml);
	const auto* metrics = riskModelXml->FirstChildElement("Metrics");
	m_riskModel = std::make_unique<RiskManagement::DerivativesRiskModel>(metrics);
}

void DerivativesArbitrageStrategy::PrepareTargetMonitorSymbols()
{
	const auto* targetSymbolXml = m_strategyCfgXml->FirstChildElement("TargetSymbol");
	assert(targetSymbolXml);
	const XMLElement* symbolsXml = targetSymbolXml->FirstChildElement("AllSymbols");
	assert(symbolsXml);
	const bool useRemoteExchangeList = symbolsXml->BoolAttribute("UseRemoteExchangeList");
	if (useRemoteExchangeList)
	{
		m_logger->Info("Querying remote binance exchange listing symbols info...");
		//m_targetTradeSymbols = StaticDataMgr->GetAllRemoteListingSymbols(true);
		m_targetTradeSymbols.emplace_back("BTCUSDT");
		m_targetTradeSymbols.emplace_back("ETHUSDT");
		m_targetTradeSymbols.emplace_back("BNBUSDT");
#ifdef SAVE_BINANCE_LISTINGS // remove this macro to saving binance listings
		FileUtils::FromVectorStringToFile(m_targetTradeSymbols,
			(std::filesystem::path(PathUtils::GetApplicationFolderPath()) / "Configurations" / "Common" / "BinanceListings.txt").string());
#endif // DEBUG
	}
	else
	{
		std::string localListingFile(symbolsXml->Attribute("LocalListingFile"));
		PathUtils::ReplaceSubString(localListingFile, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());
		m_targetTradeSymbols = FileUtils::ReadFileContentToLines(localListingFile, true);
	}
}

void DerivativesArbitrageStrategy::SubscribeTargetSymbols()
{
	if (m_targetTradeSymbols.empty())
	{
		throw std::runtime_error("No target symbols to subscribe market data.");
	}
	// register this class with market data to receive real time data
	m_marketData->RegisterDataListener(this);
	// subscibe all target symbols
	for (const auto& symbol : m_targetTradeSymbols)
	{
		m_marketData->SubscribeSymbol(symbol);
	}
}

void DerivativesArbitrageStrategy::UnsubscribeTargetSymbols()
{
	for (const auto& symbol : m_targetTradeSymbols)
	{
		m_marketData->UnsubscribeSymbol(symbol);
	}
}

// DOWNSTREAM ACKS --------------------------------------------------------------------------------------------------

void DerivativesArbitrageStrategy::OnOrderOpeningPositionAck(const OrderManagement::BinanceNewOrder* openingOrder)
{
}

void DerivativesArbitrageStrategy::OnOrderClosedPositionAck(const OrderManagement::BinanceNewOrder* closedOrder)
{
}

void DerivativesArbitrageStrategy::OnOrderLiquidatedPositionAck(const OrderManagement::BinanceNewOrder* liquidatedOrder)
{
}

void DerivativesArbitrageStrategy::OnOrderMarginCalledPositionAck(const OrderManagement::BinanceNewOrder* marginCalledOrder)
{
}