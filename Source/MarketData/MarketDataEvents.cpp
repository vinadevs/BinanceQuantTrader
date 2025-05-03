/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "MarketDataEvents.h"
#include "MarketDataFeedHandler.h"

#include "../SettingNConfig/tinyxml2.h"

#include "../LibraryUtils/Logger.h"
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

MarketDataEvents::MarketDataEvents(
    const XMLElement* marketDataConfigXml,
    MarketDataFeedHandler* feedHandler)
    : m_marketDataConfigXml(marketDataConfigXml),
    m_feedHandler(feedHandler),
    m_mdSubscriptionMgr(std::make_unique<MarketDataSubscriptionManager>()),
    m_logger(std::make_unique<Logger>("MarketDataEvents"))
{
    assert(m_marketDataConfigXml);
    CreateWebSocketConnection();
}

MarketDataEvents::~MarketDataEvents()
{
    m_logger->Info("Shutdown update events and unsubscribe all symbols.");
    AsyncUnsubscribeAll();
}

void MarketDataEvents::CreateWebSocketConnection()
{
    const auto* connectionXml = m_marketDataConfigXml->FirstChildElement("Connection");
    assert(connectionXml);
    const auto streamBinanceCom = connectionXml->Attribute("StreamBinanceCom");
    const auto streamConnectionPort = connectionXml->Attribute("StreamConnectionPort");
    m_webSocketRealTime = std::make_unique<binapi::ws::websockets>(
        m_ioContext
        , streamBinanceCom
        , streamConnectionPort);
    m_logger->Info("Binance web socket created.");
}

void MarketDataEvents::LoadInterestingDataSymbols(const char* filePath)
{
    std::string assetFilePath(filePath);
    PathUtils::ReplaceSubString(assetFilePath, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());
    if (std::filesystem::exists(assetFilePath))
    {
        std::ifstream file(assetFilePath);
        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                if (line.empty() || line[0] == '#') // ignore empty/comment line
                {
                    continue;
                }
                std::istringstream ss(line);
                std::string tradingPair;
                if (std::getline(ss, tradingPair))
                {
                    StringUtils::StrimString(tradingPair);
                    m_staticSymbols.emplace(tradingPair);
                }
                else
                {
                    m_logger->Warning("Invalid line, reading a symbol from file failed.");
                }
            }
            file.close();
        }
        else
        {
            throw std::runtime_error("MarketDataEvents: error opening file=" + assetFilePath);
        }
    }
    else
    {
        throw std::runtime_error("MarketDataEvents: file does not exist=" + assetFilePath);
    }
}

void MarketDataEvents::StartIOContext()
{
    // Set the atomic flag to true, indicating that the IO context should start.
    m_startIOContext.store(true);
    // Notify MarketDataEvents::Wait() that the condition variable has been triggered.
    m_marketDataCond.notify_one();
}

bool MarketDataEvents::Subscribe(const std::string& symbol)
{
    std::lock_guard<std::mutex> lock(m_marketDataMutex);
    m_logger->Info("Starting subscribing real time market data for symbol=" + symbol);
    if (m_feedHandler->CreateNewMarketDataFeed(symbol))
    {
        const auto* dataTypeSubscriptionXml = m_marketDataConfigXml->FirstChildElement("SubscriptionData");
        assert(dataTypeSubscriptionXml);
        if (StringUtils::IsConfigAttributeMatched(dataTypeSubscriptionXml->Attribute("IndividualBookTickerData"), "true"))
        {
            SubscibeIndividualBookTicker(symbol);
        }
        if (StringUtils::IsConfigAttributeMatched(dataTypeSubscriptionXml->Attribute("TradeData"), "true"))
        {
            SubscibeTrade(symbol);
        }
        if (StringUtils::IsConfigAttributeMatched(dataTypeSubscriptionXml->Attribute("IndividualMarketTickerData"), "true"))
        {
            SubscibeIndividualMarketTicker(symbol);
        }
        if (StringUtils::IsConfigAttributeMatched(dataTypeSubscriptionXml->Attribute("AllMarketTickersData"), "true"))
        {
            SubscibeAllMarketTickers(symbol);
        }
        if (StringUtils::IsConfigAttributeMatched(dataTypeSubscriptionXml->Attribute("IndividualMiniTickerData"), "true"))
        {
            SubscibeIndividualMiniTicker(symbol);
        }
        if (StringUtils::IsConfigAttributeMatched(dataTypeSubscriptionXml->Attribute("AllMiniTickersData"), "true"))
        {
            SubscibeAllMiniTickers(symbol);
        }
        if (StringUtils::IsConfigAttributeMatched(dataTypeSubscriptionXml->Attribute("AggregateTradeData"), "true"))
        {
            SubscibeAggregateTrade(symbol);
        }
        if (StringUtils::IsConfigAttributeMatched(dataTypeSubscriptionXml->Attribute("KlineCandleStickData"), "true"))
        {
            SubscibeKlineCandleStick(symbol);
        }
        if (StringUtils::IsConfigAttributeMatched(dataTypeSubscriptionXml->Attribute("PartDepthData"), "true"))
        {
            SubscibePartDepth(symbol);
        }
        if (StringUtils::IsConfigAttributeMatched(dataTypeSubscriptionXml->Attribute("DiffDepthData"), "true"))
        {
            SubscibeDiffDepth(symbol);
        }
        // stores subscribed symbol
        m_subscribedSymbols.emplace(symbol);
        return true;
    }
    else
    {
        m_logger->Warning("Could not create new market data feed for symbol=" + symbol);
        return false;
    }
}

