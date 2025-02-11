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
#include "Trader.h"

#include <string>

namespace PortfolioManager {
	class PortfolioInvestmentLocalTest;
}

namespace UserAccount {
	class DLL_CLASS_USERACCOUNT_EXPORTS 
		TestTrader : public Trader
	{
	public:
		TestTrader(PortfolioManager::PortfolioInvestmentLocalTest* portfolio);

		bool Buy(const std::string& symbol,
			const binapi::double_type quality, 
			const binapi::double_type refPrice) override;

		bool Sell(const std::string& symbol,
			const binapi::double_type quality, 
			const binapi::double_type refPrice) override;

		void ReportTradeData(const std::string& symbol) override;
	private:
		PortfolioManager::PortfolioInvestmentLocalTest* m_portfolio{ nullptr };
	};
};