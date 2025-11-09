/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "protobuf/user_future_account_data.pb.h"
#include "protobuf/user_future_account_data.grpc.pb.h"

#include <grpcpp/grpcpp.h>
#include <memory>

namespace LibraryUtils {
    class Logger;
};

namespace ExchangeSimulator {

    /**
     * @class UserFutureAccountService
     * @brief Implementation of the UserFutureAccountService gRPC interface.
     *
     * This class provides the core functionality for managing binance wallet user accounts
    */
    class UserAccountManager;

    class UserFutureAccountService final
        : public futureaccount::UserAccountService::Service {
    public:
        UserFutureAccountService(UserAccountManager* userAccountManager);
        ~UserFutureAccountService() override;
    private:
        grpc::Status GetUserFutureAccount(
			grpc::ServerContext* context,
			const futureaccount::GetUserFutureAccountRequest* request,
			futureaccount::GetUserFutureAccountResponse* response) override;

        std::unique_ptr<LibraryUtils::Logger> m_logger;
        UserAccountManager* m_userAccountManager{ nullptr };
    };
};