#include "pch.h"
#include "StrategyParentOrderHttpService.h"

grpc::Status TradingStrategies::StrategyParentOrderServiceImpl::SubmitParentOrder(grpc::ServerContext* context, const strategy::StrategyParentOrderRequest* request, strategy::StrategyParentOrderResponse* response)
{
    return grpc::Status();
}
