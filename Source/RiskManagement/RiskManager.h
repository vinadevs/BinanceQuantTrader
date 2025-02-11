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

namespace tinyxml2 {
	class XMLElement;
};

namespace LibraryUtils {
	class Logger;
};

namespace ComplianceNRegulatory {
	class BinanceTradingRules;
}

namespace RiskManagement {
	class RiskManager;
}

// -Risk management in trading is a vital discipline that helps traders
// protect their capitaland manage potential losses.
// -It includes essential functionalities such as tracking position size, 
// calculating risk per trade, and ensuring the position does not exceed
// a specified risk limit.

namespace RiskManagement {
	class DLL_CLASS_RISKMANAGEMENT_EXPORTS RiskManager
	{
	public:
		RiskManager(const tinyxml2::XMLElement* riskManagementConfigXml);
		~RiskManager();

		// Setter for account balance
		void SetAccountBalance(const double accountBalance);

		// Setter for risk per trade
		void SetRiskPerTradePercent(const double riskPerTradePercent);

		// Getter for account balance
		double GetAccountBalance() const;

		// Getter for risk per trade
		double GetRiskPerTradePercent() const;

		// Getter for max position size
		double GetMaxPositionSize() const;

		// Calculate position size based on entry price, stop loss, and risk per trade
		double CalculatePositionSize(double entryPrice, double stopLoss) const;
	private:
		std::unique_ptr<LibraryUtils::Logger> m_logger;

		double m_accountBalance{ 0 };      // Total account balance
		double m_riskPerTradePercent{ 0 }; // Risk percentage per trade (e.g., 1% = 0.01)
		double m_maxPositionSize{ 0 };     // Maximum allowable position size
	};
};

