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

namespace ComplianceNRegulatory {
    class BinanceExchangeProfileMgr;
}

namespace PortfolioManager {
    class PortfolioInvestmentBinance;
}

namespace UserAccount {
    /**
    * @class BackTestReporter
    * @brief A final class that extends ExchangeReporter to provide backtesting reporting functionalities.
    *
    * This class is responsible for setting up the reporter, updating remote data, and generating various reports
    * related to trades, open orders, account balance, exchange prices, and loss calculations. It also handles
    * remote and local execution reports and merges local and remote reports.
    */
    class BackTestReporter final : public ExchangeReporter
    {
    public:
        BackTestReporter(
            const tinyxml2::XMLElement* reportConfigXml,
            binapi::rest::account_info_t* accountInfo,
            ComplianceNRegulatory::BinanceExchangeProfileMgr* exchangeProfileMgr,
            PortfolioManager::PortfolioInvestmentBinance* portfolio);
        BackTestReporter() = default;
        ~BackTestReporter() override;
        void SetupReporter(const tinyxml2::XMLElement* reportCfg) override;
        void UpdateRemoteData(const std::string& symbol) override;
        void UpdateRemoteReportTrades(const std::string& symbol) override;
        void UpdateRemoteReportOpenOrders(const std::string& symbol) override;
        void UpdateRemoteReportAccountBalance(const std::string& symbol) override;
        void UpdateRemoteReportExchangerPriceForOrders(const std::string& symbol) override;
        void UpdateRemoteReportCalculateLossForOrders(const std::string& symbol) override;
        void DoRemoteExecutionReport(const std::string& symbol) override;
        void DoLocalExecutionReport(const std::string& symbol) override;
        void DoTradeExecutionReport(const std::string& symbol) override;
    private:
        bool MergeLocalAndRemmoteReport() override;
    };
};

