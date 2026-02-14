/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "../OrderManagement/ExternalParentOrder.h"
#include "../OrderManagement/ParentOrderManager.h"
#include "../LibraryUtils/Logger.h"

namespace TradingStrategies {

	class ExternalAdminRequest;

	// Base class for receiving external parent orders and admin requests
	// Inherit from this class to implement custom handling of external requests

	class ExternalRequestReceiver {
	public:

		// Override this method to handle external parent orders
		virtual void OnParentOrderReceived(const OrderManagement::ParentOrder* parentOrder) {
			DispatchExternalParentOrder(parentOrder);
		}

		// Override this method to handle external admin requests
		virtual void OnAdminRequestReceived(const ExternalAdminRequest* adminRequest) {}

		// Set the ParentOrderManager to manage parent orders
		void SetParentOrderManager(OrderManagement::ParentOrderManager* parentOrderManager) {
			m_parentOrderManager = parentOrderManager;
		}

		std::string GetUpstreamReceiver() const {
			return m_upstreamReceiver;
		}

		void SetUpstreamReceiver(const std::string& upstreamReceiver) {
			m_upstreamReceiver = upstreamReceiver;
		}

	protected:
		// List of parent order callbacks for derived class to implement to handle external parent order events
		virtual void OnNewExternalParentOrder(OrderManagement::NewExternalParentOrder* newOrder) {}
		virtual void OnCancelExternalParentOrder(OrderManagement::CancelExternalParentOrder* cancelOrder) {}
		virtual void OnAmendExternalParentOrder(OrderManagement::AmendExternalParentOrder* amendOrder) {}

	private:

		// Dispach external parent order to appropriate callback
		void DispatchExternalParentOrder(const OrderManagement::ParentOrder* parentOrder) {
			switch (parentOrder->GetParentOrderType()) {
			case OrderManagement::ParentOrderType::NEW: {

				std::shared_ptr<OrderManagement::NewExternalParentOrder> newOrder =
					std::make_shared<OrderManagement::NewExternalParentOrder>(
						parentOrder->GetSymbol(),
						parentOrder->GetParentOrderId(),
						parentOrder->GetPrice(),
						parentOrder->GetQuantity(),
						parentOrder->GetTargetStrategy(),
						parentOrder->GetStartTime(),
						parentOrder->GetEndTime(),
						parentOrder->GetSide(),
						parentOrder->GetChildOrderType());
				m_parentOrderManager->AddNewParentOrder(newOrder);
				m_logger->Info("Received New External Parent Order: " + newOrder->ToString());
				OnNewExternalParentOrder(newOrder.get());
				break;
			}
			case OrderManagement::ParentOrderType::CANCEL: {

				std::shared_ptr<OrderManagement::CancelExternalParentOrder> cancelOrder =
					std::make_shared<OrderManagement::CancelExternalParentOrder>(
						parentOrder->GetSymbol(),
						parentOrder->GetParentOrderId(),
						parentOrder->GetPrice(),
						parentOrder->GetQuantity(),
						parentOrder->GetTargetStrategy(),
						parentOrder->GetStartTime(),
						parentOrder->GetEndTime(),
						parentOrder->GetSide(),
						parentOrder->GetChildOrderType(),
						parentOrder->GetOriginalParentOrderId());
				m_parentOrderManager->AddCancelParentOrder(cancelOrder);
				m_logger->Info("Received Cancel External Parent Order: " + cancelOrder->ToString());
				OnCancelExternalParentOrder(cancelOrder.get());
				break;
			}
			case OrderManagement::ParentOrderType::AMEND: {

				std::shared_ptr<OrderManagement::AmendExternalParentOrder> amendOrder =
					std::make_shared<OrderManagement::AmendExternalParentOrder>(
						parentOrder->GetSymbol(),
						parentOrder->GetParentOrderId(),
						parentOrder->GetPrice(),
						parentOrder->GetQuantity(),
						parentOrder->GetTargetStrategy(),
						parentOrder->GetStartTime(),
						parentOrder->GetEndTime(),
						parentOrder->GetSide(),
						parentOrder->GetChildOrderType(),
						parentOrder->GetOriginalParentOrderId());
				m_parentOrderManager->AddAmendParentOrder(amendOrder);
				m_logger->Info("Received Amend External Parent Order: " + amendOrder->ToString());
				OnAmendExternalParentOrder(amendOrder.get());
				break;
			}
			default:
				throw std::invalid_argument("Invalid ParentOrderType in DispatchExternalParentOrder");
			}
		}

		OrderManagement::ParentOrderManager* m_parentOrderManager{ nullptr };
		std::unique_ptr<LibraryUtils::Logger> m_logger{ std::make_unique<LibraryUtils::Logger>("ExternalRequestReceiver") };
		std::string m_upstreamReceiver;
	};
};