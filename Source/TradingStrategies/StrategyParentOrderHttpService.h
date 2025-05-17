/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "Protobuf/strategy_parent_order.pb.h"
#include "Protobuf/strategy_parent_order.grpc.pb.h"
#undef max
#undef min
#include <grpcpp/grpcpp.h>
#include <memory>

namespace LibraryUtils {
    class Logger;
};

namespace TradingStrategies {

    class StrategyParentOrderHttpService final
        : public strategy::StrategyOrderService::Service {
    public:
        StrategyParentOrderHttpService();
        ~StrategyParentOrderHttpService() override;
    private:
        grpc::Status SubmitParentOrder(grpc::ServerContext* context,
            const strategy::StrategyParentOrderRequest* request,
            strategy::StrategyParentOrderResponse* response) override;
    };
};