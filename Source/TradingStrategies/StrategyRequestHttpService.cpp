#include "pch.h"
#include "StrategyRequestHttpService.h"

grpc::Status TradingStrategies::StrategyRequestServiceImpl::ControlStrategy(grpc::ServerContext* context, const strategy::StrategyRequest* request, strategy::StrategyResponse* response)
{
    return grpc::Status();
}
