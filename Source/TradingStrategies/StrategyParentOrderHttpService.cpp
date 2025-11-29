#include "pch.h"
#include "StrategyParentOrderHttpService.h"

#include "../OrderManagement/ParentOrder.h"

#include <memory>

using namespace TradingStrategies;
using namespace LibraryUtils;
using namespace OrderManagement;

StrategyParentOrderHttpService::StrategyParentOrderHttpService()
{
}

StrategyParentOrderHttpService::~StrategyParentOrderHttpService()
{
}

void StrategyParentOrderHttpService::AddTargetStrategy(ExternalRequestReceiver* recevier)
{
	m_targetStrategy = recevier;
}

grpc::Status StrategyParentOrderHttpService::SubmitParentOrder(
    grpc::ServerContext* context,
    const strategy::StrategyParentOrderRequest* request,
    strategy::StrategyParentOrderResponse* response)
{
    // Extract request fields
    const std::string symbol = request->symbol();
    const double      price = request->price();
    const double      quantity = request->quantity();
    const std::string strategyName = request->strategy();
    const std::string startTime = request->start_time();
    const std::string endTime = request->end_time();
    const std::string side = request->side();
    const std::string childOrderType = request->child_order_type();
    const std::string parentOrderType = request->parent_order_type();
    const std::string parentOrderId = request->parent_order_id();
    const std::string originalParentOrderId = request->original_parent_order_id();

    // Validate
    if (parentOrderType != "NEW" &&
        parentOrderType != "CANCEL" &&
        parentOrderType != "AMEND")
    {
        response->set_status(strategy::ORDER_FAILED);
        response->set_error_message("Order Invalid: parent_order_type must be NEW, CANCEL, or AMEND");
        return grpc::Status::OK;
    }

    if (parentOrderId.empty())
    {
        response->set_status(strategy::ORDER_FAILED);
        response->set_error_message("Order Invalid: parent_order_id is required");
        return grpc::Status::OK;
    }

    if (parentOrderType != "NEW")
    {
        if (originalParentOrderId.empty())
        {
            response->set_status(strategy::ORDER_FAILED);
            response->set_error_message("Order Invalid: original_parent_order_id is required for CANCEL or AMEND");
            return grpc::Status::OK;
        }
    }

    // Forward to strategy/algo engine
    try
    {
        if (m_targetStrategy) {
			if (m_targetStrategy->GetUpstreamReceiver() != strategyName) {
				response->set_status(strategy::ORDER_FAILED);
				response->set_error_message("Algo Engine: current strategy in Algo Engine is="
                    + m_targetStrategy->GetUpstreamReceiver() + ", pls use correct strategy name");
				return grpc::Status::OK;
			}

			std::unique_ptr<ParentOrder> parentOrder = std::make_unique<ParentOrder>(
				symbol,
				parentOrderId,
				price,
				quantity,
				strategyName,
				startTime,
				endTime,
                ParentOrder::ConvertStringToParentOrderSide(side),
                ParentOrder::ConvertStringToChildOrderType(childOrderType),
                ParentOrder::ConvertStringToParentOrderType(parentOrderType),
				originalParentOrderId);
            m_targetStrategy->OnParentOrderReceived(parentOrder.get());
        }
        else {
            response->set_error_message("Algo Engine: No target strategy configured to receive parent orders.");
        }
    }
    catch (const std::exception& e)
    {
        response->set_status(strategy::ORDER_FAILED);
        response->set_error_message(e.what());
        return grpc::Status::OK;
    }
    response->set_status(strategy::ORDER_SUCCESS);
    return grpc::Status::OK;
}
