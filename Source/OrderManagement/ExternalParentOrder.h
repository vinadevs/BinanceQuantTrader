/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"
#include "ParentOrder.h"

#include <string>

namespace OrderManagement {

	// External Parent Order Types

	class NewExternalParentOrder final : public ParentOrder
	{
	public:
		NewExternalParentOrder(
			const std::string& symbol,
			const std::string& parentOrderId,
			const double price,
			const double quantity,
			const std::string& targetStrategy,
			const std::string& startTime,
			const std::string& endTime,
			const ParentOrderSide side,
			const ChildOrderType childOrderType)
			: ParentOrder(
				symbol,
				parentOrderId,
				price,
				quantity,
				targetStrategy,
				startTime,
				endTime,
				side,
				childOrderType,
				ParentOrderType::NEW)
		{
		}

		std::string ToString() const {
			return "NewExternalParentOrder: { Symbol: " + GetSymbol() +
				", ParentOrderId: " + GetParentOrderId() +
				", Price: " + std::to_string(GetPrice()) +
				", Quantity: " + std::to_string(GetQuantity()) +
				", TargetStrategy: " + GetTargetStrategy() +
				", StartTime: " + GetStartTime() +
				", EndTime: " + GetEndTime() +
				", Side: " + (GetSide() == ParentOrderSide::BUY ? "BUY" : "SELL") +
				", ChildOrderType: " + std::to_string(static_cast<unsigned>(GetChildOrderType())) +
				" }";
		}
	};

	class CancelExternalParentOrder final : public ParentOrder
	{
	public:
		CancelExternalParentOrder(
			const std::string& symbol,
			const std::string& parentOrderId,
			const double price,
			const double quantity,
			const std::string& targetStrategy,
			const std::string& startTime,
			const std::string& endTime,
			const ParentOrderSide side,
			const ChildOrderType childOrderType,
			const std::string& originalParentOrderId)
			: ParentOrder(
				symbol,
				parentOrderId,
				price,
				quantity,
				targetStrategy,
				startTime,
				endTime,
				side,
				childOrderType,
				ParentOrderType::CANCEL,
				originalParentOrderId)
		{
			if (originalParentOrderId.empty()) {
				throw std::invalid_argument("Original Parent Order ID must be provided for CancelExternalParentOrder");
			}
		}

		std::string ToString() const {
			return "CancelExternalParentOrder: { Symbol: " + GetSymbol() +
				", ParentOrderId: " + GetParentOrderId() +
				", Price: " + std::to_string(GetPrice()) +
				", Quantity: " + std::to_string(GetQuantity()) +
				", TargetStrategy: " + GetTargetStrategy() +
				", StartTime: " + GetStartTime() +
				", EndTime: " + GetEndTime() +
				", Side: " + (GetSide() == ParentOrderSide::BUY ? "BUY" : "SELL") +
				", ChildOrderType: " + std::to_string(static_cast<unsigned>(GetChildOrderType())) +
				", OriginalParentOrderId: " + GetOriginalParentOrderId() +
				" }";
		}
	};

	class AmendExternalParentOrder final : public ParentOrder
	{
	public:
		AmendExternalParentOrder(
			const std::string& symbol,
			const std::string& parentOrderId,
			const double price,
			const double quantity,
			const std::string& targetStrategy,
			const std::string& startTime,
			const std::string& endTime,
			const ParentOrderSide side,
			const ChildOrderType childOrderType,
			const std::string& originalParentOrderId)
			: ParentOrder(
				symbol,
				parentOrderId,
				price,
				quantity,
				targetStrategy,
				startTime,
				endTime,
				side,
				childOrderType,
				ParentOrderType::AMEND,
				originalParentOrderId)
		{
			if (originalParentOrderId.empty()) {
				throw std::invalid_argument("Original Parent Order ID must be provided for AmendExternalParentOrder");
			}
		}

		std::string ToString() const {
			return "AmendExternalParentOrder: { Symbol: " + GetSymbol() +
				", ParentOrderId: " + GetParentOrderId() +
				", Price: " + std::to_string(GetPrice()) +
				", Quantity: " + std::to_string(GetQuantity()) +
				", TargetStrategy: " + GetTargetStrategy() +
				", StartTime: " + GetStartTime() +
				", EndTime: " + GetEndTime() +
				", Side: " + (GetSide() == ParentOrderSide::BUY ? "BUY" : "SELL") +
				", ChildOrderType: " + std::to_string(static_cast<unsigned>(GetChildOrderType())) +
				", OriginalParentOrderId: " + GetOriginalParentOrderId() +
				" }";
		};
	};
};