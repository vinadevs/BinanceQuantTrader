/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"
#include "ExternalParentOrder.h"

#include <string>
#include <memory>
#include <unordered_map>

namespace OrderManagement {

	class ParentOrderManager final {
	public:
		void AddNewParentOrder(const std::shared_ptr<NewExternalParentOrder>& newOrder) {
			m_newParentOrders[newOrder->GetParentOrderId()] = newOrder;
		}

		void AddCancelParentOrder(const std::shared_ptr<CancelExternalParentOrder>& cancelOrder) {
			m_cancelParentOrders[cancelOrder->GetParentOrderId()] = cancelOrder;
		}

		void AddAmendParentOrder(const std::shared_ptr<AmendExternalParentOrder>& amendOrder) {
			m_amendParentOrders[amendOrder->GetParentOrderId()] = amendOrder;
		}

		std::shared_ptr<NewExternalParentOrder> GetNewParentOrder(const std::string& parentOrderId) const {
			auto it = m_newParentOrders.find(parentOrderId);
			if (it != m_newParentOrders.end()) {
				return it->second;
			}
			return nullptr;
		}

		std::shared_ptr<CancelExternalParentOrder> GetCancelParentOrder(const std::string& parentOrderId) const {
			auto it = m_cancelParentOrders.find(parentOrderId);
			if (it != m_cancelParentOrders.end()) {
				return it->second;
			}
			return nullptr;
		}

		std::shared_ptr<AmendExternalParentOrder> GetAmendParentOrder(const std::string& parentOrderId) const {
			auto it = m_amendParentOrders.find(parentOrderId);
			if (it != m_amendParentOrders.end()) {
				return it->second;
			}
			return nullptr;
		}
	private:
		std::unordered_map<std::string, std::shared_ptr<NewExternalParentOrder>> m_newParentOrders;
		std::unordered_map<std::string, std::shared_ptr<CancelExternalParentOrder>> m_cancelParentOrders;
		std::unordered_map<std::string, std::shared_ptr<AmendExternalParentOrder>> m_amendParentOrders;
	};
};