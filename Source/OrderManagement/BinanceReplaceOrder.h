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

    enum class BinanceReplaceOrderStatus : unsigned
    {
        UNDEF,
        NEW,
        WAITING_FOR_REPLACE,
        FILLED,
        REJECTED,
    };

    class DLL_CLASS_ORDERMANAGEMENT_EXPORTS
        BinanceReplaceOrder final : public Order
    {
    public:
        BinanceReplaceOrder() = default;

        BinanceReplaceOrder(
            const std::string& clientOrderId
            , const std::string& symbol
            , std::size_t orderId
            , const std::string& origClientOrderId);

        ~BinanceReplaceOrder() override;

        // Getters
        const std::string& GetOrigClientOrderId() const { return m_origClientOrderId; }

        // Execution
        BinanceReplaceOrderStatus GetOrderStatus() const;
        std::string GetOrderStatusStr() const;
        static BinanceReplaceOrderStatus GetOrderStatusEnum(const std::string status);
        void SetOrderStatus(const BinanceReplaceOrderStatus status);

        // Serialization
        std::string ToStringOrder() const;
        std::string ToStringAck() const;
        friend std::ostream& operator<<(std::ostream& os, const BinanceReplaceOrder& order)
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
        BinanceReplaceOrderStatus m_orderStatus{ BinanceReplaceOrderStatus::NEW };
    };
};