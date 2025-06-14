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
	BinanceTrader* trader,
	BinanceTradingRules* tradingRules)
	: TradingStrategyBase("FomoTradingStrategy", "The fear of missing out...",
		strategyCfgPath, marketData, trader, tradingRules)
{
	SetStrategyType(StrategyType::FULL_AUTO);
	InitializeParameters(strategyCfgPath);
	// Subscribe target symbols to receive real time market data
	m_logger->Info("Subscribe target symbols.");
	SubscribeTargetSymbols();
	m_logger->Info("Completed initialization for the strategy.");
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
	m_tradingSignalService = std::make_unique<TradingSignalService>(m_trader->GetPortfolio(), signalXml);
	m_tradingSignalService->RegisterTradingHintsListener(this);
}

void FomoTradingStrategy::SubscribeTargetSymbols()
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
	// register TradingSignalService class with market data to receive real time data
	m_marketData->RegisterDataListener(m_tradingSignalService.get());
	// subscibe all target symbols
	for (const auto& symbol : m_targetTradeSymbols)
	{
		m_marketData->SubscribeSymbol(symbol);
	}
	m_marketData->StartIOContext();
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
	m_trader->CreatePortfolioManagement(m_targetTradeSymbols);
	IncreaseComplianceRestAPIRequestCounter(1); // register a sent http request to ComplianceNRegulatory
}

void FomoTradingStrategy::CreateOrderParameterGenerator()
{
	m_orderParammeterGenerator = std::make_unique<OrderParammeterGenerator>(
		m_trader->GetTradingRules(),
		m_trader->GetPortfolio(),
		m_trader->GetRiskManager(),
		m_trader->GetPositionManager(),
		m_logger.get());
}

void FomoTradingStrategy::CreateBinanceExchangeProfile()
{
	for (const auto& symbol : m_targetTradeSymbols)
	{
		m_tradingRules->GetExchangeProfileMgr()->UpdateRemoteExchangeProfiles(symbol, true);
		IncreaseComplianceRestAPIRequestCounter(1); // register a sent http request to ComplianceNRegulatory
	}
}

bool FomoTradingStrategy::TradeAsHints(const TradingHints* hints)
{
	try
	{
		if (m_strategyRunStatus == StrategyRunStatus::LIVE)
		{
			if (IsNotIsNotExceededTradingRules())
			{
				m_logger->Info("Creating order parameters for symbol=" + hints->symbol);
				const auto orderList = m_orderParammeterGenerator->GenerateFomoOrders(hints);
				for (const auto& order : orderList)
				{
					if (m_trader->CreateNewPosition(order))
					{
						if (order.m_side == binapi::e_side::buy)
						{
							m_logger->Info("Created a new [Long Position] for symbol=" + hints->symbol);
						}
						else if (order.m_side == binapi::e_side::sell)
						{
							m_logger->Info("Created a new [Short Position] for symbol=" + hints->symbol);
						}
						IncreaseComplianceRestAPIRequestCounter(1); // register a sent http request to ComplianceNRegulatory
						ReportTradeResults(hints->symbol);
						IncreaseComplianceRestAPIRequestCounter(2); // register a sent http request to ComplianceNRegulatory
					}
				}		
			}
			else
			{
				m_logger->Debug("Strategy received a hint signal but it is exceeded exchange rule/limitations.");
			}
		}
		else
		{
			m_logger->Warning("Strategy received a hint signal but it is not living now.");
		}
	}
	catch (const std::exception& e)
	{
		m_logger->Exception(std::string(e.what()));
		return false;
	}
	catch (...)
	{
		m_logger->Exception("Unknown exception occurred.");
		return false;
	}
	return true;
}

void FomoTradingStrategy::ReportTradeResults(const std::string& symbol)
{
	m_trader->ReportTradeResults(symbol);
}

void FomoTradingStrategy::StartLive()
{
	// Change Strategy state to live
	m_strategyRunStatus = StrategyRunStatus::LIVE;
	// Create exchange filter profile
	m_logger->Info("Create binance exchange profile.");
	CreateBinanceExchangeProfile();
	// Create portfolio management
	m_logger->Info("Create portfolio management.");
	CreatePortfolioManagement();
	// Creating trending services
	m_logger->Info("Creating trending services.");
	CreateTradingSignalServices();
	// Create order parammeter generator
	m_logger->Info("Create order parammeter generator.");
	CreateOrderParameterGenerator();
	// Listen on trading hints
	m_logger->Info("Starting live and trade.");
#if USE_MULTITHREADING
	TradingLoop();
#endif
}

void FomoTradingStrategy::StopLive()
{
#if USE_MULTITHREADING
	m_isThreadTradeOngoing.store(false); // break while loop
#endif
	// Change Strategy state to stop
	m_strategyRunStatus = StrategyRunStatus::STOP;
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