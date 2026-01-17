/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "../KernelTrading/types.h"

#include "MarketDataSubject.h"
#include "SynchronousMarketDataFeed.h"

// A feed handler for historical market data, this class is responsible for receiving,
// processing, and distributing historical market data from historical data sources

namespace MarketData {
	class SynchronousMarketDataFeed;
	class HistoricalMarketDataFeedHandler : public MarketDataSubject
	{
	public:
		HistoricalMarketDataFeedHandler();

		// market data feed access
		bool CreateNewMarketDataFeed(const std::string& symbol);
		void RemoveMarketDataFeed(const std::string& symbol);
		SynchronousMarketData* GetSynchronousMarketData(const std::string& symbol) override;

		void HandleDataItem(const std::string& symbol, const std::string& subscriptionType, const std::unordered_map<std::string, std::string>& dataItem);
	private:
		binapi::ws::book_ticker_t BuildIndividualBookTickerData(const std::string& symbol, const std::unordered_map<std::string, std::string>& dataItem);
		binapi::ws::trade_t BuildTradeData(const std::string& symbol, const std::unordered_map<std::string, std::string>& dataItem);
		binapi::ws::mini_ticker_t BuildIndividualMiniTickerData(const std::string& symbol, const std::unordered_map<std::string, std::string>& dataItem);
		binapi::ws::agg_trade_t BuildAggregateTradeData(const std::string& symbol, const std::unordered_map<std::string, std::string>& dataItem);

		std::unique_ptr<MarketDataFeedManager> m_synchronousFeedMgr;
	};
};