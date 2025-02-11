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

#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>

#include <fstream>
#include <filesystem>
#include <exception>

using namespace MarketData;
using namespace tinyxml2;
using namespace LibraryUtils;

//int test() {
//    boost::asio::io_context ioctx;
//
//    binapi::ws::websockets ws{
//         ioctx
//        ,"stream.binance.com"
//        ,"9443"
//    };
//
//    ws.klines("BTCUSDT", "1s",
//        [](const char *fl, int ec, std::string emsg, auto klines) {
//            if ( ec ) {
//                std::cerr << "subscribe klines error: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;
//
//                return false;
//            }
//
//            std::cout << "klines: " << klines << std::endl;
//
//            return true;
//        }
//    );
//
//    ws.part_depth("BTCUSDT", binapi::e_levels::_5, binapi::e_freq::_100ms,
//        [](const char *fl, int ec, std::string emsg, auto depths) {
//            if ( ec ) {
//                std::cerr << "subscribe part_depth error: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;
//
//                return false;
//            }
//
//            std::cout << "part_depths: " << depths << std::endl;
//
//            return true;
//          }
//    );
//
//    ws.diff_depth("BTCUSDT", binapi::e_freq::_100ms,
//        [](const char *fl, int ec, std::string emsg, auto depths) {
//            if ( ec ) {
//                std::cerr << "subscribe diff_depth error: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;
//
//                return false;
//            }
//
//            std::cout << "diff_depths: " << depths << std::endl;
//
//            return true;
//        }
//    );
//
//    ws.trade("BTCUSDT",
//        [](const char *fl, int ec, std::string emsg, auto trades) {
//            if ( ec ) {
//                std::cerr << "subscribe trades error: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;
//
//                return false;
//            }
//
//            std::cout << "trades: " << trades << std::endl;
//
//            return true;
//        }
//    );
//
//    auto book_handler = ws.book("BTCUSDT",
//        [](const char *fl, int ec, std::string emsg, auto book) {
//            if ( ec ) {
//                std::cerr << "subscribe book error: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;
//
//                return false;
//            }
//
//            std::cout << "book: " << book << std::endl;
//
//            return true;
//        }
//    );
//
//    ws.mini_tickers(
//        [](const char *fl, int ec, std::string emsg, auto mini_tickers) {
//            if ( ec ) {
//                std::cerr << "subscribe mini_tickers error: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;
//
//                return false;
//            }
//
//            std::cout << "mini_tickers: " << mini_tickers << std::endl;
//
//            return true;
//        }
//    );
//
//#if 1
//    boost::asio::steady_timer timer0{ioctx, std::chrono::steady_clock::now() + std::chrono::seconds(5)};
//    timer0.async_wait([&ws, book_handler](const auto &/*ec*/){
//        std::cout << "unsubscribing book_handler: " << book_handler << std::endl;
//        ws.unsubscribe(book_handler);
//    });
//#endif
//
//    boost::asio::steady_timer timer2{ioctx, std::chrono::steady_clock::now() + std::chrono::seconds(300)};
//    timer2.async_wait([&ws](const auto &/*ec*/){
//        std::cout << "async unsubscribing all" << std::endl;
//        ws.async_unsubscribe_all();
//    });
//
//    ioctx.run();
//
//    return EXIT_SUCCESS;
//}

MarketDataEvents::MarketDataEvents(
    const XMLElement* configXml,
    MarketDataFeedHandler* feedHandler)
    : m_feedHandler(feedHandler),
    m_mdSubscriptionMgr(std::make_unique<MarketDataSubscriptionManager>()),
    m_logger(std::make_unique<Logger>("MarketDataEvents")),
    m_configXml(configXml)
{
    assert(m_configXml);
    assert(m_feedHandler);
    const auto* connectionXml = m_configXml->FirstChildElement("Connection");
	assert(connectionXml);
    const auto streamBinanceCom = connectionXml->Attribute("StreamBinanceCom");
    const auto streamConnectionPort = connectionXml->Attribute("StreamConnectionPort");
    m_webSocketRealTime = std::make_unique<binapi::ws::websockets>(
          m_ioContext
        , streamBinanceCom
        , streamConnectionPort);
    m_logger->Info("Binance web socket created.");

    const auto* interestingDataSymbolsXml = m_configXml->FirstChildElement("InterestingDataSymbols");
    assert(interestingDataSymbolsXml);
    const auto* assetFile = interestingDataSymbolsXml->Attribute("File");
    assert(assetFile);
    m_logger->Info("Loading binance asset symbol data.");
    LoadInterestingDataSymbols(assetFile);
}

MarketDataEvents::~MarketDataEvents()
{
    m_logger->Info("Unsubscribe all symbols.");
    AsyncUnsubscribeAll();
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
                    m_subscribingSymbols.emplace(tradingPair);
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

void MarketDataEvents::StartSubscriptionEvents()
{
    m_logger->Info("Starting subscribing real time market data.");
    if (m_subscribingSymbols.empty())
    {
        throw std::runtime_error("MarketDataEvents: interesting symbol list is empty. No symbols to subscribe market data.");
    }
    for (const auto& symbol : m_subscribingSymbols)
    {
        if (m_feedHandler->CreateNewMarketDataFeed(symbol))
        {
            const auto* dataTypeSubscriptionXml = m_configXml->FirstChildElement("SubscriptionData");
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
        }
        else
        {
            m_logger->Warning("Could not create new market data feed for symbol=" + symbol);
        }
    }
}

void MarketDataEvents::Wait()
{
    // ansyn wait in main loop
    m_ioContext.run();
}

const std::unordered_set<std::string>& MarketDataEvents::GetSubscribingSymbols() const
{
    return m_subscribingSymbols;
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
