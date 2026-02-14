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
	class SynchronousMarketDataFeed final
	{
	public:
		SynchronousMarketDataFeed(const std::string& symbol);
		void UpdateIndividualBookTickerData(const binapi::ws::book_ticker_t& book);
		void UpdateTradeData(const binapi::ws::trade_t& trade);
		void UpdateIndividualMarketTickerData(const binapi::ws::market_ticker_t& market);
		void UpdateMiniTickerData(const binapi::ws::mini_ticker_t& mini);
		void UpdateAggregateTradeData(const binapi::ws::agg_trade_t& aggregate);
		void UpdateKlineCandleStickData(const binapi::ws::kline_t& kline);
		void UpdateAllPartDepthData(const binapi::ws::part_depths_t& depth);
		void UpdateAllDiffDepthData(const binapi::ws::diff_depths_t& depth);
		void UpdateUserDataAccountUpdate(const binapi::userdata::account_update_t& accountUpdate);
		void UpdateUserDataBalanceUpdate(const binapi::userdata::balance_update_t& balanceUpdate);
		void UpdateUserDataOrderUpdate(const binapi::userdata::order_update_t& orderUpdate);
		void UpdateFutureTradeData(const binapi::ws::future_trade_t& trade);
		void UpdateFutureBookData(const binapi::ws::future_book_t& book);

		SynchronousMarketData* GetSynchronousData();

		template <typename FeedType>
		SingleMarketDataFeed* GetSingleFeed(const FeedType id) const
		{
			return m_syncMarketData->GetSingleFeed(id);
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

		// Get single feed by symbol and field id
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