/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "MarketDataFeedHandler.h"

#include <iostream>

using namespace MarketData;

MarketDataFeedHandler::MarketDataFeedHandler()
    : m_synchronousFeedMgr{ std::make_unique<MarketDataFeedManager>() } {}

void MarketDataFeedHandler::RegisterObserver(MarketDataObserver* observer)
{
    MarketDataSubject::AttachMarketDataObserver(observer);
}

void MarketDataFeedHandler::UnregisterObserver(MarketDataObserver* observer)
{
    MarketDataSubject::DettachMarketDataObserver(observer);
}

bool MarketDataFeedHandler::CreateNewMarketDataFeed(const std::string& symbol)
{
    return m_synchronousFeedMgr->CreateNewFeed(symbol);
}

void MarketDataFeedHandler::RemoveMarketDataFeed(const std::string& symbol)
{
    m_synchronousFeedMgr->RemoveFeed(symbol);
}

bool MarketDataFeedHandler::HandleIndividualBookTickerData(const char* fl,
    int ec, std::string emsg, binapi::ws::book_ticker_t book)
{
    if (ec) 
    {
        std::cerr << "subscribe book error: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;
        return false;
    }
    if (auto feed = m_synchronousFeedMgr->GetSynchronousFeed(book.s))
    {
        feed->UpdateIndividualBookTickerData(book);
        MarketDataSubject::NotifyIndividualBookTickerChange(book.s);
        return true;
    }
    else
    {
        throw std::runtime_error("MarketDataFeedHandler: sycnchronous feed does not exit with symbol=" + book.s);
    }
    return false;
}

bool MarketDataFeedHandler::HandleTradeData(const char* fl, int ec, std::string emsg, binapi::ws::trade_t trade)
{
    if (ec)
    {
        std::cerr << "subscribe trade error: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;
        return false;
    }
    if (auto feed = m_synchronousFeedMgr->GetSynchronousFeed(trade.s))
    {
        feed->UpdateTradeData(trade);
        MarketDataSubject::NotifyTradeChange(trade.s);
        return true;
    }
    else
    {
        throw std::runtime_error("MarketDataFeedHandler: sycnchronous feed does not exit with symbol=" + trade.s);
    }
    return false;
}

SynchronousMarketData* MarketDataFeedHandler::GetSynchronousMarketData(const std::string& symbol)
{
    if (const auto feed = m_synchronousFeedMgr->GetSynchronousFeed(symbol))
    {
        return feed->GetSynchronousData();
    }
    return nullptr;
}
