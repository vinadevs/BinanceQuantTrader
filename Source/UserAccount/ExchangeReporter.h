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

namespace LibraryUtils {
	class Logger;
};

namespace PortfolioManager {
	class PortfolioInvestmentBinance;
}

namespace UserAccount {
	class ExchangeReporter
	{
	public:
		ExchangeReporter(PortfolioManager::PortfolioInvestmentBinance* portfolio)
			: m_portfolio(portfolio) {}
		virtual ~ExchangeReporter() {};
		virtual void SetupReporter(const tinyxml2::XMLElement* reportCfg) = 0;
		virtual void UpdateRemoteReportTrades(const std::string& symbol) = 0;
		virtual void UpdateRemoteReportOpenOrders(const std::string& symbol) = 0;
		virtual void UpdateRemoteReportAccountBalance(const std::string& symbol) = 0;
		virtual void DoRemoteExecutionReport(const std::string& symbol) = 0;
		virtual void DoLocalExecutionReport(const std::string& symbol) = 0;
		virtual void DoTradeExecutionReport() = 0;
	protected:
		virtual bool MergeLocalAndRemmoteReport() = 0;

		PortfolioManager::PortfolioInvestmentBinance* m_portfolio{ nullptr };
		std::unique_ptr<LibraryUtils::Logger> m_logger;

		bool m_enableTradeReporter{ false };
		bool m_enableOpenOrderReporter{ false };
		bool m_enableBalanceReporter{ false };
	};
};

