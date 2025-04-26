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

    // Class representing a Binance cancel order, which is used to cancel an existing order on Binance.
    // This class provides methods for managing the cancel order's lifecycle, serialization, and execution results.

    class DLL_CLASS_ORDERMANAGEMENT_EXPORTS
        BinanceCancelOrder final : public Order
    {
    public:
        BinanceCancelOrder() = default;

        BinanceCancelOrder(
              const std::string& clientOrderId
            , const std::string& symbol
            , std::size_t orderId
            , const std::string& origClientOrderId);

        ~BinanceCancelOrder() override;

        // Getters
        const std::string& GetOrigClientOrderId() const { return m_origClientOrderId; }

        // Execution
        BinanceCancelOrderStatus GetOrderStatus() const;
        std::string GetOrderStatusStr() const;
        static BinanceCancelOrderStatus GetOrderStatusEnum(const std::string status);
        void SetOrderStatus(const BinanceCancelOrderStatus status);

        // Serialization
        std::string ToStringOrder() const;
        std::string ToStringAck() const;
        friend std::ostream& operator<<(std::ostream& os, const BinanceCancelOrder& order)
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

        // Execution Result
        using Order::SetSendingOrderResult;
        void SetSendingOrderResult(
            const binapi::rest::api::result<binapi::rest::cancel_order_info_t>& sendingOrderResult)
        {
            m_sendingBinananceOrderResult = sendingOrderResult;
        }

        binapi::rest::api::result<binapi::rest::cancel_order_info_t> GetSendingBinananceOrderResult() const
        {
            return m_sendingBinananceOrderResult;
        }
    private:
        // m_origClientOrderId is original order ID from us
        std::string m_origClientOrderId;
        BinanceCancelOrderStatus m_orderStatus{ BinanceCancelOrderStatus::NEW };
        binapi::rest::api::result<binapi::rest::cancel_order_info_t> m_sendingBinananceOrderResult;
    };
};