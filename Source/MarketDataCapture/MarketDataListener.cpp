#include "../MarketData/MarketDataSubject.h"
#include "../MarketData/SynchronousMarketData.h"
#include "../LibraryUtils/Logger.h"
#include "../LibraryUtils/TimeUtils.h"

#include "MarketDataListener.h"

using namespace MarketDataCapture;
using namespace MarketData;

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
        LOG_INFO_STREAM(m_logger, "[Level2] Symbol=" << syncedData->GetSymbol()
            << "|" << IndividualBookTickerID::BEST_ASK_PRICE <<
            "=" << syncedData->GetFeed(IndividualBookTickerID::BEST_ASK_PRICE)->GetDoubleMultiprecisionData()
            << "|" << IndividualBookTickerID::BEST_ASK_QUANTITY <<
            "=" << syncedData->GetFeed(IndividualBookTickerID::BEST_ASK_QUANTITY)->GetDoubleMultiprecisionData()
            << "|" << IndividualBookTickerID::BEST_BID_PRICE <<
            "=" << syncedData->GetFeed(IndividualBookTickerID::BEST_BID_PRICE)->GetDoubleMultiprecisionData()
            << "|" << IndividualBookTickerID::BEST_BID_QUANTITY <<
            "=" << syncedData->GetFeed(IndividualBookTickerID::BEST_BID_QUANTITY)->GetDoubleMultiprecisionData());
        // Update more feeds here...
        // ...
        // Update time is always the last one.
        /*<< "|" << FromFeedIDToString(IndividualBookTickerID::EVENT_TIME) <<
           "=" << TimeUtils::GetTimestampString(syncedData->GetFeed(IndividualBookTickerID::EVENT_TIME)->GetUnsignedIntData()))*/;
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
        return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}
