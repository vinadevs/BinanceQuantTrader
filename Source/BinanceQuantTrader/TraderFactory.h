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

namespace UserSpotAccount {
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

	// TraderFactory is responsible for creating instances of BinanceTrader based on XML configuration.
	// It encapsulates the logic for constructing the appropriate BinanceTrader object
	// using provided portfolio, trading rules, and risk management dependencies.
	class TraderFactory final
	{
	public:
		// Deleted default constructor to prevent instantiation without parameters.
		TraderFactory() = delete;
		// Deleted copy constructor and assignment operator to prevent copying.
		TraderFactory(const TraderFactory&) = delete;
		TraderFactory& operator=(const TraderFactory&) = delete;
		// Deleted move constructor and assignment operator to prevent moving.
		TraderFactory(TraderFactory&&) = delete;
		TraderFactory& operator=(TraderFactory&&) = delete;
		// Factory method to create a BinanceTrader based on the provided XML configuration.
		static std::unique_ptr<UserSpotAccount::BinanceTrader>
			CreateSmartTrader(PortfolioManager::PortfolioInvestmentBinance* portfolio,
							  ComplianceNRegulatory::BinanceTradingRules* tradingRules,
							  RiskManagement::RiskManager* riskManager,
				              const tinyxml2::XMLElement* traderXmlCfg);
	};
};