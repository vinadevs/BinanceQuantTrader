/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"

#include "../OrderManagement/PositionManager.h"

#include "Trader.h"

#include <string>
#include <vector>
#include <memory>

namespace PortfolioManager {
	class PortfolioInvestmentBinance;
}

namespace RiskManagement {
	class RiskManager;
}

namespace ComplianceNRegulatory {
	class BinanceTradingRules;
}

namespace tinyxml2 {
	class XMLElement;
};

namespace TradingStrategies {
	class TradingStrategyBase;
};

namespace UserAccount {

	// This class will manage binance hybrid trade activities for both spot and future like buy/sell/report...
	// It will also manage the portfolio, trading rules, and risk management.
	class DLL_CLASS_USERACCOUNT_EXPORTS
		HybridTrader final : public Trader
	{
	};
};