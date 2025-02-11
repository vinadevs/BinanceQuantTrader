/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "MarketDataSubscriptionManager.h"
#include "../LibraryUtils/Logger.h"

#include <stdexcept>

using namespace MarketData;
using namespace LibraryUtils;

MarketDataSubscriptionManager::MarketDataSubscriptionManager() {}

bool MarketDataSubscriptionManager::AddHandle(
    const std::string& symbol,
    binapi::ws::websockets::handle h,
    const SubscriptionHandleType type)
{
    switch (type)
    {
    case SubscriptionHandleType::INDIVIDUAL_BOOK_TICKER:
        return m_individualBookTickerPool.try_emplace(symbol, h).second;
    case SubscriptionHandleType::TRADE:
        return m_tradePool.try_emplace(symbol, h).second;
    case SubscriptionHandleType::INDIVIDUAL_MARKET_TICKER:
        return m_individualMarketTickerPool.try_emplace(symbol, h).second;
    case SubscriptionHandleType::ALL_MARKET_TICKERS:
        return m_allMarketTickersPool.try_emplace(symbol, h).second;
    case SubscriptionHandleType::INDIVIDUAL_MINI_TICKER:
        return m_individualMiniTickerPool.try_emplace(symbol, h).second;
    case SubscriptionHandleType::ALL_MINI_TICKERS:
        return m_allMiniTickersPool.try_emplace(symbol, h).second;
    case SubscriptionHandleType::AGGREGATE_TRADE:
        return m_aggregateTradePool.try_emplace(symbol, h).second;
    case SubscriptionHandleType::KLINE_CANDLE_STICK:
        return m_klineCandleStickPool.try_emplace(symbol, h).second;
    case SubscriptionHandleType::PART_DEPTH:
        return m_partDepthPool.try_emplace(symbol, h).second;
    case SubscriptionHandleType::DIFF_DEPTH:
        return m_diffDepthPool.try_emplace(symbol, h).second;
    case SubscriptionHandleType::USER_DATA:
        return m_userDataPool.try_emplace(symbol, h).second;
    default:
        throw std::runtime_error("MarketDataSubscriptionManager: unsupported SubscriptionHandleType.");
    }
    return false;
}

static void RemoveFromPool(
    std::unordered_map<std::string, binapi::ws::websockets::handle>& pool,
    const std::string& symbol)
{
    if (const auto it = pool.find(symbol); it != pool.end()) 
    {
        pool.erase(it);
    }
    else 
    {
        throw std::runtime_error("MarketDataSubscriptionManager: Symbol=" + symbol + " not found");
    }
}

void MarketDataSubscriptionManager::RemoveHandle(
    const std::string& symbol,
    const SubscriptionHandleType type)
{
    switch (type)
    {
    case SubscriptionHandleType::INDIVIDUAL_BOOK_TICKER:
        RemoveFromPool(m_individualBookTickerPool, symbol);
        break;
    case SubscriptionHandleType::TRADE:
        RemoveFromPool(m_individualBookTickerPool, symbol);
        break;
    case SubscriptionHandleType::INDIVIDUAL_MARKET_TICKER:
        RemoveFromPool(m_individualBookTickerPool, symbol);
        break;
    case SubscriptionHandleType::ALL_MARKET_TICKERS:
        RemoveFromPool(m_individualBookTickerPool, symbol);
        break;
    case SubscriptionHandleType::INDIVIDUAL_MINI_TICKER:
        RemoveFromPool(m_individualBookTickerPool, symbol);
        break;
    case SubscriptionHandleType::ALL_MINI_TICKERS:
        RemoveFromPool(m_individualBookTickerPool, symbol);
        break;
    case SubscriptionHandleType::AGGREGATE_TRADE:
        RemoveFromPool(m_individualBookTickerPool, symbol);
        break;
    case SubscriptionHandleType::KLINE_CANDLE_STICK:
        RemoveFromPool(m_individualBookTickerPool, symbol);
        break;
    case SubscriptionHandleType::PART_DEPTH:
        RemoveFromPool(m_individualBookTickerPool, symbol);
        break;
    case SubscriptionHandleType::DIFF_DEPTH:
        RemoveFromPool(m_individualBookTickerPool, symbol);
        break;
    case SubscriptionHandleType::USER_DATA:
        RemoveFromPool(m_individualBookTickerPool, symbol);
        break;
    default:
        throw std::runtime_error("MarketDataSubscriptionManager: unsupported SubscriptionHandleType.");
    }
}

static binapi::ws::websockets::handle LookupFromPool(
    std::unordered_map<std::string, binapi::ws::websockets::handle>& pool,
    const std::string& symbol)
{
    if (const auto it = pool.find(symbol); it != pool.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("MarketDataSubscriptionManager: Symbol=" + symbol + " not found");
    }
}

binapi::ws::websockets::handle MarketDataSubscriptionManager::GetHandle(
    const std::string& symbol,
    const SubscriptionHandleType type)
{
    switch (type)
    {
    case SubscriptionHandleType::INDIVIDUAL_BOOK_TICKER:
        return LookupFromPool(m_individualBookTickerPool, symbol);
        break;
    case SubscriptionHandleType::TRADE:
        return LookupFromPool(m_individualBookTickerPool, symbol);
        break;
    case SubscriptionHandleType::INDIVIDUAL_MARKET_TICKER:
        return LookupFromPool(m_individualBookTickerPool, symbol);
        break;
    case SubscriptionHandleType::ALL_MARKET_TICKERS:
        return LookupFromPool(m_individualBookTickerPool, symbol);
        break;
    case SubscriptionHandleType::INDIVIDUAL_MINI_TICKER:
        return LookupFromPool(m_individualBookTickerPool, symbol);
        break;
    case SubscriptionHandleType::ALL_MINI_TICKERS:
        return LookupFromPool(m_individualBookTickerPool, symbol);
        break;
    case SubscriptionHandleType::AGGREGATE_TRADE:
        return LookupFromPool(m_individualBookTickerPool, symbol);
        break;
    case SubscriptionHandleType::KLINE_CANDLE_STICK:
        return LookupFromPool(m_individualBookTickerPool, symbol);
        break;
    case SubscriptionHandleType::PART_DEPTH:
        return LookupFromPool(m_individualBookTickerPool, symbol);
        break;
    case SubscriptionHandleType::DIFF_DEPTH:
        return LookupFromPool(m_individualBookTickerPool, symbol);
        break;
    case SubscriptionHandleType::USER_DATA:
        return LookupFromPool(m_individualBookTickerPool, symbol);
        break;
    default:
        throw std::runtime_error("MarketDataSubscriptionManager: unsupported SubscriptionHandleType.");
    }
}
