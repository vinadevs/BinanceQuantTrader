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
	class BinanceTrader;
};

namespace TradingStrategies {
	class TradingStrategyBase;
};

namespace ComplianceNRegulatory {
	class BinanceTradingRules;
}

namespace BinanceQuantTrader {

	class StrategyFactory final
	{
	public:
	static std::unique_ptr<TradingStrategies::TradingStrategyBase>
		CreateTargetStrategy(
			const tinyxml2::XMLElement* strategyXmlCfg,
			MarketData::RealTimeMarketData* marketData,
			UserAccount::BinanceTrader* trader,
			ComplianceNRegulatory::BinanceTradingRules* tradingRules);
	};
};