bool MarketDataEvents::Unsubscribe(const std::string& symbol)
{
    std::lock_guard<std::mutex> lock(m_marketDataMutex);
    FOR_LOOP_ENUM(iter, SubscriptionHandleType)
    {
        auto h = m_mdSubscriptionMgr->GetHandle(symbol, iter);
        if (h) 
        {
            Unsubscribe(h);
        }
        else
        {
            return false;
        }
    }  
    return true;
}

bool MarketDataEvents::IsSubscribed(const std::string& symbol)
{
    std::lock_guard<std::mutex> lock(m_marketDataMutex);
    return m_subscribedSymbols.find(symbol) != m_subscribedSymbols.end();
}

void MarketDataEvents::Wait()
{
	// Wait for the condition variable to be notified
    std::unique_lock<std::mutex> lock(m_marketDataMutex);
	m_marketDataCond.wait(lock, [&]()
	{
		return m_startIOContext.load();
	});
    // If there is any bloclking call at our side then Binance side will disconnect websocket 
    // connection with the error: ec=10053, emsg=An established connection was aborted 
    // by the software in your host machine, so please carefully to use lock stuffs within this class
    m_ioContext.run(); // never return!!!
	assert(false); // alert if we reach here, maybe missing call StartIOContext()
}

const std::unordered_set<std::string>& MarketDataEvents::GetSubscribingSymbols() const
{
    return m_subscribedSymbols;
}

void MarketDataEvents::VerifySubscriptionHandle(
    const std::string& symbol,
    const std::string& dataName,
    binapi::ws::websockets::handle h,
    const SubscriptionHandleType type)
{
    if (h)
    {
        if (m_mdSubscriptionMgr->AddHandle(symbol, h, type))
        {
            m_logger->Info("Subscibed " + dataName + " for symbol=" + symbol);
        }
    }
    else
    {
        m_logger->Warning("Could not create new " + dataName + " handle for symbol=" + symbol);
    }
}

void MarketDataEvents::SubscibeIndividualBookTicker(const std::string& symbol)
{
    const auto handle = m_webSocketRealTime->book(symbol.c_str(),
        std::bind(&MarketDataFeedHandler::HandleIndividualBookTickerData,
            m_feedHandler,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3,
            std::placeholders::_4));
    VerifySubscriptionHandle(symbol, "individual book ticker", handle,
        SubscriptionHandleType::INDIVIDUAL_BOOK_TICKER);
}

void MarketDataEvents::SubscibeTrade(const std::string& symbol)
{
    const auto handle = m_webSocketRealTime->trade(symbol.c_str(),
        std::bind(&MarketDataFeedHandler::HandleTradeData,
            m_feedHandler,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3,
            std::placeholders::_4));
    VerifySubscriptionHandle(symbol, "trade", handle,
        SubscriptionHandleType::TRADE);
}

void MarketDataEvents::SubscibeIndividualMarketTicker(const std::string& symbol)
{
}

void MarketDataEvents::SubscibeAllMarketTickers(const std::string& symbol)
{
}

void MarketDataEvents::SubscibeIndividualMiniTicker(const std::string& symbol)
{
}

void MarketDataEvents::SubscibeAllMiniTickers(const std::string& symbol)
{
}

void MarketDataEvents::SubscibeAggregateTrade(const std::string& symbol)
{
}

void MarketDataEvents::SubscibeKlineCandleStick(const std::string& symbol)
{
}

void MarketDataEvents::SubscibePartDepth(const std::string& symbol)
{
}

void MarketDataEvents::SubscibeDiffDepth(const std::string& symbol)
{
}

void MarketDataEvents::SubscibeUserData(const std::string& symbol)
{
}

void MarketDataEvents::Unsubscribe(const binapi::ws::websockets::handle& h)
{
    m_webSocketRealTime->unsubscribe(h);
}

void MarketDataEvents::AsyncUnsubscribe(const binapi::ws::websockets::handle& h)
{
    m_webSocketRealTime->async_unsubscribe(h);
}

void MarketDataEvents::UnsubscribeAll()
{
    m_webSocketRealTime->unsubscribe_all();
}

void MarketDataEvents::AsyncUnsubscribeAll()
{
    m_webSocketRealTime->async_unsubscribe_all();
}
