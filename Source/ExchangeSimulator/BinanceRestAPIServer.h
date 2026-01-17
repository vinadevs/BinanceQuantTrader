/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "ExchangeServiceInterface.h"

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

namespace ExchangeSimulator {

    /**
     * @class BinanceRestAPIServer
     * @brief A Google Protobuf HTTP message server for handling account/exchange queries.
     * This server is specifically designed to receive and process user account
     * queries from the Algo Trading System. 
     */

    class UserAccountManager;
	class ExchangeInfoManager;
	class UserTradeProfileManager;
    class UserAccountHttpService;
	class UserFutureAccountService;
	class ExchangeInfoHttpService;
	class UserTradeProfileService;

    class BinanceRestAPIServer final : public ExchangeServiceInterface
    {
    public:
        BinanceRestAPIServer(
            const tinyxml2::XMLElement* binanceRestAPIServerXmlCfg,
            UserAccountManager* userAccountManager,
            ExchangeInfoManager* exchangeInfoManager,
            UserTradeProfileManager* userTradeProfileManager);
        ~BinanceRestAPIServer() override;

        void Start() override;
        void Stop() override;
    private:
        void WaitForIncomingMessage();

        std::unique_ptr<LibraryUtils::Logger> m_logger;
		// http services
        std::unique_ptr<UserAccountHttpService> m_userAccountHttpService;
		std::unique_ptr<UserFutureAccountService> m_userFutureAccountHttpService;
        std::unique_ptr<ExchangeInfoHttpService> m_exchangeInfoHttpService;
		std::unique_ptr<UserTradeProfileService> m_userTradeProfileService;
        // http services
        std::unique_ptr<grpc::Server> m_grpcServer;
        std::thread m_grpcThread;
        std::string m_serverIpAddress;
        std::string m_serverPort;
        std::string m_serverConnection;
    };
};

