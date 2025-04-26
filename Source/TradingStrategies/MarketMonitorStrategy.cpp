/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "MarketMonitorStrategy.h"
#include "../MarketData/RealTimeMarketData.h"
#include "../MarketData/SynchronousMarketData.h"
#include "../QuantitativeModel/MarketDataAnalyzer.h"
#include "../QuantitativeModel/QuantMarketDataAnalyzer.h"

using namespace TradingStrategies;
using namespace MarketData;
using namespace QuantitativeModel;

MarketMonitorStrategy::MarketMonitorStrategy(
	const std::string& strategyCfgPath,
	MarketData::RealTimeMarketData* marketData)
	: TradingStrategyBase("MarketMonitorStrategy",
		"analysis real time market data and generate trading signals...",
		strategyCfgPath, marketData)
{
	InitializeParameters(strategyCfgPath);
	//m_marketDataAnalyzer = std::make_unique<QuantitativeModel::MarketDataAnalyzer>(m_logger.get());
	marketData->RegisterDataListener(this); // I want receive market data to get fun!
}

MarketMonitorStrategy::~MarketMonitorStrategy()
{
	m_marketData->UnRegisterDataListener(this); // I earn enough money, leave the market now!
}

bool MarketMonitorStrategy::OnIndividualBookTickerChange(MarketDataSubject* marketData, const std::string& symbol)
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

bool MarketMonitorStrategy::OnTradeChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisTrade(syncedData->m_tradeData);
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool MarketMonitorStrategy::OnIndividualMarketTickerChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisIndividualMarketTicker(syncedData->m_individualMarketTickerData);
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool MarketMonitorStrategy::OnMiniTickerChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisMiniTicker(syncedData->m_individualMiniTickerData);
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool MarketMonitorStrategy::OnAggregateTradeChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisAggregateTrade(syncedData->m_aggregateTradeData);
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool MarketMonitorStrategy::OnKlineCandleStickChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisKlineCandleStick(syncedData->m_klineCandleStickData);
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool MarketMonitorStrategy::OnAllMarketTickersChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisAllMarketTickers(syncedData->m_allMarketTickerData);
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool MarketMonitorStrategy::OnAllMiniTickersChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisAllMiniTickers(syncedData->m_allMiniTickerData);
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool MarketMonitorStrategy::OnAllMarketDepthChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisAllMarketDepth(syncedData->m_allPartDepthData);
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool MarketMonitorStrategy::OnAllMarketDepthDiffChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisAllMarketDepthDiff(syncedData->m_allDiffDepthData);
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

void MarketMonitorStrategy::InitializeParameters(const std::string& strategyCfgPath)
{
}

void MarketMonitorStrategy::StartLive()
{
}

void MarketMonitorStrategy::StopLive()
{
}
