/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "UpstreamOrder.h"

#include <list>
#include <unordered_map>
#include <mutex>
#include <optional>
#include <string>

namespace ExchangeSimulator {

 /**
 * @class OrderQueue
 * @brief Manages a queue of trading orders in a financial system.
 *
 * The OrderQueue class maintains a prioritized queue of buy/sell orders,
 * ensuring efficient order matching, insertion, and removal. It supports
 * operations such as adding new orders, removing executed/canceled orders,
 * and retrieving the highest-priority order based on trading rules.
 *
 * Backbone algorithsms:
  -Why std::list + std::unordered_map ?
  std::list allows fast insertion and deletion at any position in O(1) time with an iterator.
  std::unordered_map allows fast lookup by clientOrderId in O(1) time.
  -The map will store the clientOrderId → iterator(list node), so finding and modifying elements is efficient.
 */

class UpstreamOrderQueue {
private:
    struct OrderNode final {
        std::string clientOrderId;
        UpstreamOrder order;
    };

    std::list<OrderNode> m_queue; // Doubly linked list for fast deletion & insertion
    std::unordered_map<std::string, typename std::list<OrderNode>::iterator> m_map; // clientOrderId → Iterator
    mutable std::mutex m_mutex; // Mutex for thread safety

public:
    // Enqueue: Add element to the back
    void EnqueueOrder(const std::string& clientOrderId, const UpstreamOrder& order) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_map.find(clientOrderId) != m_map.end()) {
            throw std::runtime_error("UpstreamOrderQueue: " + clientOrderId + " already exists.");
        }
        m_queue.push_back({ clientOrderId, order });
        m_map[clientOrderId] = --m_queue.end(); // Store iterator to the last inserted element
    }

    // Dequeue: Remove from the front and return the order
    std::optional<UpstreamOrder> DequeueOrder() {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_queue.empty()) {
            return std::nullopt;
        }
        std::string clientOrderId = m_queue.front().clientOrderId;
        const auto order = m_queue.front().order;
        m_map.erase(clientOrderId);
        m_queue.pop_front();
        return order;
    }

    // Delete by clientOrderId
    bool RemoveOrderWithClientOrderId(const std::string& clientOrderId) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_map.find(clientOrderId);
        if (it != m_map.end()) {
            m_queue.erase(it->second);
            m_map.erase(it);
            return true;
        }
        return false;
    }

    // Replace order by clientOrderId
    bool ReplaceOrderWithClientOrderId(const std::string& clientOrderId, const UpstreamOrder& neworder) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_map.find(clientOrderId);
        if (it != m_map.end()) {
            it->second->order = neworder;
            return true;
        }
        return false;
    }

    // Lookup by clientOrderId
    std::optional<UpstreamOrder> LookupOrderWithClientOrderId(const std::string& clientOrderId) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_map.find(clientOrderId);
        if (it != m_map.end()) {
            return it->second->order;
        }
        return std::nullopt;
    }

    // Check if queue is empty
    bool IsEmpty() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    // Get the number of elements in the queue
    size_t GetSize() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.size();
    }

    // Clear all elements from the queue
    void Clear() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.clear();
        m_map.clear();
    }
};
};
