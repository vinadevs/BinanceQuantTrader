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

using namespace TradingStrategies;
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
	InitializeParameters(strategyCfgPath);
	m_logger->Info("Initialized trading parameters for strategy.");
}

void FomoTradingStrategy::InitializeParameters(const std::string& strategyCfgPath)
{
	auto strategyCfgPathXml = std::make_unique<XMLDocument>();
	const auto errLoadFileXml = strategyCfgPathXml->LoadFile(strategyCfgPath.c_str());
	if (errLoadFileXml != XML_SUCCESS)
	{
		throw std::runtime_error("FomoTradingStrategy: Load file Xml error=" 
			+ std::string(XMLDocument::ErrorIDToName(errLoadFileXml)) + ", error path:" + strategyCfgPath);
	}
	SetupStrategyLifeTime(strategyCfgPathXml.get());
	const auto* signalXml = strategyCfgPathXml->FirstChildElement("IndicatorAndSignals");
	assert(signalXml);
	m_tradingSignalService = std::make_unique<TradingSignalService>(m_trader->GetPortfolio(), signalXml);
	m_tradingSignalService->RegisterTradingHintsListener(this);
	m_marketData->RegisterDataListener(m_tradingSignalService.get());
}

FomoTradingStrategy::~FomoTradingStrategy()
{
	m_tradingSignalService->UnregisterTradingHintsListener(this);
	m_marketData->UnRegisterDataListener(m_tradingSignalService.get());
}

bool FomoTradingStrategy::OnReceivedTradingHints(const TradingHints* hints)
{
#if USE_MULTITHREADING
	std::unique_lock<std::mutex> lock(m_marketDataMutex);
	m_tradingHintsQueue.PushItem(hints);
	m_hasNewTradingHint.store(true);
	m_tradingHintCond.notify_one();
	return true;
#else
	return TradeAsHints(hints);
#endif
}

bool FomoTradingStrategy::TradeAsHints(const TradingHints* hints)
{
	try
	{
		if (m_strategyRunStatus == StrategyRunStatus::LIVE)
		{
			if (IsNotIsNotExceededTradingRules())
			{
				const auto* symbolProfile = m_tradingRules->GetExchangeProfileMgr()->LookupExchangeProfile(hints->symbol);
				if (symbolProfile)
				{
					Sleep(1000); // delay to avoid bans while testing
					if (hints->isUpTrend)
					{
						if (m_trader->CreateLongPosition(hints->symbol,
							symbolProfile->m_minTradeAmount, hints->windowBestBidPrice.convert_to<double>()))
						{
							IncreaseOrderCounter(); // register a sent order request to ComplianceNRegulatory
						}
					}
					else if (hints->isDownTrend)
					{
						if (m_trader->CreateShortPosition(hints->symbol,
							symbolProfile->m_minTradeAmount, hints->windowBestAskPrice.convert_to<double>()))
						{
							IncreaseOrderCounter(); // register a sent order request to ComplianceNRegulatory
						}
					}
				}
				else
				{
					m_logger->Error("Strategy could not lookup Exchange Profile for symbol=" + hints->symbol);
				}
			}
			else
			{
				m_logger->Warning("Strategy received a hint signal but it is exceeded exchange limitations.");
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

#ifndef USE_BACK_TEST_TRADING
void FomoTradingStrategy::ReportTradeResults(const std::string& symbol)
{
	m_trader->ReportTradeData(symbol);
}
#endif

void FomoTradingStrategy::StartLive()
{
	m_strategyRunStatus = StrategyRunStatus::LIVE;
#if USE_MULTITHREADING
	TradingLoop();
#endif
}

void FomoTradingStrategy::StopLive()
{
#if USE_MULTITHREADING
	m_isThreadTradeOngoing.store(false); // break while loop
#endif
	m_strategyRunStatus = StrategyRunStatus::STOP;
}

#if USE_MULTITHREADING
// Thread's main trading loop
void FomoTradingStrategy::TradingLoop()
{
	m_isThreadTradeOngoing.store(true);
	while (m_isThreadTradeOngoing.load())
	{
		std::unique_lock<std::mutex> lock(m_marketDataMutex);
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