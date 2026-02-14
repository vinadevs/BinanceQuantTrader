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
#include <mutex>

#include "MarketDataEventBase.h"

namespace tinyxml2 {
	class XMLElement;
};

namespace LibraryUtils {
	class Logger;
};

namespace HistoricalData {
	class MarketDataFileReader;
}

/**
 * @class HistoricalMarketDataEvents
 *
 * @brief
 * Replays historical market data events for backtesting and simulation.
 *
 * @details
 * This class is responsible for sequentially replaying historical market data
 * (ticks, trades, candles, etc.) as if they were arriving in real time.
 * It supports multiple replay modes, including:
 *
 *  - Event-time replay using the original exchange timestamps
 *  - Fixed-interval seconds (tick-by-tick) replay
 *
 * The replay behavior is fully deterministic and driven by configuration,
 * making it suitable for backtesting, strategy validation, and simulation.
 *
 * The replay can be configured to run once or continuously repeat the
 * historical data stream, depending on the configured repeat flag.
 *
 * Only subscribed symbols and data types are dispatched to the feed handler.
 * Each data item is validated before being forwarded.
 *
 * Typical usage scenarios:
 *  - Strategy backtesting
 *  - Market data simulation
 *  - Regression testing of trading logic
 *
 * @note
 * This class does not generate or modify market data.
 * It strictly replays existing historical data in order.
 */

namespace MarketData {
	class MarketDataSubject;
	class HistoricalMarketDataFeedHandler;
	class HistoricalMarketDataEvents : public MarketDataEventBase
	{
	public:
		HistoricalMarketDataEvents(
			const tinyxml2::XMLElement* marketDataConfigXml,
			MarketDataSubject* feedHandler);
		~HistoricalMarketDataEvents();

		enum class LoadEvent : unsigned
		{
			TickByTick = 0, // Fixed-interval seconds (tick-by-tick) replay
			LastUpdateTime, // Event-time replay using the original exchange timestamps
		};

		bool Subscribe(const std::string& symbol) override;
		bool Unsubscribe(const std::string& symbol) override;
		bool IsSubscribed(const std::string& symbol) override;

		void Wait() override;

	private:
		HistoricalMarketDataFeedHandler* m_feedHandler{ nullptr };
		HistoricalData::MarketDataFileReader* m_historicalDataFileReader{ nullptr };
		std::vector<std::string> m_subscriptionTypes;
		LoadEvent m_loadEvent{ LoadEvent::TickByTick };
		int m_intervalSeconds{ 0 };
		bool m_AllowRepeat{ false };
		std::mutex m_eventMutex;
	};
};