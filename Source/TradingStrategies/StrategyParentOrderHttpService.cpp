#include "pch.h"
#include "StrategyParentOrderHttpService.h"

using namespace TradingStrategies;
using namespace LibraryUtils;

StrategyParentOrderHttpService::StrategyParentOrderHttpService()
{
}

StrategyParentOrderHttpService::~StrategyParentOrderHttpService()
{
}

grpc::Status StrategyParentOrderHttpService::SubmitParentOrder(grpc::ServerContext* context, const strategy::StrategyParentOrderRequest* request, strategy::StrategyParentOrderResponse* response)
{
    return grpc::Status();
}
