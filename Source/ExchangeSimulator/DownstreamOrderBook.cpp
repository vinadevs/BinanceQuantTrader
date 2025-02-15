/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../LibraryUtils/Logger.h"

#include "DownstreamOrderBook.h"

using namespace ExchangeSimulator;

DownstreamOrderBook::DownstreamOrderBook()
	: m_logger{ std::make_unique<LibraryUtils::Logger>("DownstreamOrderBook") }
{
}

DownstreamOrderBook::~DownstreamOrderBook()
{
}

void DownstreamOrderBook::CreateNewOrderBook(const std::string& symbol)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    const auto result = m_orderBooks.try_emplace(symbol, std::make_unique<OrderBook>());
    if (!result.second)
    {
        LOG_WARNING_STREAM(m_logger, "OrderBook with symbol '" << symbol << "' already exists.");
    }
}

void DownstreamOrderBook::RemoveOrderBook(const std::string& symbol)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_orderBooks.find(symbol);
    if (it != m_orderBooks.end())
    {
        m_orderBooks.erase(it);
        LOG_INFO_STREAM(m_logger, "OrderBook with symbol '" << symbol << "' removed successfully.");
    }
    else
    {
        throw std::runtime_error("No OrderBook found with symbol '" + symbol + "'.");
    }
}

OrderBook* DownstreamOrderBook::LookupOrderBook(const std::string& symbol)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_orderBooks.find(symbol);
    if (it != m_orderBooks.end())
    {
        return it->second.get();
    }
    throw std::runtime_error("No OrderBook found with symbol '" + symbol + "'.");
}

void DownstreamOrderBook::ClearAll()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_orderBooks.clear();
}

const OrderBooks& DownstreamOrderBook::GetOrderBooks()
{
    return m_orderBooks;
}
