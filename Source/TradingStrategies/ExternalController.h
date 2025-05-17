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
#include <thread>
#include <string>

namespace tinyxml2 {
    class XMLElement;
};

namespace LibraryUtils {
    class Logger;
};

namespace grpc {
    class Server;
};

namespace TradingStrategies {

    class StrategyRequestHttpService;
    class StrategyParentOrderHttpService;

    class DLL_CLASS_TRADING_TRATEGIES_EXPORTS ExternalController final
    {
    public:
        ExternalController(const tinyxml2::XMLElement* binanceRestAPIServerXmlCfg);
        ~ExternalController();

        void Start();
        void Stop();
    private:
        void WaitForIncomingMessage();

        std::unique_ptr<LibraryUtils::Logger> m_logger;
        // http services
        std::unique_ptr<StrategyRequestHttpService> m_strategyRequestHttpService;
        std::unique_ptr<StrategyParentOrderHttpService> m_strategyParentOrderHttpService;
        // http services
        std::unique_ptr<grpc::Server> m_grpcServer;
        std::thread m_grpcThread;
        std::string m_serverIpAddress;
        std::string m_serverPort;
        std::string m_serverConnection;
    };
};