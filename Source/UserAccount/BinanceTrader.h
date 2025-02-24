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
#include "../OrderManagement/BinanceWorkedOrderManager.h"
#include "../OrderManagement/PositionManager.h"

#if USE_TEST_TRADING
#include "../MiddlewareMQ/BqtJsonMessage.h"
#endif

#include "BinanceReporter.h"
#include "Trader.h"

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

	class DLL_CLASS_USERACCOUNT_EXPORTS
		BinanceTrader : public Trader
	{
	public:
		BinanceTrader(const tinyxml2::XMLElement* reportCfg, 
			          PortfolioManager::PortfolioInvestmentBinance* portfolio,
					  RiskManagement::RiskManager* riskManager);

		////////////// UPSTREAM PROCESSING /////////////////////////////

		bool Buy(const std::string& symbol,
			const binapi::double_type quality,
			const binapi::double_type refPrice) override;

		bool Sell(const std::string& symbol,
			const binapi::double_type quality,
			const binapi::double_type refPrice) override;

		void ReportTradeData(const std::string& symbol) override;

		void UpdateAccountInfo();

		PortfolioManager::PortfolioInvestmentBinance* GetPortfolio() const { return m_portfolio; }
		
		////////////// DOWNSTREAM PROCESSING /////////////////////////////
#if USE_TEST_TRADING  
		void HandleDownstreamAckMessage(const MiddlewareMQ::BqtJsonMessage& message);
#endif
	private:
		void SetupReporter(const tinyxml2::XMLElement* reportCfg) override;
		binapi::double_type CalculateTradeValue(
			const binapi::double_type quality,
			const binapi::double_type refPrice);

		PortfolioManager::PortfolioInvestmentBinance* m_portfolio{ nullptr };
		RiskManagement::RiskManager* m_riskManager{ nullptr };  // stop loss
		std::unique_ptr<OrderManagement::BinanceWorkedOrderManager> m_workedOrderManager;
		std::unique_ptr<OrderManagement::PositionManager> m_positionManager;
		std::unique_ptr<BinanceReporter> m_reporter;

		bool m_enableTradeReporter{ false };
		bool m_enableOpenOrderReporter{ false };
		bool m_enableBalanceReporter{ false };
	};
};