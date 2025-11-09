#include "pch.h"
#include "StrategyRequestHttpService.h"

using namespace TradingStrategies;
using namespace LibraryUtils;

StrategyRequestHttpService::StrategyRequestHttpService()
{
}

StrategyRequestHttpService::~StrategyRequestHttpService()
{
}

grpc::Status StrategyRequestHttpService::ControlStrategy(grpc::ServerContext* context, const strategy::StrategyRequest* request, strategy::StrategyResponse* response)
{
    return grpc::Status();
}
