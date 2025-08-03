/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "FutureTradeReporter.h"

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

namespace KernelTrading {
	class UserFutureAccount;
}

namespace UserAccount {
    
	/**
	 * @class FutureBackTestReporter
	 * @brief A final class that extends FutureTradeReporter to provide backtesting reporting functionalities for futures trading.
	 *
	 * This class is responsible for setting up the reporter, updating remote data, and generating various reports
	 * related to trades, open orders, account balance, exchange prices, and loss calculations. It also handles
	 * remote and local execution reports.
	 */
    class FutureBackTestReporter final : public FutureTradeReporter
    {
    public:
        FutureBackTestReporter(
            const tinyxml2::XMLElement* reportConfigXml,
            KernelTrading::UserFutureAccount* accountInfo,
            ComplianceNRegulatory::BinanceExchangeProfileMgr* exchangeProfileMgr,
            OrderManagement::PositionManager* positionManager);

        FutureBackTestReporter() = default;
        ~FutureBackTestReporter() override;

        void SetupReporter(const tinyxml2::XMLElement* reportCfg) override;
        void DoTradeExecutionReport(const std::string& symbol) override;
		void UpdateRemoteData(const std::string& symbol) override;
    };
};

