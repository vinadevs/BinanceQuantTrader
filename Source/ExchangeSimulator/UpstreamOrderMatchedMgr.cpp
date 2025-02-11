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
}

void UpstreamOrderMatchedMgr::RemoveOrder(const std::string& clientOrderId)
{
}

const UpstreamOrder& UpstreamOrderMatchedMgr::LookupOrder(const std::string& clientOrderId) const
{
    return m_matchedOrders.begin()->second;
}

void UpstreamOrderMatchedMgr::ClearAll()
{
}
