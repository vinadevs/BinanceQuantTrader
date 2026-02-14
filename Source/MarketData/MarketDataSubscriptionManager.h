/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "../RealTimeSocket/RealTimeSocket.h"

#include <memory>
#include <string>
#include <list>
#include <unordered_map>

namespace MarketData {
	using SubscriptionHandlePool 
		= std::unordered_map<std::string, binapi::ws::websockets::handle>;

	enum class SubscriptionHandleType : unsigned
	{
		INDIVIDUAL_BOOK_TICKER,
		TRADE,
		INDIVIDUAL_MARKET_TICKER,
		ALL_MARKET_TICKERS,
		INDIVIDUAL_MINI_TICKER,
		ALL_MINI_TICKERS,
		AGGREGATE_TRADE,
		KLINE_CANDLE_STICK,
		PART_DEPTH,
		DIFF_DEPTH,
		USER_DATA,
		FUTURE_TRADE,
		FUTURE_BOOK,
		END_NOT_USING  // Add this to define the range
	};

	class MarketDataSubscriptionManager
	{
	public:
		MarketDataSubscriptionManager();
		bool AddHandle(const std::string& symbol,
			binapi::ws::websockets::handle h,
			const SubscriptionHandleType type);
		void RemoveHandle(const std::string& symbol,
			const SubscriptionHandleType type);
		binapi::ws::websockets::handle GetHandle(
			const std::string& symbol,
			const SubscriptionHandleType type);
	private:
		SubscriptionHandlePool m_individualBookTickerPool;
		SubscriptionHandlePool m_tradePool;
		SubscriptionHandlePool m_individualMarketTickerPool;
		SubscriptionHandlePool m_allMarketTickersPool;
		SubscriptionHandlePool m_individualMiniTickerPool;
		SubscriptionHandlePool m_allMiniTickersPool;
		SubscriptionHandlePool m_aggregateTradePool;
		SubscriptionHandlePool m_klineCandleStickPool;
		SubscriptionHandlePool m_partDepthPool;
		SubscriptionHandlePool m_diffDepthPool;
		SubscriptionHandlePool m_userDataPool;
		SubscriptionHandlePool m_futureTradePool;
		SubscriptionHandlePool m_futureBookPool;
	};
};