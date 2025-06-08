/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../OrderManagement/Order.h"
#include "../LibraryUtils/Logger.h"

#include "UpstreamOrderQueueMgr.h"

using namespace ExchangeSimulator;

UpstreamOrderQueueMgr::UpstreamOrderQueueMgr()
{
}

UpstreamOrderQueueMgr::~UpstreamOrderQueueMgr()
{
}

void UpstreamOrderQueueMgr::PushOrderToQueue(const std::string& clientOrderId, const UpstreamOrder& order)
{
	m_upstreamOrderQueue.EnqueueOrder(clientOrderId, order);
}

bool UpstreamOrderQueueMgr::RemoveOrder(const std::string& clientOrderId)
{
	return m_upstreamOrderQueue.RemoveOrderWithClientOrderId(clientOrderId);
}

bool UpstreamOrderQueueMgr::ReplaceOrder(const std::string& clientOrderId, const UpstreamOrder& order)
{
	return m_upstreamOrderQueue.ReplaceOrderWithClientOrderId(clientOrderId, order);
}

UpstreamOrder UpstreamOrderQueueMgr::LookupOrder(const std::string& clientOrderId) const
{
	auto result = m_upstreamOrderQueue.LookupOrderWithClientOrderId(clientOrderId);
	if (result)
	{
		return *result;
	}
	else
	{
		throw std::runtime_error("UpstreamOrderQueueMgr: could not found order with clientOrderId=" + clientOrderId);
	}
}

void UpstreamOrderQueueMgr::ClearAll()
{
	m_upstreamOrderQueue.Clear();
}

bool UpstreamOrderQueueMgr::HasNoOrders()
{
	return m_upstreamOrderQueue.IsEmpty();
}

UpstreamOrder UpstreamOrderQueueMgr::GetNextOrder()
{
	auto result = m_upstreamOrderQueue.DequeueOrder();
	if (result)
	{
		return *result;
	}
	else
	{
		throw std::runtime_error("UpstreamOrderQueueMgr: empty order queue");
	}
}
