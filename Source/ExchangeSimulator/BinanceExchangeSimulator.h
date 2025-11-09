/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include "ExchangeServiceInterface.h"

#include <memory>

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

    class UserAccountManager;
	class ExchangeInfoManager;
	class UserTradeProfileManager;

    class DLL_CLASS_EXCHANGESIMULATOR_EXPORTS BinanceExchangeSimulator final
    {
    public:
        BinanceExchangeSimulator(const tinyxml2::XMLDocument* configSimulatorXml);
        ~BinanceExchangeSimulator();
        
        // Start exchange services, now the simulator can receive test orders
        // from upstream side and fill them
        void Run();
        // Bring down the simulator when we finished the strategy benchmarking
        void Shutdown();
    private:
        // All exchange services will be initiated in this function
        // when we add new services, it should be initiated here
        void PrepareExchangeServices(const tinyxml2::XMLDocument* configSimulatorXml);

        // List exchange component services 
        ExchangeService m_matchingEngine;
        ExchangeService m_bqtJsonMessageServer;
        ExchangeService m_binanceRestAPIServer;

        std::unique_ptr<UserAccountManager> m_userAccountManager;
        std::unique_ptr<ExchangeInfoManager> m_exchangeInfoManager;
		std::unique_ptr<UserTradeProfileManager> m_userTradeProfileManager;
        std::unique_ptr<LibraryUtils::Logger> m_logger;
    };
};