/********************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be useddistributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"

#include "../RestAPI/RestAPI.h"
#include "../RestAPI/BinanceSpotApiGateWay.h"
#include "../OrderManagement/TypeToStringUtils.h"
#include "../OrderManagement/OrderDefinitions.h"

#include <string>

namespace QuantitativeModel {

// Struct: QuantOrderParammeter
// ----------------------------
// Represents the parameters required for placing or managing a quantitative trading order.
// This structure is typically used to encapsulate all configurable inputs for order execution
// in algorithmic or automated trading systems.

// It includes details such as:  
// - m_symbol: The trading pair symbol (e.g., BTCUSDT).  
// - m_side: The side of the order (e.g., buy or sell).  
// - m_type: The type of the order (e.g., market, limit).  
// - m_time: The time in force for the order (e.g., GTC, IOC).  
// - m_amount: The quantity of the asset to trade.  
// - m_price: The price at which the order is placed (applicable for limit orders).  
// - m_stopPrice: The stop price for stop-limit orders.  
// - m_icebergAmount: The amount for iceberg orders.
// - m_tradeType: The trading type (e.g., spot, margin).
// This struct is designed to be lightweight and final, ensuring no inheritance.

    struct DLL_CLASS_QUANTITATIVEMODEL_EXPORTS
           QuantOrderParammeter final {
           std::string m_symbol;
           binapi::e_side m_side;
           binapi::e_type m_type;
           binapi::e_time m_time;
           double m_amount{ 0 };
           double m_price{ 0 };
           double m_stopPrice{ 0 };
		   double m_icebergAmount{ 0 };
		   double m_leverageRatio{ 1.0 }; // Default leverage ratio is 1.0 (no leverage)
		   std::string m_stableCurrency; // Stable currency for trading, e.g., USDT, BUSD
		   std::string m_parentOrderId; // Parent order ID for tracking complex orders
           OrderManagement::BinanceNewOrderTradingType m_tradeType{ OrderManagement::BinanceNewOrderTradingType::UNDEF };

           QuantOrderParammeter() = default;

            // Converts the QuantOrderParammeter object into a human-readable string representation.
            // This method is useful for logging or debugging purposes.
            std::string AsString() const {
               std::ostringstream oss;
               oss << "QuantOrderParammeter { "
                   << "Symbol: " << m_symbol << ", "
                   << "Side: " << OrderManagement::TypeToStringUtils::ToString(m_side) << ", "
                   << "Type: " << OrderManagement::TypeToStringUtils::ToString(m_type) << ", "
                   << "Time: " << OrderManagement::TypeToStringUtils::ToString(m_time) << ", "
                   << "Amount: " << m_amount << ", "
                   << "Price: " << m_price << ", "
				   << "StopPrice: " << m_stopPrice << ", "
				   << "IcebergAmount: " << m_icebergAmount << ", "
				   << "LeverageRatio: " << m_leverageRatio << ", "
				   << "StableCurrency: " << m_stableCurrency << ", "
				   << "TradeType: " << OrderManagement::TypeToStringUtils::ToString(m_tradeType) << ", "
				   << "ParentOrderId: " << m_parentOrderId
                   << " }";
               return oss.str();
            }

           // Constructor
           QuantOrderParammeter(
               const std::string& symbol,
               binapi::e_side side,
               binapi::e_type type,
               binapi::e_time time,
               const double amount,
               const double price,
			   const double stopPrice,
			   const double icebergAmount,
			   const std::string& stableCurrency,
			   const OrderManagement::BinanceNewOrderTradingType tradeType,
               const std::string& parentOrderId)
               : m_symbol(symbol),
                 m_side(side),
                 m_type(type),
                 m_time(time),
                 m_amount(amount),
                 m_price(price),
                 m_stopPrice(stopPrice),
			     m_icebergAmount(icebergAmount),
			     m_stableCurrency(stableCurrency),
			     m_tradeType(tradeType),
			   m_parentOrderId(parentOrderId)
		   {
		   }
       };
};
