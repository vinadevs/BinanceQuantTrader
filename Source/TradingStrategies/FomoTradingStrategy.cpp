/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "FomoTradingStrategy.h"

#include "../MarketData/RealTimeMarketData.h"
#include "../UserAccount/BinanceTrader.h"
#include "../SettingNConfig/tinyxml2.h"
#include "../ComplianceNRegulatory/BinanceTradingRules.h"
#include "../ComplianceNRegulatory/BinanceExchangeProfile.h"
#include "../PortfolioManager/PortfolioInvestmentBinance.h"
#include "../QuantitativeModel/OrderParammeterGenerator.h"
#include "../LibraryUtils/StringUtils.h"
#include "../RiskManagement/SpotRiskEngine.h"

using namespace TradingStrategies;
using namespace QuantitativeModel;
using namespace IndicatorNSignals;
using namespace MarketData;
using namespace UserAccount;
using namespace ComplianceNRegulatory;
using namespace LibraryUtils;
using namespace tinyxml2;

#define WAIT_TIME_OUT std::chrono::seconds(1)

FomoTradingStrategy::FomoTradingStrategy(
	const std::string& strategyCfgPath,
	RealTimeMarketData* marketData,
	Trader* trader,
	BinanceTradingRules* tradingRules)
	: TradingStrategyBase("FomoTradingStrategy", "The fear of missing out...",
		strategyCfgPath, marketData, trader, tradingRules)
{
	START_STRATEGY_INITIALIZATION_SECTION

	SetStrategyType(StrategyType::FULL_AUTO);
	InitializeParameters(strategyCfgPath);
	m_logger->Info("Completed initialization for the strategy.");

	END_STRATEGY_INITIALIZATION_SECTION
}

void FomoTradingStrategy::InitializeParameters(const std::string& strategyCfgPath)
{
	m_strategyCfgXml = std::make_unique<XMLDocument>();
	const auto errLoadFileXml = m_strategyCfgXml->LoadFile(strategyCfgPath.c_str());
	if (errLoadFileXml != XML_SUCCESS)
	{
		throw std::runtime_error("FomoTradingStrategy: Load file Xml error=" 
			+ std::string(XMLDocument::ErrorIDToName(errLoadFileXml)) + ", error path:" + strategyCfgPath);
	}
	SetupStrategyLifeTime(m_strategyCfgXml.get());
}

FomoTradingStrategy::~FomoTradingStrategy()
{
	m_tradingSignalService->UnregisterTradingHintsListener(this);
	m_marketData->UnRegisterDataListener(m_tradingSignalService.get());
}

bool FomoTradingStrategy::OnReceivedTradingHints(const TradingHints* hints)
{
#if USE_MULTITHREADING
	std::unique_lock<std::mutex> lock(m_tradingHintsMutex);
	m_tradingHintsQueue.PushItem(hints);
	m_hasNewTradingHint.store(true);
	m_tradingHintCond.notify_one();
	return true;
#else
	return TradeAsHints(hints);
#endif
}

void FomoTradingStrategy::CreateTradingSignalServices()
{
	const auto* signalXml = m_strategyCfgXml->FirstChildElement("IndicatorAndSignals");
	assert(signalXml);
	m_tradingSignalService = std::make_unique<TradingSignalService>(m_spotTrader->GetPortfolio(), signalXml);
	m_tradingSignalService->RegisterTradingHintsListener(this);
}

void FomoTradingStrategy::LoadTargetTradeSymbols()
{
	const auto* targetSymbolXml = m_strategyCfgXml->FirstChildElement("TargetSymbol");
	assert(targetSymbolXml);
	const XMLElement* symbolsXml = targetSymbolXml->FirstChildElement("Symbols");
	assert(symbolsXml);
	m_targetTradeSymbols = StringUtils::SplitAndTrimString(symbolsXml->Attribute("List"), ',');
	if (m_targetTradeSymbols.empty())
	{
		throw std::runtime_error("No target symbols to subscribe market data.");
	}
}

void FomoTradingStrategy::SubscribeTargetSymbols()
{
	// register TradingSignalService class with market data to receive real time data
	m_marketData->RegisterDataListener(m_tradingSignalService.get());
	// subscibe all target symbols
	for (const auto& symbol : m_targetTradeSymbols)
	{
		m_marketData->SubscribeSymbol(symbol);
	}
}

void FomoTradingStrategy::UnsubscribeTargetSymbols()
{
	for (const auto& symbol : m_targetTradeSymbols)
	{
		m_marketData->UnsubscribeSymbol(symbol);
	}
}

void FomoTradingStrategy::CreatePortfolioManagement()
{
	m_spotTrader->CreatePortfolioManagement(m_targetTradeSymbols);
	IncreaseComplianceRestAPIRequestCounter(BinanceTradingRules::SINGLE_REQUEST); // register a sent http request to ComplianceNRegulatory
}

void FomoTradingStrategy::CreateOrderParameterGenerator()
{
	m_orderParammeterGenerator = std::make_unique<OrderParammeterGenerator>(
		m_spotTrader->GetTradingRules(),
		m_spotTrader->GetPortfolio(),
		m_spotTrader->GetRiskManager(),
		m_spotTrader->GetPositionManager(),
		m_logger.get());
}

