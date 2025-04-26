/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"

#include "Order.h"

#include <string>

namespace OrderManagement {

    // Enum representing the various statuses of a Binance order query.
    enum class BinanceQueryOrderStatus : unsigned
    {
        UNDEF, // Undefined status, used as a default or uninitialized state.
        NEW, // The order has been created but not yet processed.
        WAITING_FOR_QUERY, //The order is awaiting a query or response from the system.
        FILLED,// The order has been successfully completed.
        REJECTED, // The order has been rejected by the system.
    };

    // The BinanceQueryOrder class represents an order query in the Binance trading system.  
    // It includes functionality to manage order status, serialization, and simulator testing.  
    // The BinanceQueryOrderStatus enum defines the possible states of an order query.
    class DLL_CLASS_ORDERMANAGEMENT_EXPORTS
        BinanceQueryOrder final : public Order
    {
    public:
        BinanceQueryOrder() = default;

        BinanceQueryOrder(
            const std::string& clientOrderId
            , const std::string& symbol
            , std::size_t orderId
            , const std::string& origClientOrderId);

        ~BinanceQueryOrder() override;

        // Getters
        const std::string& GetOrigClientOrderId() const { return m_origClientOrderId; }

        // Execution
        BinanceQueryOrderStatus GetOrderStatus() const;
        std::string GetOrderStatusStr() const;
        static BinanceQueryOrderStatus GetOrderStatusEnum(const std::string status);
        void SetOrderStatus(const BinanceQueryOrderStatus status);

        // Serialization
        std::string ToStringOrder() const;
        std::string ToStringAck() const;
        friend std::ostream& operator<<(std::ostream& os, const BinanceQueryOrder& order)
        {
            os << order.ToStringOrder();
            return os;
        }

        // Setters
        void SetOrderId(const std::size_t orderId) { m_orderId = orderId; }
        void SetOrigClientOrderId(const std::string& origClientOrderId) { m_origClientOrderId = origClientOrderId; }

        // Simulator test
        MiddlewareMQ::BqtJsonMessage ToBqtJsonMessageOrder() const;
        MiddlewareMQ::BqtJsonMessage ToBqtJsonMessageOrderAck() const;
    private:
        // m_origClientOrderId is original order ID from us
        std::string m_origClientOrderId;
        BinanceQueryOrderStatus m_orderStatus{ BinanceQueryOrderStatus::NEW };
    };
};