/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"
#include "../KernelTrading/double_type.h"
#include "../RestAPI/RestAPI.h"

#include <string>
#include <memory>
#include <unordered_map>
#include <mutex>

namespace OrderManagement {
    class OrderCreator;
    class BinanceNewOrder;
    class BinanceCancelOrder;
    class BinanceReplaceOrder;
}

namespace LibraryUtils {
    class Logger;
};

namespace OrderManagement {

 /**
 * @class BinanceWorkedOrderManager
 * @brief Manages created and sent orders for Binance trading.
 *
 * This class is responsible for tracking, updating, and managing orders that have been
 * created and sent to Binance. It maintains order states, processes execution reports,
 * and handles order lifecycle events such as fills, cancellations, and expirations.
 *
 * Responsibilities:
 * - Store and track active orders.
 * - Update order status based on exchange responses.
 * - Handle partial and full fills.
 * - Manage order cancellations and expirations.
 * - Provide an interface to query existing orders.
 */

    template<typename T>
    using OrderWorkedList = std::unordered_map<std::string, std::unique_ptr<T>>;

    class DLL_CLASS_ORDERMANAGEMENT_EXPORTS BinanceWorkedOrderManager final
    {
    public:
        BinanceWorkedOrderManager();
        ~BinanceWorkedOrderManager();

        // Add new order
        void AddNewOrder(const std::string& clientOrderId, std::unique_ptr<BinanceNewOrder> order);

        // Add cancel order
        void AddCancelOrder(const std::string& clientOrderId, std::unique_ptr<BinanceCancelOrder> order);

        // Add replace order
        void AddReplaceOrder(const std::string& clientOrderId, std::unique_ptr<BinanceReplaceOrder> order);

        // Remove an order by clientOrderId
        void RemoveNewOrder(const std::string& clientOrderId);

        // Lookup an order by clientOrderId
        BinanceNewOrder* LookupOrder(const std::string& clientOrderId);

        // Remove a cancel order by clientOrderId
        void RemoveCancelOrder(const std::string& clientOrderId);

        // Lookup a cancel order by clientOrderId
        BinanceCancelOrder* LookupCancelOrder(const std::string& clientOrderId);

        // Remove a replace order by clientOrderId
        void RemoveReplaceOrder(const std::string& clientOrderId);

        // Lookup a replace order by clientOrderId
        BinanceReplaceOrder* LookupReplaceOrder(const std::string& clientOrderId);

        // Clear all orders
        void ClearAll();

        // Clear all cancel orders
        void ClearAllCancelOrders();

        // Clear all replace orders
        void ClearAllReplaceOrders();

        // Show all stored orders
        void ShowOrders();

        // Show all cancel orders
        void ShowCancelOrders();

        // Show all replace orders
        void ShowReplaceOrders();

        // Getters to return the maps
        const OrderWorkedList<BinanceNewOrder>& GetWorkedOrders() const;

        const OrderWorkedList<BinanceCancelOrder>& GetWorkedCancelOrders() const;

        const OrderWorkedList<BinanceReplaceOrder>& GetWorkedReplaceOrders() const;
    private:
        std::unique_ptr<LibraryUtils::Logger> m_logger;
        OrderWorkedList<BinanceNewOrder> m_newWorkedOrders;
        OrderWorkedList<BinanceCancelOrder> m_workedCancelOrders;
        OrderWorkedList<BinanceReplaceOrder> m_workedReplaceOrders;
        std::mutex m_mutex;
    };
};