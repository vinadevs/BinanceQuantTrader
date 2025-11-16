/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"

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

// -Risk management in trading is a vital discipline that helps traders
// protect their capitaland manage potential losses.
// -It includes essential functionalities such as tracking position size, 
// calculating risk per trade, and ensuring the position does not exceed
// a specified risk limit.

namespace RiskManagement {
	class DLL_CLASS_RISKMANAGEMENT_EXPORTS FutureRiskEngine
	{
	public:
		FutureRiskEngine(PortfolioManager::PortfolioInvestmentBinance* portfolio,
						 RiskManagement::RiskManager* riskManager,
			             KernelTrading::UserFutureAccount* userFutureAccount,
						 LibraryUtils::Logger* logger);
		~FutureRiskEngine();
	private:
		LibraryUtils::Logger* m_logger{ nullptr };
		PortfolioManager::PortfolioInvestmentBinance* m_portfolio{ nullptr };
		RiskManagement::RiskManager* m_riskManager{ nullptr };
		KernelTrading::UserFutureAccount* m_userFutureAccount{ nullptr };
	};
};

