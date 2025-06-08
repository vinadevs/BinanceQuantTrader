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

namespace OrderManagement {
	class PositionManager;
}

namespace UserAccount {
    /**
     * @class BinanceReporter
     * @brief A final class derived from ExchangeReporter to handle reporting for Binance exchange.
     *
     * This class is responsible for generating various reports related to trading activities on the Binance exchange.
     * It provides functionalities to update remote data, setup the reporter, and generate different types of reports
     * such as trades, open orders, account balance, exchange prices for orders, and loss calculations for orders.
     * It also supports remote and local execution reports and merging local and remote reports.
     */
    class BinanceReporter final : public ExchangeReporter
    {
    public:
        BinanceReporter(
            const tinyxml2::XMLElement* reportConfigXml,
            binapi::rest::account_info_t* accountInfo,
            ComplianceNRegulatory::BinanceExchangeProfileMgr* exchangeProfileMgr,
            OrderManagement::PositionManager* positionManager);
        BinanceReporter() = default;
        ~BinanceReporter() override;
        void UpdateRemoteData(const std::string& symbol) override;
        void SetupReporter(const tinyxml2::XMLElement* reportCfg) override;
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

