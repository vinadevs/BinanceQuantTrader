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
#include <unordered_set>

namespace OrderManagement {

    /**
    * @class BinanceNewOrder
    * @brief Represents a new order in the Binance trading system.
    *
    * This class encapsulates the details of a new order, including its type, side,
    * price, quantity, and other relevant attributes. It provides methods to
    * retrieve and modify these attributes, as well as to serialize the order
    * for communication with external systems.
    *
    * Key Features:
    * - Supports various order types (e.g., limit, market) and trade types (e.g., real, test).
    * - Tracks the status of the order (e.g., new, partially filled, fully filled).
    * - Provides serialization methods for order and acknowledgment messages.
    * - Includes support for simulator testing and execution results.
    */

    class DLL_CLASS_ORDERMANAGEMENT_EXPORTS
        BinanceNewOrder final : public Order 
    {
    public:
        BinanceNewOrder() = default;

        BinanceNewOrder(
            const std::string& clientOrderId,
            const std::string& symbol,
            const binapi::e_side side,
            const binapi::e_type type,
            const binapi::e_time time,
            const double amount,
            const double price,
            const double stopPrice,
            const double icebergAmount,
			const BinanceNewOrderTradingType tradingType,
            const ExchangeConnectivityType exchangeConnectivityType);

        ~BinanceNewOrder() override;

        // Getters
        binapi::e_side GetSide() const { return m_side; }
        binapi::e_type GetType() const { return m_type; }
        binapi::e_time GetTimeInForce() const { return m_timeInForce; }
        std::string GetAmountStr() const { return std::to_string(m_amount); }
        double GetAmount() const { return m_amount; }
        std::string GetFilledAmountStr() const { return std::to_string(m_filledAmount); }
        double GetFilledAmount() const { return m_filledAmount; }
        std::string GetPriceStr() const { return std::to_string(m_price); }
        double GetFilledPrice() const { return m_filledPrice; }
        std::string GetFilledPriceStr() const { return std::to_string(m_filledPrice); }
        double GetPrice() const { return m_price; } 
        std::string GetStopPriceStr() const { return std::to_string(m_stopPrice); }
        double GetStopPrice() const { return m_stopPrice; }
        std::string GetIcebergAmountStr() const { return std::to_string(m_icebergAmount); }
        double GetIcebergAmount() const { return m_icebergAmount; }
        std::string GetOrigQuoteOrderQuantityStr() const { return std::to_string(m_origQuoteOrderQuantity); }
        double GetOrigQuoteOrderQuantity() const { return m_origQuoteOrderQuantity; }
        std::string GetCumulativeQuoteQuantityStr() const { return std::to_string(m_cumulativeQuoteQuantity); }
        double GetCumulativeQuoteQuantity() const { return m_cumulativeQuoteQuantity; }
        std::string GetRemainingAmountStr() const { return std::to_string(m_remainingAmount); }
        double GetRemainingAmount() const { return m_remainingAmount; }
		double GetFutureLeverageRatio() const { return m_futureOrderLeverageRatio; }

        // Setters
        void SetSide(binapi::e_side side) { m_side = side; }
        void SetType(binapi::e_type type) { m_type = type; }
        void SetTimeInForce(binapi::e_time timeInForce) { m_timeInForce = timeInForce; }
        void SetAmount(const double amount) { m_amount = amount; }
        void SetFilledAmount(const double filledAmount) { m_filledAmount = filledAmount; }
        void SetPrice(const double price) { m_price = price; }
        void SetFilledPrice(const double filledPrice) { m_filledPrice = filledPrice; }
        void SetStopPrice(const double stopPrice) { m_stopPrice = stopPrice; }
        void SetIcebergAmount(const double icebergAmount) { m_icebergAmount = icebergAmount; }
        void SetOrigQuoteOrderQuantity(const double origQuoteOrderQuantity) { m_origQuoteOrderQuantity = origQuoteOrderQuantity; }
        void SetCumulativeQuoteQuantity(const double cumulativeQuoteQuantity) { m_cumulativeQuoteQuantity = cumulativeQuoteQuantity; }
        void SetRemainingAmount(const double remainingAmount) { m_remainingAmount = remainingAmount; }
        void SetFutureLeverageRatio(const double leverageRatio) { m_futureOrderLeverageRatio = leverageRatio; }

        // Execution
        BinanceNewOrderStatus GetOrderStatus() const;
        std::string GetOrderStatusStr() const;
        static BinanceNewOrderStatus GetOrderStatusEnum(const std::string status);
        void SetOrderStatus(const BinanceNewOrderStatus status);
		void SetOrderTradingType(const BinanceNewOrderTradingType tradingType) { m_orderTradingType = tradingType; }
		BinanceNewOrderTradingType GetOrderTradingType() const { return m_orderTradingType; }
        std::string GetOrderTradingTypeStr() const;
		binapi::e_type GetOrderType() const { return m_orderType; }
		void SetOrderType(const binapi::e_type orderType) { m_orderType = orderType; }

        // Serialization
        std::string ToStringOrder() const;
        std::string ToStringAck() const;
        friend std::ostream& operator<<(std::ostream& os, const BinanceNewOrder& order)
        {
            os << order.ToStringOrder();
            return os;
        }

        // Simulator test
        MiddlewareMQ::BqtJsonMessage ToBqtJsonMessageOrder() const;
        MiddlewareMQ::BqtJsonMessage ToBqtJsonMessageOrderAck () const;
        // Execution Result
        using Order::SetSendingOrderResult;
        void SetSendingOrderResult(
            const binapi::rest::api::result<binapi::rest::new_order_resp_type>& sendingOrderResult)
        {
            m_sendingBinananceOrderResult = sendingOrderResult;
        }

        binapi::rest::api::result<binapi::rest::new_order_resp_type> GetSendingBinananceOrderResult() const 
        {
            return m_sendingBinananceOrderResult;
        }
    private:
        binapi::e_side m_side { binapi::e_side::buy}; // buy side or sell side
        binapi::e_type m_type { binapi::e_type::limit }; // order type: limit or market,..
        // https://www.binance.com/en/support/faq/detail/5d3fa5e5709f47e0b5f186b350da1655?hl=en
        binapi::e_time m_timeInForce{ binapi::e_time::GTC }; // Time In Force of Order at the Exchange
        double m_amount{ 0 }; // Original Quantity
        double m_filledAmount{ 0 }; // Executed Quantity
        double m_remainingAmount{ 0 }; // Partial Fill
        /* m_price specifies the maximum price a buyer is willing to pay(for buy orders) or 
        the minimum price a seller is willing to accept(for sell orders).*/
        double m_price{ 0 }; // limit price to execute order
        double m_filledPrice{ 0 }; // executed price of order at the Exchange
        double m_origQuoteOrderQuantity{ 0 }; // m_price * m_amount (Order PreTrade Volume)
        double m_cumulativeQuoteQuantity{ 0 }; // m_filledPrice * m_filledAmount  (Order Traded Volume)
        /* m_stopPrice acts as a trigger : When the market price reaches m_stopPrice, the order converts into a market
        order(for stop orders) or a limit order(for stop - limit orders).*/
        double m_stopPrice{ 0 };
		// m_icebergAmount is used for iceberg orders, which allows traders to hide the true size of their orders.
        double m_icebergAmount{ 0 };
		// leverage ratio for the future order
		double m_futureOrderLeverageRatio{ 0.0 };
        // If we trade with a basket of orders
        std::unordered_set<std::string> m_clientOrderIdList;
        BinanceNewOrderStatus m_orderStatus{ BinanceNewOrderStatus::NEW };
		BinanceNewOrderTradingType m_orderTradingType{ BinanceNewOrderTradingType::UNDEF };
        binapi::rest::api::result<binapi::rest::new_order_resp_type> m_sendingBinananceOrderResult;
        binapi::e_type m_orderType{ binapi::e_type::limit };
    };
};