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
#include "../QuantitativeModel/CostOfCarryFuturesPricing.h"
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
			std::lock_guard lock(m_marketDataMutex);
			auto& snapshot = m_marketDataSnapshots[symbol];
			snapshot->m_spotBestBidPrice = syncedData->GetSingleFeed(IndividualBookTickerID::BEST_BID_PRICE)->GetDoubleData();
			snapshot->m_spotBestAskPrice = syncedData->GetSingleFeed(IndividualBookTickerID::BEST_ASK_PRICE)->GetDoubleData();
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
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisTrade(syncedData->m_tradeData);
			std::lock_guard lock(m_marketDataMutex);
			auto& snapshot = m_marketDataSnapshots[symbol];
			snapshot->m_spotLastPrice = syncedData->GetSingleFeed(TradeID::PRICE)->GetDoubleData();
			snapshot->m_spotLastTradeVolume = syncedData->GetSingleFeed(TradeID::QUANTITY)->GetDoubleData();
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool DerivativesArbitrageStrategy::OnBookDataFutureChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisFutureBook(syncedData->m_futureBookData);
			std::lock_guard lock(m_marketDataMutex);
			auto& snapshot = m_marketDataSnapshots[symbol];
			snapshot->m_futureBestBidPrice = syncedData->GetSingleFeed(FutureBookTickerID::BEST_BID_PRICE)->GetDoubleData();
			snapshot->m_futureBestAskPrice = syncedData->GetSingleFeed(FutureBookTickerID::BEST_ASK_PRICE)->GetDoubleData();
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool DerivativesArbitrageStrategy::OnTradeDataFutureChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisFutureTrade(syncedData->m_futureTradeData);
			std::lock_guard lock(m_marketDataMutex);
			auto& snapshot = m_marketDataSnapshots[symbol];
			snapshot->m_futureLastPrice = syncedData->GetSingleFeed(FutureTradeID::PRICE)->GetDoubleData();
			snapshot->m_futureLastTradeVolume = syncedData->GetSingleFeed(FutureTradeID::QUANTITY)->GetDoubleData();
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool DerivativesArbitrageStrategy::OnFutureFundingDataChange(MarketData::MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisFutureFunding(syncedData->m_futureFundingData);
			std::lock_guard lock(m_marketDataMutex);
			auto& snapshot = m_marketDataSnapshots[symbol];
			snapshot->m_fundingRate = syncedData->GetSingleFeed(FutureFundingRateID::FUNDING_RATE)->GetDoubleData();
			snapshot->m_timeToExpiry = syncedData->GetSingleFeed(FutureFundingRateID::FUNDING_TIME)->GetUnsignedIntData();
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
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
	const XMLElement* algorithmConfigXml = m_strategyCfgXml->FirstChildElement("Algorithm");
	assert(algorithmConfigXml);
	const XMLElement* arbitrageTypeXml = algorithmConfigXml->FirstChildElement("ArbitrageType");
	assert(arbitrageTypeXml);
	if (arbitrageTypeXml->Attribute("Value") == "QuoteSpotAndHedgeFuture")
	{
		m_arbitrageType = ArbitrageType::QUOTE_SPOT_AND_HEDGE_FUTURE;
	}
	else if (arbitrageTypeXml->Attribute("Value") == "QuoteFutureAndHedgeSpot")
	{
		m_arbitrageType = ArbitrageType::QUOTE_FUTURE_AND_HEDGE_SPOT;
	}
	else if (arbitrageTypeXml->Attribute("Value") == "DynamicQuoteAndHedge")
	{
		m_arbitrageType = ArbitrageType::DYNAMIC_QUOTE_AND_HEDGE;
	}
	else
	{
		throw std::runtime_error("DerivativesArbitrageStrategy: Invalid arbitrage type="
			+ std::string(arbitrageTypeXml->Attribute("Value")));
	}
	const XMLElement* entryThresholdXml = algorithmConfigXml->FirstChildElement("EntryEdgePriceThreshold");
	assert(entryThresholdXml);
	m_entryThreshold = entryThresholdXml->DoubleAttribute("Value");
	if (m_entryThreshold <= 0)
	{
		throw std::runtime_error("DerivativesArbitrageStrategy: Invalid entry edge price threshold="
			+ std::to_string(m_entryThreshold) + ", must be greater than 0.");
	}
	SetupStrategyLifeTime(m_strategyCfgXml.get());
	// when we use alarm system, we need to set up the order scheduler
	SetupOrderScheduler();
}

