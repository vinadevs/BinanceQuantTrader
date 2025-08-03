/********************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be useddistributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"

#include "../MarketData/SynchronousMarketData.h"

#include <string>
#include <unordered_map>
#include <memory>
#include <deque>
#include <mutex>

namespace LibraryUtils {
	class Logger;
}

namespace MarketData {
	class SynchronousMarketData;
}

namespace QuantitativeModel {

	enum class PriceTickerTrend : unsigned
	{
		UNDEF,
		UP_TREND, // price is going up
		DOWN_TREND, // price is going down
		INVERTED_TREND, // price is going down but we expect it to go up
		INVERTED_DOWN_TREND, // price is going up but we expect it to go down
		STABLE_TREND, // price is stable, no change
		NO_TREND // no trend detected
	};

	enum class PricePeriodTrend : unsigned
	{
		UNDEF,
		UP_PERIOD, // price is going up in a period
		DOWN_PERIOD, // price is going down in a period
		INVERTED_UP_PERIOD, // price is going down in a period but we expect it to go up
		INVERTED_DOWN_PERIOD, // price is going up in a period but we expect it to go down
		STABLE_PERIOD, // price is stable in a period
		NO_PERIOD // no period trend detected
	};

	struct MarketDataSignals final
	{
		std::deque<binapi::double_type> m_bestBidPrices;
		std::deque<binapi::double_type> m_bestAskPrices;
		std::deque<binapi::double_type> m_bestBidQty;
		std::deque<binapi::double_type> m_bestAskQty;
		size_t m_windowSize{ 20 };
		binapi::double_type m_smaPrice{ 0.0 };
		binapi::double_type m_lastBestBidPrice{ 0.0 };
		binapi::double_type m_lastBestAskPrice{ 0.0 };
		bool m_isInvertedTrend{ false };
		size_t m_triggerMaxiumUpTick{ 5 };
		size_t m_triggerMaxiumDownTick{ 5 };
		size_t m_triggerMaxiumUpPeriod{ 5 };
		size_t m_triggerMaxiumDownPeriod{ 5 };
		size_t m_tickbyTickUpCounter{ 10 };
		size_t m_tickbyTickDownCounter{ 10 };
		size_t m_periodUpCounter{ 10 };
		size_t m_periodDownCounter{ 10 };
		PriceTickerTrend m_priceTickerTrend{ PriceTickerTrend::UNDEF };
		PricePeriodTrend m_pricePeriodTrend{ PricePeriodTrend::UNDEF };
	};

	class DLL_CLASS_QUANTITATIVEMODEL_EXPORTS QuantMarketDataAnalyzer final {
	public:
		QuantMarketDataAnalyzer(const std::string& symbol);
		~QuantMarketDataAnalyzer();

		void AnalysisIndividualBookTicker(const MarketData::IndividualBookTickerData& data);
		void AnalysisTrade(const MarketData::TradeData& data);
		void AnalysisIndividualMarketTicker(const MarketData::IndividualMarketTickerData& data);
		void AnalysisMiniTicker(const MarketData::IndividualMiniTickerData& data);
		void AnalysisAggregateTrade(const MarketData::AggregateTradeData& data);
		void AnalysisKlineCandleStick(const MarketData::KlineCandleStickData& data);
		void AnalysisAllMarketTickers(const MarketData::AllMarketTickerData& data);
		void AnalysisAllMiniTickers(const MarketData::AllMiniTickerData& data);
		void AnalysisAllBookTickers(const MarketData::AllMarketTickerData& data);
		void AnalysisAllMarketDepth(const MarketData::AllPartDepthData& data);
		void AnalysisAllMarketDepthDiff(const MarketData::AllDiffDepthData& data);

		// not thread safe, must be called with a lock on m_mutex
		const MarketDataSignals& GetMarketDataSignals() const {
			return m_marketDataSignals;
		}

		std::mutex m_mutex; // to protect access to m_marketDataSignals

	private:
		std::string m_symbol;
		MarketDataSignals m_marketDataSignals;
		std::unique_ptr<LibraryUtils::Logger> m_logger;
	};
};
