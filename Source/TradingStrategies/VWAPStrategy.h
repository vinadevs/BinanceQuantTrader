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
#include "VWAPParentOrder.h"

#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <mutex>

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

namespace OrderManagement {
	// Internal orders
	class BinanceNewOrder;
	class BinanceCancelOrder;
	class BinanceReplaceOrder;
	class BinanceQueryOrder;
	// External parent orders
	class NewExternalParentOrder;
	class CancelExternalParentOrder;
	class AmendExternalParentOrder;
}

namespace QuantitativeModel {
	class OrderParammeterGenerator;
}

/**
 * @class VWAPStrategy
 * @brief Implements a Volume Weighted Average Price (VWAP) trading strategy.
 * It will receive external parent orders and execute them as VWAP child orders.
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

		// order book market data from exchange
		bool OnIndividualBookTickerChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		// last trade market data from exchange
		bool OnTradeChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;

		void ReportTradeResults(const ParentOrderId& parentOrderId) override;

		void InitializeParameters(const std::string& strategyCfgPath) override;

		void StartTrade() override;

		void StopTrade() override;

		// external callback for parent order event handling
		void OnNewExternalParentOrder(
			 OrderManagement::NewExternalParentOrder* newOrder) override;

		void OnCancelExternalParentOrder(
			 OrderManagement::CancelExternalParentOrder* cancelOrder) override;

		void OnAmendExternalParentOrder(
			 OrderManagement::AmendExternalParentOrder* amendOrder) override;

		// - This function is called when the alarm is triggered, which is used to send orders
		void OnAlarmTriggered(const int passToDerived = 0) override;
	private:
		void InitializeMarketDataAnalyzer();
		void SetupOrderScheduler();
		void SetupVWAPProfileBucketSeconds();
		void SetupVWAPRiskManagements();
		void CreateBinanceExchangeProfile(const std::string& symbol);
		void CreatePortfolioManagement(const std::string& symbol);
		void CreateOrderParameterGenerator();
		void SubscribeMarketData(const std::string& symbol);
		void InitMarketData();
		void UnsubscribeTargetSymbols();
		double CalculateCurrentVWAP(const ParentOrderId& parentOrderId);
		double GetOrderSizeForCurrentBucket(const ParentOrderId& parentOrderId, const std::size_t ts);
		void RecordTradeExecution(const ParentOrderId& parentOrderId, const double volume, const std::size_t ts);
		size_t GetBucketVWAPId(const std::size_t ts) const;
		void SendVWAPChildOrderToExchange(
			const ParentOrderId& parentOrderId,
			const std::string& symbol,
			const double orderSize,
			const double limitPrice,
			const OrderManagement::ParentOrderSide side);
		void HaltExecution();

		std::size_t m_currentExchangeTimePointMs{ 0 };
		int m_profileBucketMs{ 0 };
		double m_maxParticipationRate{ 0.0 };
		double m_averageSlippageTolerance{ 0.0 };
		double m_maxDrawdownTolerance{ 0.0 };
		std::unique_ptr<QuantitativeModel::OrderParammeterGenerator> m_orderParammeterGenerator;
		std::unique_ptr<QuantitativeModel::MarketDataAnalyzer> m_marketDataAnalyzer;
		std::unordered_map<std::string, VWAPParentOrder> m_activeVWAPOrders;
		std::vector<std::shared_ptr<OrderManagement::CancelExternalParentOrder>> m_cancellingParentOrders;
		std::vector<std::shared_ptr<OrderManagement::AmendExternalParentOrder>> m_amendingParentOrders;
		std::mutex m_mutex;
	};
};