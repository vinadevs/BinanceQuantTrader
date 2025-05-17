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

namespace LibraryUtils {
	class Logger;
}

namespace MarketData {
	class SynchronousMarketData;
}

namespace QuantitativeModel {

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
	private:
		std::string m_symbol;
		std::deque<binapi::double_type> m_bestBidPrices;
		std::deque<binapi::double_type> m_bestAskPrices;
		std::deque<binapi::double_type> m_bestBidQty;
		std::deque<binapi::double_type> m_bestAskQty;
		size_t m_windowSize{ 20 };
		binapi::double_type m_smaPrice{ 0.0 };
		binapi::double_type m_lastBestBidPrice{ 0.0 };
		bool m_isInvertedTrend{ false };
		size_t m_triggerMaxiumUpTick{ 5 };
		size_t m_triggerMaxiumDownTick{ 5 };
		size_t m_triggerMaxiumUpPeriod{ 5 };
		size_t m_triggerMaxiumDownPeriod{ 5 };
		size_t m_tickbyTickUpCounter{ 10 };
		size_t m_tickbyTickDownCounter{ 10 };
		size_t m_periodUpCounter{ 10 };
		size_t m_periodDownCounter{ 10 };
		std::unique_ptr<LibraryUtils::Logger> m_logger;
	};
};
