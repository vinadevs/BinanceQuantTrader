#include "../MarketData/MarketDataSubject.h"
#include "../MarketData/SynchronousMarketData.h"
#include "../LibraryUtils/Logger.h"
#include "../LibraryUtils/TimeUtils.h"

#include "MarketDataListener.h"

#include <type_traits>

using namespace MarketDataCapture;
using namespace MarketData;

inline constexpr std::string_view indentation = "    "; // 4 spaces

MarketDataListener::MarketDataListener()
    : m_logger{ std::make_unique<LibraryUtils::Logger>("MarketDataListener") }
{
}

MarketDataListener::~MarketDataListener()
{
}

bool MarketDataListener::OnIndividualBookTickerChange(
	MarketData::MarketDataSubject* marketData, const std::string& symbol)
{
    if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
        LOG_INFO_STREAM(m_logger, "[Level2] Symbol=" << syncedData->GetSymbol() << " | "
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
	MarketData::MarketDataSubject* marketData, const std::string& symbol)
{
    if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
        LOG_INFO_STREAM(m_logger, "[Level2] Symbol=" << syncedData->GetSymbol() << " | "
            << syncedData->m_tradeData);

        return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}

bool MarketDataListener::OnIndividualMarketTickerChange(MarketData::MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
        LOG_INFO_STREAM(m_logger, "[Level2] Symbol=" << syncedData->GetSymbol() << " | "
            << syncedData->m_individualMarketTickerData);
		return true;
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}

	return false;
}

bool MarketDataListener::OnMiniTickerChange(MarketData::MarketDataSubject* marketData, const std::string& symbol)
{
    if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
        LOG_INFO_STREAM(m_logger, "[Level2] Symbol=" << syncedData->GetSymbol() << " | "
            << syncedData->m_individualMiniTickerData);

        return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }

    return false;
}

bool MarketDataListener::OnAggregateTradeChange(MarketData::MarketDataSubject* marketData, const std::string& symbol)
{
    if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
        LOG_INFO_STREAM(m_logger, "[Level2] Symbol=" << syncedData->GetSymbol() << " | "
            << syncedData->m_aggregateTradeData);
        return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }

    return false;
}

bool MarketDataCapture::MarketDataListener::OnKlineCandleStickChange(MarketData::MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		LOG_INFO_STREAM(m_logger, "[Level2] Symbol=" << syncedData->GetSymbol() << " | "
			<< syncedData->m_klineCandleStickData);
		return true;
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
    return false;
}

bool MarketDataListener::OnAllMarketTickersChange(MarketData::MarketDataSubject* marketData, const std::string& symbol)
{
    if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
        return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }

    return false;
}

bool MarketDataListener::OnAllMiniTickersChange(MarketData::MarketDataSubject* marketData, const std::string& symbol)
{
    if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {

        return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }

    return false;
}

bool MarketDataListener::OnAllMarketDepthChange(MarketData::MarketDataSubject* marketData, const std::string& symbol)
{
    if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {

        return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }

    return false;
}

bool MarketDataListener::OnAllMarketDepthDiffChange(MarketData::MarketDataSubject* marketData, const std::string& symbol)
{
    if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
        return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }

    return false;
}
