/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"
#include "../MarketData/SynchronousMarketData.h"
#include "../MarketData/MarketDataObserver.h"
#include "TradingStrategyBase.h"

#include <string>
#include <memory>

namespace MarketData {
	class RealTimeMarketData;
	class MarketDataSubject;
}

namespace QuantitativeModel {
	class MarketDataAnalyzer;
}

// The MarketMonitorStrategy class is a non-trading strategy designed to monitor market data in real-time.  
// It inherits from TradingStrategyBase and implements the MarketDataObserver interface.  
// This class provides various event handlers to process changes in market data, such as ticker updates,  
// trade changes, and candlestick updates. It is primarily used for market analysis and does not execute trades.

namespace TradingStrategies {
	class DLL_CLASS_TRADING_TRATEGIES_EXPORTS
		MarketMonitorStrategy : public TradingStrategyBase,
		public MarketData::MarketDataObserver
	{
	public:
		MarketMonitorStrategy(
			const std::string& strategyCfgPath,
			MarketData::RealTimeMarketData* marketData);

		virtual ~MarketMonitorStrategy();

		bool OnIndividualBookTickerChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		bool OnTradeChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		bool OnIndividualMarketTickerChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		bool OnMiniTickerChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		bool OnAggregateTradeChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		bool OnKlineCandleStickChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		bool OnAllMarketTickersChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		bool OnAllMiniTickersChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		bool OnAllMarketDepthChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		bool OnAllMarketDepthDiffChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;

		// This strategy is not for trading, so we don't need to implement these methods
		void ReportTradeResults(const std::string& symbol) override {}

		void InitializeParameters(const std::string& strategyCfgPath) override;

		void StartLive() override;

		void StopLive() override;
	private:
		std::unique_ptr<QuantitativeModel::MarketDataAnalyzer> m_marketDataAnalyzer;
	};
};