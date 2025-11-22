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

namespace OrderManagement {
	class Order;
}

namespace binapi {
	namespace rest {
		struct account_info_t;
	}
}

namespace RiskManagement {

	// Risk management engine for spot trading
	// This engine will monitor and manage the risks associated with spot trading activities
	// It will analyze market conditions, portfolio exposure, and user account status to make risk mitigation decisions

	class DLL_CLASS_RISKMANAGEMENT_EXPORTS SpotRiskEngine
	{
	public:
		SpotRiskEngine(PortfolioManager::PortfolioInvestmentBinance* portfolio,
			RiskManagement::RiskManager* riskManager,
			binapi::rest::account_info_t* userSpotAccount,
			LibraryUtils::Logger* logger);
		~SpotRiskEngine();

		RiskReport AssessTradingRisk(const OrderManagement::Order* order);

	private:
		LibraryUtils::Logger* m_logger{ nullptr };
		PortfolioManager::PortfolioInvestmentBinance* m_portfolio{ nullptr };
		RiskManagement::RiskManager* m_riskManager{ nullptr };
		binapi::rest::account_info_t* m_userSpotAccount{ nullptr };
	};
};

