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

	//- In trading, a position represents a trader's exposure to an asset
	// or financial instrument. It indicates whether a trader has 
	// bought (or is long on) or sold (or is short on) a certain 
	// quantity of an asset. Positions reflect the trader's stance
	// in the market and play a crucial role in determining potential
	// profits or losses as asset prices fluctuate.
	// -This class will manage all worked trading postions
	// -It can create new, cancel, replace, amend, an order... 

    template<typename T>
    using OrderList = std::unordered_map<std::string, std::unique_ptr<T>>;

	class DLL_CLASS_ORDERMANAGEMENT_EXPORTS PositionManager final
	{
	public:
		PositionManager();
		~PositionManager();

        ////////////////////// postion ////////////////////////////////////////

        // Add new long postion
        std::unique_ptr<BinanceNewOrder> CreateLongPositionOrder(
			const std::string& symbol,
			const binapi::double_type quality,
			const binapi::double_type refPrice);

        // Add new short postion
        std::unique_ptr<BinanceNewOrder> CreateShortPositionOrder(
			const std::string& symbol,
			const binapi::double_type quality,
			const binapi::double_type refPrice);

        /////////////////////// order ///////////////////////////////////////
  
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
        const OrderList<BinanceNewOrder>& GetOrders() const;

        const OrderList<BinanceCancelOrder>& GetCancelOrders() const;

        const OrderList<BinanceReplaceOrder>& GetReplaceOrders() const;
	private:
        std::unique_ptr<LibraryUtils::Logger> m_logger;
		std::unique_ptr<OrderCreator> m_orderCreator;
        OrderList<BinanceNewOrder> m_newOrders;
        OrderList<BinanceCancelOrder> m_cancelOrders;
        OrderList<BinanceReplaceOrder> m_replaceOrders;
        std::mutex m_mutex;
	};
};