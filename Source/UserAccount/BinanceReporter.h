/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "ExchangeReporter.h"

#include <string>

namespace tinyxml2 {
	class XMLElement;
};

namespace PortfolioManager {
	class PortfolioInvestmentBinance;
}

namespace UserAccount {
	class BinanceReporter final : public ExchangeReporter
	{
	public:
		BinanceReporter(
			const tinyxml2::XMLElement* reportConfigXml,
			PortfolioManager::PortfolioInvestmentBinance* portfolio);
		BinanceReporter() = default;
		~BinanceReporter() override;
		void SetupReporter(const tinyxml2::XMLElement* reportCfg) override;
		void UpdateRemoteReportTrades(const std::string& symbol) override;
		void UpdateRemoteReportOpenOrders(const std::string& symbol) override;
		void UpdateRemoteReportAccountBalance(const std::string& symbol) override;
		void UpdateRemoteReportExchangerPriceForOrders(const std::string& symbol) override;
		void UpdateRemoteReportCalculateLossForOrders(const std::string& symbol) override;
		void DoRemoteExecutionReport(const std::string& symbol) override;
		void DoLocalExecutionReport(const std::string& symbol) override;
		void DoTradeExecutionReport() override;
	private:
		bool MergeLocalAndRemmoteReport() override;
	};
};

