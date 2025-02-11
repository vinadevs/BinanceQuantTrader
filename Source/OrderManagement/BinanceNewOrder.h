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
#include "../MiddlewareMQ/MessageDelivery.h"
#include "../MiddlewareMQ/BqtJsonMessage.h"
#endif
#include "Order.h"

#include <string>

namespace OrderManagement {

    enum class BinanceNewOrderStatus : unsigned
    {
        UNDEF,
        NEW,
        WAITING_FOR_FILL,
        PRTIAL_FILLED,
        FULL_FILLED,
        CANCELLED,
        REPLACED,
    };

    class DLL_CLASS_ORDERMANAGEMENT_EXPORTS
        BinanceNewOrder final : public Order 
    {
    public:
        BinanceNewOrder() = default;

        BinanceNewOrder(
            const std::string& symbol,
            const binapi::e_side side,
            const binapi::e_type type,
            const binapi::e_time time,
            const double amount,
            const double price,
            const std::string& clientOrderId,
            const std::string& stopPrice,
            const std::string& icebergAmount);

        ~BinanceNewOrder() override;

        // Getters
        binapi::e_side GetSide() const { return m_side; }
        binapi::e_type GetType() const { return m_type; }
        binapi::e_time GetTime() const { return m_time; }
        std::string GetAmountStr() const { return std::to_string(m_amount); }
        double GetAmountDouble() const { return m_amount; }
        std::string GetFilledAmountStr() const { return std::to_string(m_filledAmount); }
        double GetFilledAmountDouble() const { return m_filledAmount; }
        std::string GetPriceStr() const { return std::to_string(m_price); }
        double GetFilledPriceDouble() const { return m_filledPrice; }
        std::string GetFilledPriceStr() const { return std::to_string(m_filledPrice); }
        double GetPriceDouble() const { return m_price; }
        const std::string& GetClientOrderId() const { return m_clientOrderId; }
        const std::string& GetStopPrice() const { return m_stopPrice; }
        double GetStopPriceDouble() const { return std::stod(m_stopPrice); }
        const std::string& GetIcebergAmount() const { return m_icebergAmount; }
        double GetIcebergAmountDouble() const { return std::stod(m_icebergAmount); }

        // Setters
        void SetSide(binapi::e_side side) { m_side = side; }
        void SetType(binapi::e_type type) { m_type = type; }
        void SetTime(binapi::e_time time) { m_time = time; }
        void SetAmount(const double amount) { m_amount = amount; }
        void SetFilledAmount(const double filledAmount) { m_filledAmount = filledAmount; }
        void SetPrice(const double price) { m_price = price; }
        void SetFilledPrice(const double filledPrice) { m_filledAmount = filledPrice; }
        void SetClientOrderId(const std::string& clientOrderId) { m_clientOrderId = clientOrderId; }
        void SetStopPrice(const std::string& stopPrice) { m_stopPrice = stopPrice; }
        void SetIcebergAmount(const std::string& icebergAmount) { m_icebergAmount = icebergAmount; }

        // Execution
        BinanceNewOrderStatus GetOrderStatus() const;
        std::string GetOrderStatusStr() const;
        void SetOrderStatus(const BinanceNewOrderStatus status);

        // Serialization
        std::string ToString() const;
        friend std::ostream& operator<<(std::ostream& os, const BinanceNewOrder& order)
        {
            os << order.ToString();
            return os;
        }

#if USE_TEST_TRADING
        // Simulator test
        MiddlewareMQ::BqtJsonMessage ToBqtJsonMessage();
        void SetExecutionResult(const MiddlewareMQ::MiddlewareMQResult& executionResult);
#else
    // Execution Result
    void SetExecutionResult(
        const binapi::rest::api::result<binapi::rest::new_order_resp_type>& executionResult);
#endif
    private:
        binapi::e_side m_side;
        binapi::e_type m_type;
        binapi::e_time m_time;
        double m_amount{ 0 };
        double m_filledAmount{ 0 };
        double m_price{ 0 };
        double m_filledPrice{ 0 };
        // m_clientOrderId is new order ID from us
        std::string m_clientOrderId;
        std::string m_stopPrice;
        std::string m_icebergAmount;

        BinanceNewOrderStatus m_orderStatus{ BinanceNewOrderStatus::NEW };

#if USE_TEST_TRADING
        MiddlewareMQ::MiddlewareMQResult m_executionResult;
#else
        // Execution Result
        binapi::rest::api::result<binapi::rest::new_order_resp_type> m_executionResult;
#endif
    };
};