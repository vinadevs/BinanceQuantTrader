/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "UpstreamOrderQueue.h"

#include <memory>
#include <string>

#include <iostream>
namespace LibraryUtils {
	class Logger;
};

namespace ExchangeSimulator {

	class UpstreamOrderQueueMgr final
	{
	public:
		UpstreamOrderQueueMgr();
		~UpstreamOrderQueueMgr();

		void PushOrderToQueue(const std::string& clientOrderId, const UpstreamOrder& order);
		bool RemoveOrder(const std::string& clientOrderId);
		bool ReplaceOrder(const std::string& clientOrderId, const UpstreamOrder& order);
		const UpstreamOrder& LookupOrder(const std::string& clientOrderId) const;
		void ClearAll();
		bool HasNoOrders();
		UpstreamOrder GetNextOrder();
	private:
		std::unique_ptr<LibraryUtils::Logger> m_logger;
		// TODO: replace by LockFreeQueue.h
		UpstreamOrderQueue m_upstreamOrderQueue;
	};
};