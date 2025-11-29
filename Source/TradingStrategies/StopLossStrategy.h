/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"

#include "../LibraryUtils/BQTQueue.h"
#include "../IndicatorNSignals/TradingSignalService.h"

#include "TradingStrategyBase.h"

#include <memory>
#include <string>
#include <vector>
#if USE_MULTITHREADING
#include <queue>
#include <mutex>
#include <condition_variable>
#endif

namespace MarketData {
	class RealTimeMarketData;
}

namespace UserAccount {
	class Trader;
}

namespace ComplianceNRegulatory {
	class BinanceTradingRules;
}

namespace QuantitativeModel {
	class OrderParammeterGenerator;
};

namespace tinyxml2 {
	class XMLDocument;
};

// The StopLossStrategy class is an automated trading strategy designed to manage stop-loss orders 
// in a trading system. It inherits from TradingStrategyBase and implements the TradingHintsListener 
// interface to react to trading hints. This class is responsible for initializing parameters, 
// managing trading signals, subscribing to target symbols, and handling live trading operations. 
// It supports multithreading for efficient processing of trading hints and ensures thread safety 
// using mutexes and condition variables. The class also integrates with various components such as 
// market data, trading rules, and order parameter generators to execute trades based on predefined 
// stop-loss strategies.

// THIS IS AN AUTO STRATEGY

namespace TradingStrategies {
	class DLL_CLASS_TRADING_TRATEGIES_EXPORTS
		StopLossStrategy
		: public TradingStrategyBase,
		  public IndicatorNSignals::TradingHintsListener
	{
	public:
		StopLossStrategy(const std::string& strategyCfgPath,
			MarketData::RealTimeMarketData* marketData,
			UserAccount::Trader* trader,
			ComplianceNRegulatory::BinanceTradingRules* tradingRules);

		virtual ~StopLossStrategy();

		bool OnReceivedTradingHints(const IndicatorNSignals::TradingHints* hints) override;

		void ReportTradeResults(const std::string& symbol) override;

		void InitializeParameters(const std::string& strategyCfgPath) override;

		void StartTrade() override;

		void StopTrade() override;
	private:
		void CreateTradingSignalServices();
		void SubscribeTargetSymbols();
		void UnsubscribeTargetSymbols();
		void CreatePortfolioManagement();
		void CreateOrderParameterGenerator();
		void CreateBinanceExchangeProfile();
		bool TradeAsHints(const IndicatorNSignals::TradingHints* hints);

#if USE_MULTITHREADING
		void TradingLoop();

		std::unique_ptr<tinyxml2::XMLDocument> m_strategyCfgXml;
		std::unique_ptr<QuantitativeModel::OrderParammeterGenerator> m_orderParammeterGenerator;
		std::vector<std::string> m_targetTradeSymbols;
		std::mutex m_tradingHintsMutex;
		std::condition_variable m_tradingHintCond; // avoid polling thread
		std::atomic<bool> m_hasNewTradingHint{ false }; // lock free thread
		// customize queue with thread safe protection
		MultipleThreads::BQTQueue<const IndicatorNSignals::TradingHints*> m_tradingHintsQueue;
#endif
		std::unique_ptr<IndicatorNSignals::TradingSignalService> m_tradingSignalService;
	};
};