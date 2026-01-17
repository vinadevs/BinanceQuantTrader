/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include <string>
#include <unordered_map>

namespace TradingStrategies {

	class TradingStrategyBase;

	class DLL_CLASS_TRADING_TRATEGIES_EXPORTS StrategyManager final {
	public:
		void AddStrategy(TradingStrategyBase* strategy);

		TradingStrategyBase* GetStrategyByID(const std::string& strategyID);

		bool RemoveStrategyByID(const std::string& strategyID);
	private:
		// Map strategy id to strategy object
		std::unordered_map<std::string, TradingStrategyBase*> m_strategies;
	};
}