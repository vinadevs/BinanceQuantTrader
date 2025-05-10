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

#include <memory>

namespace LibraryUtils {
    class Logger;
};

namespace TradingStrategies {

    class StrategyParentOrderServiceImpl final : public strategy::StrategyOrderService::Service {
        grpc::Status SubmitParentOrder(grpc::ServerContext* context,
            const strategy::StrategyParentOrderRequest* request,
            strategy::StrategyParentOrderResponse* response) override;
    };
};