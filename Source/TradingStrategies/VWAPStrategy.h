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
	class Trader;
}

namespace ComplianceNRegulatory {
	class BinanceTradingRules;
}

namespace RiskManagement {
	class RiskManager;
}

namespace TradingStrategies {
	class DLL_CLASS_TRADING_TRATEGIES_EXPORTS
		VWAPStrategy final 
	{
	public:

	};
};