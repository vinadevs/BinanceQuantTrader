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

#if USE_TEST_TRADING
#include "../MiddlewareMQ/BqtJsonMessage.h"
#endif

#include "Order.h"

#include <string>

namespace OrderManagement {
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
        std::size_t GetOrderId() const { return m_orderId; }
        const std::string& GetOrigClientOrderId() const { return m_origClientOrderId; }
        const std::string& GetClientOrderId() const { return m_clientOrderId; }

        // Setters
        void SetOrderId(std::size_t orderId) { m_orderId = orderId; }
        void SetOrigClientOrderId(const std::string& origClientOrderId) { m_origClientOrderId = origClientOrderId; }
        void SetClientOrderId(const std::string& clientOrderId) { m_clientOrderId = clientOrderId; }

#if USE_TEST_TRADING
        // Simulator test
        MiddlewareMQ::BqtJsonMessage ToBqtJsonMessage();
#endif
    private:
        // m_orderId is order ID from Binance
        std::size_t m_orderId {0};
        // m_origClientOrderId is original order ID from us
        std::string m_origClientOrderId;
        // m_clientOrderId is new replace order ID from us
        std::string m_clientOrderId;
    };
};