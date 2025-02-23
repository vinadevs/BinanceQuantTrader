/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <mutex>

#include "UpstreamOrder.h"

namespace LibraryUtils {
	class Logger;
};

namespace ExchangeSimulator {

	using MatchedOrders = std::unordered_map<std::string, UpstreamOrder>;

	class UpstreamOrderMatchedMgr final
	{
	public:
		UpstreamOrderMatchedMgr();
		~UpstreamOrderMatchedMgr();

		void AddOrder(const std::string clientOrderId, const UpstreamOrder& order);
		void RemoveOrder(const std::string& clientOrderId);
		const UpstreamOrder& LookupOrder(const std::string& clientOrderId);
		void ClearAll();
		const MatchedOrders& GetMatchedOrders();
	private:
		std::mutex m_mutex;
		MatchedOrders m_matchedOrders;
	};
};