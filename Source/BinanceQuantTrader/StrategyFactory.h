/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include <memory>

namespace tinyxml2 {
	class XMLElement;
};

namespace MarketData {
	class RealTimeMarketData;
};

namespace UserAccount {
	class Trader;
};

namespace TradingStrategies {
	class TradingStrategyBase;
};

namespace ComplianceNRegulatory {
	class BinanceTradingRules;
}

namespace BinanceQuantTrader {

	// StrategyFactory is responsible for creating instances of trading strategies based on XML configuration.
	// It encapsulates the logic for constructing the appropriate TradingStrategyBase-derived object
	// using provided market data, trader, and trading rules dependencies.
	class StrategyFactory final
	{
	public:
		// Deleted default constructor to prevent instantiation without parameters.
		StrategyFactory() = delete;
		// Deleted copy constructor and assignment operator to prevent copying.
		StrategyFactory(const StrategyFactory&) = delete;
		StrategyFactory& operator=(const StrategyFactory&) = delete;
		// Deleted move constructor and assignment operator to prevent moving.
		StrategyFactory(StrategyFactory&&) = delete;
		StrategyFactory& operator=(StrategyFactory&&) = delete;
		// Factory method to create a trading strategy based on the provided XML configuration.
		static std::unique_ptr<TradingStrategies::TradingStrategyBase>
			CreateTargetStrategy(
				const tinyxml2::XMLElement* strategyXmlCfg,
				MarketData::RealTimeMarketData* marketData,
				UserAccount::Trader* trader,
				ComplianceNRegulatory::BinanceTradingRules* tradingRules);
	};
};