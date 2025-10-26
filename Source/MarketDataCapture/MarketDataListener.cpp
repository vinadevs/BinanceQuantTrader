/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "../MarketData/MarketDataSubject.h"
#include "../MarketData/SynchronousMarketData.h"
#include "../LibraryUtils/Logger.h"
#include "../LibraryUtils/TimeUtils.h"

#include "MarketDataListener.h"

#include <type_traits>

using namespace MarketDataCapture;
using namespace MarketData;

MarketDataListener::MarketDataListener()
    : m_logger{ std::make_unique<LibraryUtils::Logger>("MarketDataListener") }
{
}

MarketDataListener::~MarketDataListener() {}

bool MarketDataListener::OnIndividualBookTickerChange(
	MarketDataSubject* marketData, const std::string& symbol)
{
    if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
        LOG_INFO_STREAM(m_logger, "[Level2] Symbol=" << syncedData->GetSymbol() << "|"
            << syncedData->m_individualBookTickerData);
        return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}

bool MarketDataListener::OnTradeChange(
	MarketDataSubject* marketData, const std::string& symbol)
{
    if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
        LOG_INFO_STREAM(m_logger, "[Level1] Symbol=" << syncedData->GetSymbol() << "|"
            << syncedData->m_tradeData);
        return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}

bool MarketDataListener::OnIndividualMarketTickerChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
        LOG_INFO_STREAM(m_logger, "[Level1] Symbol=" << syncedData->GetSymbol() << "|"
            << syncedData->m_individualMarketTickerData);
		return true;
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool MarketDataListener::OnMiniTickerChange(MarketDataSubject* marketData, const std::string& symbol)
{
    if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
        LOG_INFO_STREAM(m_logger, "[Level1] Symbol=" << syncedData->GetSymbol() << "|"
            << syncedData->m_individualMiniTickerData);
        return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}

bool MarketDataListener::OnAggregateTradeChange(MarketDataSubject* marketData, const std::string& symbol)
{
    if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
        LOG_INFO_STREAM(m_logger, "[Level1] Symbol=" << syncedData->GetSymbol() << "|"
            << syncedData->m_aggregateTradeData);
        return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}

bool MarketDataListener::OnKlineCandleStickChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		LOG_INFO_STREAM(m_logger, "[Level1] Symbol=" << syncedData->GetSymbol() << "|"
			<< syncedData->m_klineCandleStickData);
		return true;
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
    return false;
}

bool MarketDataListener::OnAllMarketTickersChange(MarketDataSubject* marketData, const std::string& symbol)
{
    if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
        LOG_INFO_STREAM(m_logger, "[Level2] Symbol=" << syncedData->GetSymbol() << "|"
            << syncedData->m_allMarketTickerData);
		return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}

bool MarketDataListener::OnAllMiniTickersChange(MarketDataSubject* marketData, const std::string& symbol)
{
    if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
		LOG_INFO_STREAM(m_logger, "[Level1] Symbol=" << syncedData->GetSymbol() << "|"
			<< syncedData->m_allMiniTickerData);
		return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}

bool MarketDataListener::OnAllDiffDepthChange(MarketDataSubject* marketData, const std::string& symbol)
{
    if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
		LOG_INFO_STREAM(m_logger, "[Level2] Symbol=" << syncedData->GetSymbol() << "|"
			<< syncedData->m_allDiffDepthData);
		return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}

bool MarketDataListener::OnAllPartDepthChange(MarketDataSubject* marketData, const std::string& symbol)
{
    if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
		LOG_INFO_STREAM(m_logger, "[Level2] Symbol=" << syncedData->GetSymbol() << "|"
			<< syncedData->m_allPartDepthData);
		return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}
