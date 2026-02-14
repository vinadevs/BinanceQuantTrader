/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "protobuf/exchange_info_data.pb.h"
#include "protobuf/exchange_info_data.grpc.pb.h"

#include <grpcpp/grpcpp.h>
#include <memory>

namespace LibraryUtils {
    class Logger;
};

namespace ExchangeSimulator {

/**
 * @class ExchangeInfoHttpService
 * @brief Implementation of the ExchangeService gRPC interface.
 *
 * This class provides the core functionality for managing binance exchange info
*/
    class ExchangeInfoManager;

    class ExchangeInfoHttpService final 
        : public exchange::ExchangeService::Service {
    public:
        ExchangeInfoHttpService(ExchangeInfoManager* exchangeInfoManager);
        ~ExchangeInfoHttpService() override;
    private:
        grpc::Status GetExchangeInfo(
            grpc::ServerContext* context,
            const exchange::ExchangeInfoRequest* request,
            exchange::ExchangeInfoResponse* response) override;

        std::unique_ptr<LibraryUtils::Logger> m_logger;
		ExchangeInfoManager* m_exchangeInfoManager{ nullptr };
    };
};