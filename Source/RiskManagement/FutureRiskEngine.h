/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"
#include "RiskDefinitions.h"

#include <memory>

namespace LibraryUtils {
	class Logger;
};


namespace RiskManagement {
	class RiskManager;
}

namespace PortfolioManager {
	class PortfolioInvestmentBinance;
}

namespace KernelTrading {
	class UserFutureAccount;
}

namespace OrderManagement {
	class Order;
}

namespace RiskManagement {

	// Risk management engine for futures trading
	// This engine will monitor and manage the risks associated with futures trading activities
	// It will analyze market conditions, portfolio exposure, and user account status to make risk mitigation decisions

	class DLL_CLASS_RISKMANAGEMENT_EXPORTS FutureRiskEngine
	{
	public:
		FutureRiskEngine(PortfolioManager::PortfolioInvestmentBinance* portfolio,
						 RiskManagement::RiskManager* riskManager,
			             KernelTrading::UserFutureAccount* userFutureAccount,
						 LibraryUtils::Logger* logger);
		~FutureRiskEngine();

		RiskReport AssessTradingRisk(const OrderManagement::Order* order);

	private:
		LibraryUtils::Logger* m_logger{ nullptr };
		PortfolioManager::PortfolioInvestmentBinance* m_portfolio{ nullptr };
		RiskManagement::RiskManager* m_riskManager{ nullptr };
		KernelTrading::UserFutureAccount* m_userFutureAccount{ nullptr };
	};
};

