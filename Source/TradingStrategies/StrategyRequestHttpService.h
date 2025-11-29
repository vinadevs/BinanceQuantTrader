/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "Protobuf/strategy_control_request.pb.h"
#include "Protobuf/strategy_control_request.grpc.pb.h"

#undef max
#undef min
#include <grpcpp/grpcpp.h>
#include <memory>

namespace LibraryUtils {
    class Logger;
};

namespace TradingStrategies {

	class ExternalRequestReceiver;

    class StrategyRequestHttpService final
        : public strategy::StrategyRequestService::Service {
	public:
        StrategyRequestHttpService();
        ~StrategyRequestHttpService() override;

		void AddTargetAdminRequestHandler(ExternalRequestReceiver* receiver);

    private:
        grpc::Status ControlStrategy(grpc::ServerContext* context,
            const strategy::StrategyRequest* request,
            strategy::StrategyResponse* response) override;

		ExternalRequestReceiver* m_targetAdminRequestHandler{ nullptr };
    };
};