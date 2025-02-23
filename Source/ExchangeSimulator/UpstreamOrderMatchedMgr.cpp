/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "UpstreamOrderMatchedMgr.h"

using namespace ExchangeSimulator;

UpstreamOrderMatchedMgr::UpstreamOrderMatchedMgr()
{
}

UpstreamOrderMatchedMgr::~UpstreamOrderMatchedMgr()
{
}

void UpstreamOrderMatchedMgr::AddOrder(const std::string clientOrderId, const UpstreamOrder& order)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_matchedOrders.try_emplace(clientOrderId, order);
}

void UpstreamOrderMatchedMgr::RemoveOrder(const std::string& clientOrderId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_matchedOrders.find(clientOrderId);
    if (it != m_matchedOrders.end())
    {
        m_matchedOrders.erase(it);
    }
    else
    {
        throw std::runtime_error("No UpstreamOrder found with symbol='" + clientOrderId + "'.");
    }
}

const UpstreamOrder& UpstreamOrderMatchedMgr::LookupOrder(const std::string& clientOrderId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_matchedOrders.find(clientOrderId);
    if (it != m_matchedOrders.end())
    {
        return it->second;
    }
    throw std::runtime_error("No UpstreamOrder found with symbol='" + clientOrderId + "'.");
}

void UpstreamOrderMatchedMgr::ClearAll()
{
    m_matchedOrders.clear();
}

const MatchedOrders& UpstreamOrderMatchedMgr::GetMatchedOrders()
{
    return m_matchedOrders;
}
