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
#include <string>

#include "../OrderManagement/Order.h"

namespace LibraryUtils {
    class Logger;
};

namespace tinyxml2 {
    class XMLDocument;
};

/*
 * The class BinanceExchangeSimulator simulates trading activities on the Binance 
 * cryptocurrency exchange. It provides functionality to mimic market operations such as 
 * order placement, matching, and transaction history tracking. The simulator is designed to help
 * users test trading strategies, analyze performance, and learn about market dynamics
 * in a controlled environment without real monetary risk.
 */

namespace ExchangeSimulator {

    class MatchingEngine;
    class BqtJsonMessageServer;
    class UserAccountManager;
    class BinanceWalletServer;

    class DLL_CLASS_EXCHANGESIMULATOR_EXPORTS BinanceExchangeSimulator final
    {
    public:
        BinanceExchangeSimulator(const tinyxml2::XMLDocument* configSimulatorXml);
        ~BinanceExchangeSimulator();
        
        void Run();
        void Shutdown();
    private:
        void PrepareExchangeServices(const tinyxml2::XMLDocument* configSimulatorXml);

        std::unique_ptr<MatchingEngine> m_matchingEngine;
        std::unique_ptr<BqtJsonMessageServer> m_bqtJsonMessageServer;
        std::unique_ptr<UserAccountManager> m_userAccountManager;
        std::unique_ptr<BinanceWalletServer> m_binanceWalletServer;
        std::unique_ptr<LibraryUtils::Logger> m_logger;
    };
};