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

namespace UserAccount {
	class BinanceTrader;
};

namespace PortfolioManager {
	class PortfolioInvestmentBinance;
}

namespace ComplianceNRegulatory {
	class BinanceTradingRules;
}

namespace RiskManagement {
	class RiskManager;
}

namespace BinanceQuantTrader {

	class TraderFactory final
	{
	public:
		static std::unique_ptr<UserAccount::BinanceTrader>
			CreateSmartTrader(PortfolioManager::PortfolioInvestmentBinance* portfolio,
							  ComplianceNRegulatory::BinanceTradingRules* tradingRules,
							  RiskManagement::RiskManager* riskManager,
				              const tinyxml2::XMLElement* traderXmlCfg);
	};
};