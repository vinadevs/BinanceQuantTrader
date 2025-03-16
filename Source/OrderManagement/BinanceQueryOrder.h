/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"

#include "../LibraryUtils/SourceBuildFlags.h"
#include "../KernelTrading/double_type.h"
#include "../RestAPI/RestAPI.h"
#include "../MiddlewareMQ/BqtJsonMessage.h"

#include "Order.h"

#include <string>

namespace OrderManagement {

    enum class BinanceQueryOrderStatus : unsigned
    {
        UNDEF,
        NEW,
        WAITING_FOR_QUERY,
        FILLED,
        REJECTED,
    };

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
        std::size_t GetOrderId() const { return m_orderId; }
        std::string GetOrderIdString() const { return std::to_string(m_orderId); }
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
        // m_orderId is order ID from Binance
        std::size_t m_orderId{ 0 };
        // m_origClientOrderId is original order ID from us
        std::string m_origClientOrderId;
        BinanceQueryOrderStatus m_orderStatus{ BinanceQueryOrderStatus::NEW };
    };
};