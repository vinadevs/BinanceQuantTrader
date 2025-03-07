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
#if USE_MULTITHREADING
#include <queue>
#include <mutex>
#include <condition_variable>
#endif

namespace MarketData {
	class RealTimeMarketData;
}

namespace UserAccount {
	class BinanceTrader;
}

namespace ComplianceNRegulatory {
	class BinanceTradingRules;
}

// The fear of missing out, or FOMO, refers to the feeling or 
// perception that others are having more fun, living better lives,
// or experiencing better things than you are

// THIS IS AN AUTO STRATEGY

namespace TradingStrategies {
	class DLL_CLASS_TRADING_TRATEGIES_EXPORTS
		FomoTradingStrategy 
		: public TradingStrategyBase,
		  public IndicatorNSignals::TradingHintsListener
	{
	public:
		FomoTradingStrategy(const std::string& strategyCfgPath,
							MarketData::RealTimeMarketData* marketData,
							UserAccount::BinanceTrader* trader,
							ComplianceNRegulatory::BinanceTradingRules* tradingRules);

		virtual ~FomoTradingStrategy();

		bool OnReceivedTradingHints(const IndicatorNSignals::TradingHints* hints) override;
#ifndef USE_BACK_TEST_TRADING
		void ReportTradeResults(const std::string& symbol) override;
#endif
		void InitializeParameters(const std::string& strategyCfgPath) override;

		void StartLive() override;

		void StopLive() override;
	private:
		bool TradeAsHints(const IndicatorNSignals::TradingHints* hints);

#if USE_MULTITHREADING
		void TradingLoop();
		std::mutex m_marketDataMutex;
		std::condition_variable m_tradingHintCond; // avoid polling thread
		std::atomic<bool> m_hasNewTradingHint{ false }; // lock free thread
		// customize queue with thread safe protection
		MultipleThreads::BQTQueue<const IndicatorNSignals::TradingHints*> m_tradingHintsQueue;
#endif
		std::unique_ptr<IndicatorNSignals::TradingSignalService> m_tradingSignalService;
	};
};