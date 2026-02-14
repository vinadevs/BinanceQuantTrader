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

#include "BinanceTrader.h"
#include "FutureTrader.h"


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

namespace UserAccount {

	// This class will manage binance hybrid trade activities for both spot and future like buy/sell/report...
	// It will also manage the portfolio, trading rules, and risk management.
	class DLL_CLASS_USERACCOUNT_EXPORTS
		HybridTrader final : public Trader
	{
	public:
		HybridTrader() = default;

		HybridTrader(
			const tinyxml2::XMLElement* reportCfg,
			PortfolioManager::PortfolioInvestmentBinance* portfolio,
			ComplianceNRegulatory::BinanceTradingRules* tradingRules,
			RiskManagement::RiskManager* riskManager);

		bool CreateSpotTrader(
			const tinyxml2::XMLElement* reportCfg,
			PortfolioManager::PortfolioInvestmentBinance* portfolio,
			ComplianceNRegulatory::BinanceTradingRules* tradingRules,
			RiskManagement::RiskManager* riskManager);

		bool CreateFutureTrader(
			const tinyxml2::XMLElement* reportCfg,
			PortfolioManager::PortfolioInvestmentBinance* portfolio,
			ComplianceNRegulatory::BinanceTradingRules* tradingRules,
			RiskManagement::RiskManager* riskManager);

		UserAccount::BinanceTrader* GetSpotTrader() const;

		UserAccount::FutureTrader* GetFutureTrader() const;

		void ReportTradeResults(const std::string& symbol);
	private:
		std::unique_ptr<UserAccount::BinanceTrader> m_spotTrader; // binance spot trader
		std::unique_ptr<UserAccount::FutureTrader> m_futureTrader; // binance future trader
	};
}; // namespace UserAccount