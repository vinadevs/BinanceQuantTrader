/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include <memory>
#include <vector>
#include <string>
#include <unordered_set>
#include <mutex>
#include <condition_variable>

#include "MarketDataEventBase.h"

namespace tinyxml2 {
	class XMLElement;
};

namespace LibraryUtils {
	class Logger;
};

namespace MarketData {

	class HistoricalMarketDataFeedHandler;

	class HistoricalMarketDataEvents : public MarketDataEventBase
	{
	public:
		HistoricalMarketDataEvents(
			const tinyxml2::XMLElement* marketDataConfigXml,
			HistoricalMarketDataFeedHandler* feedHandler);
		~HistoricalMarketDataEvents();

		bool Subscribe(const std::string& symbol) override;
		bool Unsubscribe(const std::string& symbol) override;
		bool IsSubscribed(const std::string& symbol) override;
	private:
		HistoricalMarketDataFeedHandler* m_feedHandler{ nullptr };
	};
};