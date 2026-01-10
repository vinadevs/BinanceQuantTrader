/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "MarketDataSubject.h"
#include "SynchronousMarketDataFeed.h"

namespace MarketData {
	class SynchronousMarketDataFeed;
	class HistoricalMarketDataFeedHandler : public MarketDataSubject
	{
	public:
		HistoricalMarketDataFeedHandler();

		// register data listeners
		void RegisterObserver(MarketDataObserver* observer);
		void UnregisterObserver(MarketDataObserver* observer);
		// market data feed access
		bool CreateNewMarketDataFeed(const std::string& symbol);
		void RemoveMarketDataFeed(const std::string& symbol);
		SynchronousMarketData* GetSynchronousMarketData(const std::string& symbol) override;

	private:
		std::unique_ptr<MarketDataFeedManager> m_synchronousFeedMgr;
	};
};