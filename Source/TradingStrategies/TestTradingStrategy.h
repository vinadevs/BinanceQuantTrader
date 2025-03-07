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

namespace MarketData {
	class RealTimeMarketData;
	class MarketDataSubject;
}

namespace UserAccount {
	class BinanceTrader;
}

namespace ComplianceNRegulatory {
	class BinanceTradingRules;
}

namespace RiskManagement {
	class RiskManager;
}

// Test market data only, can not get money here :D

namespace TradingStrategies {
	class DLL_CLASS_TRADING_TRATEGIES_EXPORTS 
		TestTradingStrategy : public TradingStrategyBase,
							  public MarketData::MarketDataObserver
	{
	public:
		TestTradingStrategy(
			const std::string& strategyCfgPath,
			MarketData::RealTimeMarketData* marketData,
			UserAccount::BinanceTrader* trader,
			ComplianceNRegulatory::BinanceTradingRules* tradingRules);

		virtual ~TestTradingStrategy();

		bool OnIndividualBookTickerChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
#ifndef USE_BACK_TEST_TRADING
		void ReportTradeResults(const std::string& symbol) override {}
#endif
		void InitializeParameters(const std::string& strategyCfgPath) override {}

		void StartLive() override {}

		void StopLive() override {}
	};
};