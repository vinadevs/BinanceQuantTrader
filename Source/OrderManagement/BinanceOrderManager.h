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

#include "BinanceNewOrder.h"
#include "BinanceCancelOrder.h"

#include <string>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <vector>

namespace OrderManagement {
    class OrderCreator;
    class BinanceNewOrder;
    class BinanceCancelOrder;
    class BinanceReplaceOrder;
	class BinanceQueryOrder;
}

namespace LibraryUtils {
    class Logger;
};

namespace OrderManagement {

 /**
 * @class BinanceOrderManager
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
    using OrderList = std::unordered_map<std::string, std::unique_ptr<T>>;

    class DLL_CLASS_ORDERMANAGEMENT_EXPORTS BinanceOrderManager final
    {
    public:
        BinanceOrderManager();
        ~BinanceOrderManager();

        // Add new order
        void AddNewOrder(const std::string& clientOrderId, std::unique_ptr<BinanceNewOrder> order);

        // Add cancel order
        void AddCancelOrder(const std::string& clientOrderId, std::unique_ptr<BinanceCancelOrder> order);

        // Add replace order
        void AddReplaceOrder(const std::string& clientOrderId, std::unique_ptr<BinanceReplaceOrder> order);

		// Add query order
		void AddQueryOrder(const std::string& clientOrderId, std::unique_ptr<BinanceQueryOrder> order);

        // Remove an order by clientOrderId
        bool RemoveOrder(const std::string& clientOrderId);

		// Remove all new orders 
		bool RemoveAllNewOrders();

        // Remove new order by side
		bool RemoveNewOrderBySide(const binapi::e_side side);

        // Lookup an order by clientOrderId
        BinanceNewOrder* LookupOrder(const std::string& clientOrderId);

        // Remove a cancel order by clientOrderId
        bool RemoveCancelOrder(const std::string& clientOrderId);

        // Lookup a cancel order by clientOrderId
        BinanceCancelOrder* LookupCancelOrder(const std::string& clientOrderId);

        // Remove a replace order by clientOrderId
        bool RemoveReplaceOrder(const std::string& clientOrderId);

        // Lookup a replace order by clientOrderId
        BinanceReplaceOrder* LookupReplaceOrder(const std::string& clientOrderId);

		// Lookup a query order by clientOrderId
		BinanceQueryOrder* LookupQueryOrder(const std::string& clientOrderId);

		// Remove a query order by clientOrderId
		bool RemoveQueryOrder(const std::string& clientOrderId);

        // Clear all orders
        void ClearAll();

        // Clear all new orders
        void ClearAllNewOrders();

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
        const OrderList<BinanceNewOrder>& GetOrders() const;

        OrderList<BinanceNewOrder> GetOrdersByStatus(const BinanceNewOrderStatus orderStatus);

        const OrderList<BinanceCancelOrder>& GetCancelOrders() const;

        const OrderList<BinanceReplaceOrder>& GetReplaceOrders() const;

		const OrderList<BinanceQueryOrder>& GetQueryOrders() const;

		std::vector<BinanceNewOrder*> GetOrdersOfSymbol(const std::string& symbol);

		binapi::double_type GetWeightedAveragePrice(
			const std::string& symbol,
			const binapi::e_side side);
 
        BinanceNewOrder* UpdateNewOrderExecutionStatus(
           const std::string& clientOrderId,
           const std::string& symbol,
           const double filledAmount,
           const double filledPrice,
           const double remainingAmount,
           const std::size_t updateTime,
           const BinanceNewOrderStatus orderStatus);

        BinanceCancelOrder* UpdateOrderCancellingStatus(
           const std::string& clientOrderId,
           const std::string& symbol,
           const std::size_t updateTime,
           const BinanceCancelOrderStatus orderStatus);

    private:
        std::unique_ptr<LibraryUtils::Logger> m_logger;
        OrderList<BinanceNewOrder> m_newOrders;
        OrderList<BinanceCancelOrder> m_cancelOrders;
        OrderList<BinanceReplaceOrder> m_replaceOrders;
        OrderList<BinanceQueryOrder> m_queryOrders;
        std::mutex m_mutex;
    };
};