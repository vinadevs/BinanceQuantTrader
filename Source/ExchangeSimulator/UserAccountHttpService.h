/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "Protobuf/user_account_data.pb.h"
#include "Protobuf/user_account_data.grpc.pb.h"

#include <grpcpp/grpcpp.h>
#include <memory>

namespace LibraryUtils {
    class Logger;
};

namespace ExchangeSimulator {

/**
 * @class UserAccountHttpService
 * @brief Implementation of the UserAccountService gRPC interface.
 *
 * This class provides the core functionality for managing binance wallet user accounts
*/
    class UserAccountManager;
    class UserAccountHttpService final : public UserAccountService::Service {
    public:
        UserAccountHttpService(UserAccountManager* userAccountManager);
        ~UserAccountHttpService() override;
    private:
        grpc::Status GetUserAccountData(
            grpc::ServerContext* context,
            const UserAccountDataRequest* request,
            UserAccountDataResponse* response) override;

        std::unique_ptr<LibraryUtils::Logger> m_logger;
        UserAccountManager* m_userAccountManager{ nullptr };
    };
};