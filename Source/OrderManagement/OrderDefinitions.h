/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

namespace OrderManagement {

    // Type of order message which will send to the exchange/downstream/simulator
    enum class MessageType : unsigned
    {
        UNDEF,
        NEW,
        CANCEL,
        REPLACE,
        QUERY,
        TEST,
    };

    // Represents the status of a Binance new order
    enum class BinanceNewOrderStatus : unsigned
    {
        UNDEF,               // Undefined status
        NEW,                 // Order has been created but not yet processed
        WAITING_FOR_FILL,    // Order is waiting to be filled
        PARTIAL_FILLED,      // Order has been partially filled
		FULL_FILLED,         // Order has been fully filled, closed future position
		REJECTED,            // Order was rejected
		EXPIRED,             // Order has expired
		CANCELED,            // Order was canceled
		LIQUIDATED,          // Order was liquidated (future trading context)
		MARGIN_CALL,         // Order was margin called (future trading context)
        SKIPPED,             // Order was skipped
    };

    // Represents the type of Exchange Connectivity for a new order
    enum class ExchangeConnectivityType : unsigned
    {
        UNDEF,  // Undefined trade type
        REAL,   // Real trade
        TEST,   // Test trade
    };

    // Represents the trading type for a new order
    enum class BinanceNewOrderTradingType : unsigned
    {
        UNDEF, // Undefined trading type
        SPOT, // Spot trading
        FUTURE, // Future trading
    };

    // Enum representing the various statuses of a Binance cancel order.
    // Each status corresponds to a specific stage in the lifecycle of the cancel order.
    enum class BinanceCancelOrderStatus : unsigned
    {
        UNDEF,               // Undefined status, used as a default or uninitialized state.
        NEW,                 // The cancel order has been created but not yet processed.
        WAITING_FOR_CANCEL,  // The cancel order is waiting to be executed.
        FILLED,              // The cancel order has been successfully executed.
        REJECTED,            // The cancel order was rejected.
    };

    // Enum representing the various statuses of a Binance order query.
    enum class BinanceQueryOrderStatus : unsigned
    {
        UNDEF, // Undefined status, used as a default or uninitialized state.
        NEW, // The order has been created but not yet processed.
        WAITING_FOR_QUERY, //The order is awaiting a query or response from the system.
        FILLED,// The order has been successfully completed.
        REJECTED, // The order has been rejected by the system.
    };

    // Enum representing the various statuses of a Binance replace order.    
    enum class BinanceReplaceOrderStatus : unsigned
    {
        UNDEF,  // Undefined status, used as a default or uninitialized state. 
        NEW, // The order has been created and is new.  
        WAITING_FOR_REPLACE, // The order is waiting to be replaced.  
        FILLED, // The order has been completely filled.  
        REJECTED, // The order has been rejected.  
    };

    enum class TestOrderSide : unsigned
    {
        BUY_SIDE,
        SELL_SIDE,
        UNKNOWN,
    };
} // namespace OrderManagement