void FomoTradingStrategy::CreateBinanceExchangeProfile()
{
	for (const auto& symbol : m_targetTradeSymbols)
	{
		m_tradingRules->GetExchangeProfileMgr()->UpdateRemoteExchangeProfiles(symbol, true);
		IncreaseComplianceRestAPIRequestCounter(BinanceTradingRules::SINGLE_REQUEST); // register a sent http request to ComplianceNRegulatory
	}
}

void FomoTradingStrategy::CreateRiskManagementEngine()
{
	m_spotRiskEngine = std::make_unique<RiskManagement::SpotRiskEngine>(
		m_spotTrader->GetPortfolio(),
		m_spotTrader->GetRiskManager(),
		m_spotTrader->GetBinanceAccountInfo(),
		m_logger.get());
}

bool FomoTradingStrategy::TradeAsHints(const TradingHints* hints)
{
	BEGIN_STRATEGY_ORDER_SENDING_ACTIVITY

	m_logger->Info("Creating order parameters for symbol=" + hints->symbol);
	const auto orderList = m_orderParammeterGenerator->GenerateFomoOrders(hints);
	if (orderList.has_value()) 
	{
		for (const auto& order : orderList.value())
		{
			if (m_spotTrader->CreateNewPosition(order).first)
			{
				if (order.m_side == binapi::e_side::buy)
				{
					m_logger->Info("Created a new [Long Position] for symbol=" + hints->symbol);
				}
				else if (order.m_side == binapi::e_side::sell)
				{
					m_logger->Info("Created a new [Short Position] for symbol=" + hints->symbol);
				}
				IncreaseComplianceRestAPIRequestCounter(BinanceTradingRules::SINGLE_REQUEST); // register a sent http request to ComplianceNRegulatory
				ReportTradeResults(hints->symbol);
				IncreaseComplianceRestAPIRequestCounter(BinanceTradingRules::DOUBLE_REQUEST); // register a sent http request to ComplianceNRegulatory
			}
		}
	}
	END_STRATEGY_ORDER_SENDING_RETURN

	return true;
}

void FomoTradingStrategy::ReportTradeResults(const std::string& symbol)
{
	m_spotTrader->ReportTradeResults(symbol);
}

void FomoTradingStrategy::StartTrade()
{
	try
	{
		// Change Strategy state to live
		m_strategyRunStatus.store(StrategyRunStatus::LIVE, std::memory_order_release);
		// Load target symbols list
		LoadTargetTradeSymbols();
		// Create exchange filter profile
		m_logger->Info("Create binance exchange profile.");
		CreateBinanceExchangeProfile();
		// Create portfolio management
		m_logger->Info("Create portfolio management.");
		CreatePortfolioManagement();
		// Creating trending services
		m_logger->Info("Creating trending services.");
		CreateTradingSignalServices();
		// Subscribe target symbols to receive real time market data
		m_logger->Info("Subscribe target symbols.");
		SubscribeTargetSymbols();
		// Create order parammeter generator
		m_logger->Info("Create order parammeter generator.");
		CreateOrderParameterGenerator();
		// Listen on trading hints
		m_logger->Info("Starting live and trade.");
#if USE_MULTITHREADING
		TradingLoop();
#endif
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

void FomoTradingStrategy::StopTrade()
{
#if USE_MULTITHREADING
	m_isThreadTradeOngoing.store(false); // break while loop
#endif
	// Change Strategy state to stop
	m_strategyRunStatus.store(StrategyRunStatus::STOP, std::memory_order_release);
	// Unsubscribe target symbols to stop receiving real time market data
	m_logger->Info("Unsubscribe target symbols.");
	UnsubscribeTargetSymbols();
}

#if USE_MULTITHREADING
// Thread's main trading loop
void FomoTradingStrategy::TradingLoop()
{
	m_isThreadTradeOngoing.store(true);
	while (m_isThreadTradeOngoing.load())
	{
		std::unique_lock<std::mutex> lock(m_tradingHintsMutex);
		// Wait for new trading hint or timeout after 1 second
		m_tradingHintCond.wait_for(lock, WAIT_TIME_OUT, [&]()
		{
			return m_hasNewTradingHint.load();
		});

		// If new trading hint is available, process it
		if (m_hasNewTradingHint.load())
		{
		    while (!m_tradingHintsQueue.IsEmpty())
			{
				const TradingHints* hint = m_tradingHintsQueue.GetFifoItem();
		        lock.unlock();  // Unlock mutex during processing
				TradeAsHints(hint);
		        lock.lock();  // Lock mutex again for the next iteration
		    }
			m_hasNewTradingHint.store(false); // Reset the flag after processing
		}
	}
}
#endif

// DOWNSTREAM ACKS --------------------------------------------------------------------------------------------------

void FomoTradingStrategy::OnOrderFilledAck(const OrderManagement::BinanceNewOrder* filledOrder)
{
	m_spotRiskEngine->AssessTradingRisk(filledOrder);
}

void FomoTradingStrategy::OnOrderPartiallyFilledAck(const OrderManagement::BinanceNewOrder* partiallyFilledOrder)
{
}

void FomoTradingStrategy::OnOrderRejectedAck(const OrderManagement::BinanceNewOrder* rejectedOrder)
{
}