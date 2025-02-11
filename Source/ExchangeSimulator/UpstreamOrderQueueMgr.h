/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "LockFreeQueue.h"
#include "UpstreamOrder.h"

#include <memory>
#include <optional>
#include <string>
#include <atomic>

namespace LibraryUtils {
	class Logger;
};

namespace ExchangeSimulator {

	class UpstreamOrderQueue final : public MultipleThreads::LockFreeQueue<UpstreamOrder>
	{
	public:
		bool RemoveOrderWithClientOrderId(const std::string& clientOrderId) {
			Node* current = m_head.load();
			while (current->next.load() != nullptr) {
				Node* next = current->next.load();
				if (UpstreamOrderUtils::GetOrderClientId(next->data) == clientOrderId) {
					Node* nextNext = next->next.load();
					if (current->next.compare_exchange_weak(next, nextNext)) {
						delete next;
						return true;
					}
				}
				else {
					current = next;
				}
			}
			return false;
		}

		std::optional<UpstreamOrder> LookupOrderWithClientOrderId(const std::string& clientOrderId) const {
			Node* current = m_head.load()->next.load();  // Skip dummy node
			while (current != nullptr) {
				if (UpstreamOrderUtils::GetOrderClientId(current->data) == clientOrderId) {
					return current->data;  // Return the found element
				}
				current = current->next.load();
			}
			return std::nullopt;  // Element not found
		}

		bool ReplaceOrderWithClientOrderId(const std::string& clientOrderId, const UpstreamOrder& newOrder) {
			Node* current = m_head.load()->next.load();  // Skip the dummy node
			while (current != nullptr) {
				if (UpstreamOrderUtils::GetOrderClientId(current->data) == clientOrderId) {
					current->data = newOrder; // replace old order by new order
					return true;
				}
				current = current->next.load();
			}
			return false;
		}
	};

	class UpstreamOrderQueueMgr final
	{
	public:
		UpstreamOrderQueueMgr();
		~UpstreamOrderQueueMgr();

		void PushOrderToQueue(const UpstreamOrder& order);
		bool RemoveOrder(const std::string& clientOrderId);
		bool ReplaceOrder(const std::string& clientOrderId, const UpstreamOrder& order);
		const UpstreamOrder& LookupOrder(const std::string& clientOrderId) const;
		void ClearAll();
		bool HasNoOrders();
		UpstreamOrder GetNextOrder();
	private:
		std::unique_ptr<LibraryUtils::Logger> m_logger;
		// TODO: replace by LockFreeQueue.h
		UpstreamOrderQueue m_upstreamOrderQueue;
	};
};