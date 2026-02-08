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

namespace RiskManagement {
	class FutureRiskEngine;
}

// The DerivativesArbitrageStrategy class is an automated trading strategy designed to...

namespace TradingStrategies {
	class DLL_CLASS_TRADING_TRATEGIES_EXPORTS
		DerivativesArbitrageStrategy :
		public TradingStrategyBase, // strategy core
		public MarketData::MarketDataObserver, // market data update
		public LibraryUtils::AlarmSystem // schedule trading
	{
	public:
		DerivativesArbitrageStrategy(const std::string& strategyCfgPath,
			MarketData::RealTimeMarketData* marketData,
			UserAccount::Trader* trader,
			ComplianceNRegulatory::BinanceTradingRules* tradingRules);

		virtual ~DerivativesArbitrageStrategy();

		bool OnIndividualBookTickerChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;

		// when an order is opening and exchange sends back an ack message
		virtual void OnOrderOpeningPositionAck(const OrderManagement::BinanceNewOrder* openingOrder) override;
		// when an order is closed and exchange sends back an ack message
		virtual void OnOrderClosedPositionAck(const OrderManagement::BinanceNewOrder* closedOrder) override;
		// when an order is liquidated and exchange sends back an ack message
		virtual void OnOrderLiquidatedPositionAck(const OrderManagement::BinanceNewOrder* liquidatedOrder) override;
		// when an order is margin called and exchange sends back an ack message
		virtual void OnOrderMarginCalledPositionAck(const OrderManagement::BinanceNewOrder* marginCalledOrder) override;

		void ReportTradeResults(const std::string& symbol) override;

		void InitializeParameters(const std::string& strategyCfgPath) override;

		void StartTrade() override;

		void StopTrade() override;

		// - This function is called when the alarm is triggered, which is used to send orders
		void OnAlarmTriggered(const int passToDerived = 0) override;
	private:
		void InitializeMarketDataAnalyzer();
		void SetupOrderScheduler();
		void CreateBinanceExchangeProfile();
		void CreatePortfolioManagement();
		void CreateRiskManagementEngine();
		void PrepareTargetMonitorSymbols();
		void SubscribeTargetSymbols();
		void UnsubscribeTargetSymbols();
		// List of symbols that we will trade in future market
		std::vector<std::string> m_targetFutureTradeSymbols;
		std::unique_ptr<QuantitativeModel::MarketDataAnalyzer> m_marketDataAnalyzer;
		std::unique_ptr<RiskManagement::FutureRiskEngine> m_futureRiskEngine;
	};
};