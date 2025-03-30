/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "SynchronousMarketData.h"
#include "../KernelTrading/types.h"

#include <mutex>
#include <memory>
#include <string>
#include <unordered_map>

namespace MarketData {

// In trading, Synchronous Market Data Feed refers to a data stream 
// that provides real-time or near-real-time information from 
// a specific asset class which includes many single data feeds

	class SynchronousMarketDataFeed
	{
	public:
		SynchronousMarketDataFeed(const std::string& symbol);
		void UpdateIndividualBookTickerData(const binapi::ws::book_ticker_t& book);
		void UpdateTradeData(const binapi::ws::trade_t& trade);
		SynchronousMarketData* GetSynchronousData();

		template <typename FeedType>
		SingleMarketDataFeed* GetSingleFeed(const FeedType id) const
		{
			return m_syncMarketData->GetFeed(id);
		}
	private:
		std::unique_ptr<SynchronousMarketData> m_syncMarketData;
	};

	////////////////////////////////////////////////////////////////
	// This class will be used by mutiple threads so need to be thread safe...
	class MarketDataFeedManager final
	{
	public:
		MarketDataFeedManager() = default;
		// Thread safe methods
		bool CreateNewSynchronousFeed(const std::string& symbol);
		bool RemoveSynchronousFeed(const std::string& symbol);
		SynchronousMarketDataFeed* GetSynchronousFeed(const std::string& symbol);

		template <typename FeedType>
		SingleMarketDataFeed* GetSingleFeed(const std::string& symbol, const FeedType id)
		{
			std::unique_lock<std::mutex> lock(m_threadSafeMutex);
			if (const auto it = m_smdFeedStorage.find(symbol); it != m_smdFeedStorage.end())
			{
				return it->second.get()->GetSingleFeed(id);
			}
			return nullptr;
		}

	private:
		std::mutex m_threadSafeMutex;
		std::unordered_map<std::string, std::unique_ptr<SynchronousMarketDataFeed>> m_smdFeedStorage;
	};
};