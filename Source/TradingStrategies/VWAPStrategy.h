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
#include <vector>

namespace UserAccount {
	class Trader;
}

namespace ComplianceNRegulatory {
	class BinanceTradingRules;
}

namespace MarketData {
	class RealTimeMarketData;
	class MarketDataSubject;
}

namespace QuantitativeModel {
	class MarketDataAnalyzer;
}

/**
 * @class VWAPStrategy
 * @brief Implements a Volume Weighted Average Price (VWAP) trading strategy.
 *
 * This class calculates the VWAP based on incoming market data (trade prices and volumes)
 * and generates order execution signals to match or beat the VWAP over a specified trading period.
 *
 * The VWAP is computed as the cumulative sum of (price * volume) divided by the total volume traded,
 * providing a benchmark price for the trading day or session.
 *
 * Key responsibilities:
 *  - Accumulate trade data and compute VWAP in real-time.
 *  - Determine order slicing and execution pace based on volume participation targets.
 *  - Adjust execution strategy dynamically to minimize market impact and tracking error.
 *  - Optionally enforce risk limits such as maximum participation rate and price deviation thresholds.
 */

namespace TradingStrategies {

	class VWAPVolumeProfile;

	class DLL_CLASS_TRADING_TRATEGIES_EXPORTS
		VWAPStrategy :
		public TradingStrategyBase, // strategy core
		public MarketData::MarketDataObserver, // market data update
		public LibraryUtils::AlarmSystem // schedule trading
	{
	public:
		VWAPStrategy(const std::string& strategyCfgPath,
			MarketData::RealTimeMarketData* marketData,
			UserAccount::Trader* trader,
			ComplianceNRegulatory::BinanceTradingRules* tradingRules);

		virtual ~VWAPStrategy();

		bool OnIndividualBookTickerChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;

		void ReportTradeResults(const std::string& symbol) override;

		void InitializeParameters(const std::string& strategyCfgPath) override;

		void StartLive() override;

		void StopLive() override;

		// - This function is called when the alarm is triggered, which is used to send orders
		void OnAlarmTriggered(const int passToDerived = 0) override;
	private:
		void InitializeMarketDataAnalyzer();
		void SetupOrderScheduler();
		void SetupVWAPVolumeProfile();
		void CreateBinanceExchangeProfile();
		void CreatePortfolioManagement();
		void PrepareTargetMonitorSymbols();
		void SubscribeTargetSymbols();
		void UnsubscribeTargetSymbols();
		// List of symbols that we will trade in future market
		std::vector<std::string> m_targetFutureTradeSymbols;
		std::unique_ptr<QuantitativeModel::MarketDataAnalyzer> m_marketDataAnalyzer;
		std::unique_ptr<VWAPVolumeProfile> m_vwapVolumeProfilier; // VWAP volume profile calculator
		std::unordered_map<size_t, double> m_executedVolume;
		double m_targetVWAPAmount{ 0.0 };
		double m_cumPriceVolume{ 0.0 };
		double m_totalMarketVolume{ 0.0 };
		long m_profileBucketSeconds{ 0 };
	};
};