void DerivativesArbitrageStrategy::InitializeMarketDataAnalyzer()
{
	m_marketDataAnalyzer = std::make_unique<QuantitativeModel::MarketDataAnalyzer>(m_targetTradeSymbols, m_logger.get());
}

void DerivativesArbitrageStrategy::InitializeMarketDataSnapshots()
{
	for (const auto& symbol : m_targetTradeSymbols)
	{
		m_marketDataSnapshots.emplace(symbol, std::make_unique<MarketDataSnapshot>());
	}
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
		// Create pricing model
		CreatePricingModels();
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
	BEGIN_STRATEGY_ORDER_SENDING_ACTIVITY

	m_logger->Info("Alarm triggered, start sending orders based on fair pricing model...");

	std::lock_guard lock(m_marketDataMutex); // only lock here and dont add any blocking code inside child functions

	for (const auto& symbol : m_targetTradeSymbols)
	{
		// Get the latest market data snapshot for the symbol, which is updated in real time by the market data event handlers,
		// and we use this snapshot to calculate the fair value and edge for order sending
		const auto& snapshot = m_marketDataSnapshots[symbol];
		m_logger->Info("Trading for symbol=" + symbol);

		// Calculate the mid price of the spot as the reference price for fair value calculation,
		// we can also use other price like last price or mark price, but mid price is more commonly 
		// used in arbitrage strategy as it represents the current market consensus price and is
		// less likely to be manipulated by large orders
		const double spotMid = 0.5 * (snapshot->m_spotBestBidPrice + snapshot->m_spotBestAskPrice);

		// Fair value calculation based on cost of carry model, which takes into account the spot price, 
		// funding rate and time to expiry of the future contract
		const double fairPrice = m_fairValueModel->Compute({ spotMid, snapshot->m_fundingRate, snapshot->m_timeToExpiry });

		// Calculate the edge of the trade, which is the price difference between the future and the fair value, 
		// this edge should be greater than the entry threshold from external config to enter a trade
		const double edgeDiff = snapshot->m_futureBestBidPrice - fairPrice;

		// Risk management calculation, we calculate the greeks for both spot and future positions and combine them
		// to get the overall risk exposure of the trade

		auto symbolInfo = GetSymbolInfo(symbol);
		const auto totalTradedVolumeForSymbol = symbolInfo.GetTotalTradedVolume();
		auto geeksSpot = GreeksCalculator::Spot(totalTradedVolumeForSymbol);
		auto geeksFuture = GreeksCalculator::Futures(totalTradedVolumeForSymbol, snapshot->m_fundingRate);
		auto geeksAggregate = GreeksCalculator::Combine(geeksSpot, geeksFuture);

		// Risk check
		const double pnl = { 0 }; // TODO - we can also calculate the current pnl based on the market data snapshot and the position information from portfolio management
		if (m_riskModel->CanTradeNow(geeksAggregate, totalTradedVolumeForSymbol, edgeDiff, pnl, snapshot->m_marketSpotCummulativeVolume) == false)
			return; // if risk check failed, skip sending orders and wait for next alarm trigger

		// Generate orders based on edge and risk check, here we simply use a fixed threshold for demo, but in real case
		// we can dynamically adjust the threshold based on market conditions and risk level
		if (edgeDiff > m_entryThreshold)
		{
			double qty = 0.01;
			// Send orders to open positions, we can also use different order types like limit order or market order based on the market conditions and risk level
			symbolInfo.SetTotalTradedVolume(totalTradedVolumeForSymbol + qty);
		}
	}

	END_STRATEGY_ORDER_SENDING_NO_RETURN
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

void DerivativesArbitrageStrategy::CreatePricingModels()
{
	m_fairValueModel = std::make_unique<QuantitativeModel::CostOfCarryFuturesPricer>();
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
		m_targetTradeSymbols = StaticDataMgr->GetAllRemoteListingSymbols(true);
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
		if (m_marketData->SubscribeSymbol(symbol))
		{
			m_symbolMonitorInfos.emplace(symbol, KernelTrading::BqtSymbol(symbol, KernelTrading::SymbolType::SPOT));
		}
	}
}

void DerivativesArbitrageStrategy::UnsubscribeTargetSymbols()
{
	for (const auto& symbol : m_targetTradeSymbols)
	{
		m_marketData->UnsubscribeSymbol(symbol);
	}
}

KernelTrading::BqtSymbol& DerivativesArbitrageStrategy::GetSymbolInfo(const std::string& symbol)
{
	auto iter = m_symbolMonitorInfos.find(symbol);
	if (iter == m_symbolMonitorInfos.end())
	{
		throw std::runtime_error("Symbol info not found for symbol=" + symbol);
	}
	return iter->second;
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