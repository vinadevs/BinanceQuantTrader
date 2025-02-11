/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include "../KernelTrading/double_type.h"
#include "../OrderManagement/PositionManager.h"

#include "Trader.h"
#include "BinanceReporter.h"

#include <string>
#include <memory>

namespace PortfolioManager {
	class PortfolioInvestmentBinance;
}

namespace RiskManagement {
	class RiskManager;
}

namespace tinyxml2 {
	class XMLElement;
};

namespace UserAccount {
	class BinanceReporter;
	class DLL_CLASS_USERACCOUNT_EXPORTS
		BinanceTrader : public Trader
	{
	public:
		BinanceTrader(const tinyxml2::XMLElement* reportCfg, 
			          PortfolioManager::PortfolioInvestmentBinance* portfolio,
					  RiskManagement::RiskManager* riskManager);

		bool Buy(const std::string& symbol,
			const binapi::double_type quality,
			const binapi::double_type refPrice) override;

		bool Sell(const std::string& symbol,
			const binapi::double_type quality,
			const binapi::double_type refPrice) override;

		void ReportTradeData(const std::string& symbol) override;

		void UpdateAccountInfo();

		PortfolioManager::PortfolioInvestmentBinance* GetPortfolio() const { return m_portfolio; }
	private:
		void SetupReporter(const tinyxml2::XMLElement* reportCfg) override;

		PortfolioManager::PortfolioInvestmentBinance* m_portfolio{ nullptr };
		RiskManagement::RiskManager* m_riskManager{ nullptr };  // stop loss
		std::unique_ptr<OrderManagement::PositionManager> m_positionManager;
		std::unique_ptr<BinanceReporter> m_reporter;

		bool m_enableTradeReporter{ false };
		bool m_enableOpenOrderReporter{ false };
		bool m_enableBalanceReporter{ false };
	};
};