/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include <string>

/**
 * @class FieldLabels
 * @brief Contains all field tags used for constructing order messages sent to the exchange simulator.
 *
 * The `FieldLabels` class defines constants representing various fields required to format
 * order messages correctly when interacting with the exchange simulator. These fields are
 * essential for specifying attributes such as order ID, symbol, price, quantity, and order type.
 *
 * This class ensures consistency in the field names across different components of the system
 * and minimizes errors when constructing or parsing messages.
 *
 * Example usage:
 * @code
 * std::string message = FieldLabels::OrderID + ":12345," +
 *                       FieldLabels::Symbol + ":BTCUSD," +
 *                       FieldLabels::LimitPrice + ":50000.00";
 * @endcode
 *
 * @note This class only provides field labels; it does not handle message formatting or validation.
 */

namespace OrderManagement {
	namespace FieldLabels {

		// Header
		const std::string BinanceOrderType = "BinanceOrderType";
		// User Account
		const std::string UserAccountID = "UserAccountID";
		// Order
		const std::string Symbol = "Symbol";
		const std::string OrderId = "OrderId";
		const std::string OrigClientOrderId = "OrigClientOrderId";
		const std::string Side = "Side";
		const std::string Type = "Type";
		const std::string TimeInForce = "TimeInForce";
		const std::string Amount = "Amount";
		const std::string LimitPrice = "LimitPrice";
		const std::string FilledAmount = "FilledAmount";
		const std::string FilledPrice = "FilledPrice";
		const std::string ClientOrderId = "ClientOrderId";
		const std::string StopPrice = "StopPrice";
		const std::string IcebergAmount = "IcebergAmount";
		const std::string OrderStatus = "OrderStatus";
		const std::string OrigQuoteOrderQuantity = "OrigQuoteOrderQuantity";
		const std::string CummulativeQuoteQty = "CummulativeQuoteQty";
		const std::string UpdateTime = "UpdateTime";

		// Simulator Ack
		namespace SimulatorAck {
			const std::string AckType = "DownstreamAckType";
			const std::string ErrorCode = "ErrorCode";
			const std::string ExchangeText = "ExchangeText";
		};
	};
};