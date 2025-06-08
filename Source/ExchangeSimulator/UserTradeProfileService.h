/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "Protobuf/user_trade_profile.pb.h"
#include "Protobuf/user_trade_profile.grpc.pb.h"

#include <grpcpp/grpcpp.h>
#include <memory>

namespace LibraryUtils {
    class Logger;
};

namespace ExchangeSimulator {

	class UserTradeProfileManager;

    class UserTradeProfileService final
        : public usertradeprofile::TradeService::Service {
    public:
        UserTradeProfileService(UserTradeProfileManager* userTradeProfileManager);
        ~UserTradeProfileService() override;

        grpc::Status UpdateUserTradeProfile(grpc::ServerContext* context,
            const usertradeprofile::UpdateUserTradeProfileRequest* request,
            usertradeprofile::UpdateUserTradeProfileResponse* response) override;
    private:
        std::unique_ptr<LibraryUtils::Logger> m_logger;
        UserTradeProfileManager* m_userTradeProfileManager{ nullptr };
    };
};