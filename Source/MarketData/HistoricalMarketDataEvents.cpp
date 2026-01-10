/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "HistoricalMarketDataEvents.h"
#include "HistoricalMarketDataFeedHandler.h"

#include "../SettingNConfig/tinyxml2.h"

#include "../LibraryUtils/StringUtils.h"
#include "../LibraryUtils/PathUtils.h"
#include "../LibraryUtils/EnumIteration.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>

#include <fstream>
#include <filesystem>
#include <exception>

using namespace MarketData;
using namespace tinyxml2;
using namespace LibraryUtils;

HistoricalMarketDataEvents::HistoricalMarketDataEvents(
    const XMLElement* marketDataConfigXml,
    HistoricalMarketDataFeedHandler* feedHandler)
    : MarketDataEventBase(marketDataConfigXml),
    m_feedHandler(feedHandler)
{
    assert(m_marketDataConfigXml);
    m_logger = std::make_unique<Logger>("HistoricalMarketDataEvents");
}

HistoricalMarketDataEvents::~HistoricalMarketDataEvents()
{
    m_logger->Info("Shutdown update events and unsubscribe all symbols.");
}

bool HistoricalMarketDataEvents::Subscribe(const std::string& symbol)
{
    return false;
}

bool HistoricalMarketDataEvents::Unsubscribe(const std::string& symbol)
{
    return false;
}

bool HistoricalMarketDataEvents::IsSubscribed(const std::string& symbol)
{
    return false;
}
