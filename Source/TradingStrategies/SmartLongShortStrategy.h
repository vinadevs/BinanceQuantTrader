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

// The SmartLongShortStrategy class is an automated trading strategy designed to manage long and short positions
// in future market. It inherits from TradingStrategyBase and implements the TradingHintsListener
// interface to react to trading hints. This class is responsible for initializing parameters, managing trading
// signals, subscribing to target symbols, and handling live trading operations. It supports multithreading for
// efficient processing of trading hints and ensures thread safety using mutexes and condition variables. The class
// also integrates with various components such as market data, trading rules, and order parameter generators to
// execute trades based on predefined long and short strategies.

namespace TradingStrategies {
	class DLL_CLASS_TRADING_TRATEGIES_EXPORTS
		SmartLongShortStrategy :
			public TradingStrategyBase, // strategy core
			public MarketData::MarketDataObserver, // market data update
			public LibraryUtils::AlarmSystem // schedule trading
	{
	public:
		SmartLongShortStrategy(const std::string& strategyCfgPath,
			MarketData::RealTimeMarketData* marketData,
			UserAccount::Trader* trader,
			ComplianceNRegulatory::BinanceTradingRules* tradingRules);

		virtual ~SmartLongShortStrategy();

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