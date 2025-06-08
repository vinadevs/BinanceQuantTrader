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
		const std::string MessageType = "MessageType";
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
		const std::string RemainingAmount = "RemainingAmount";
		const std::string ClientOrderId = "ClientOrderId";
		const std::string StopPrice = "StopPrice";
		const std::string IcebergAmount = "IcebergAmount";
		const std::string OrderStatus = "OrderStatus";
		const std::string TradingType = "TradingType";
		const std::string OrigQuoteOrderQuantity = "OrigQuoteOrderQuantity";
		const std::string CummulativeQuoteQty = "CummulativeQuoteQty";
		const std::string UpdateTime = "UpdateTime";

		// Simulator Ack
		namespace SimulatorAck {
			const std::string AckType = "DownstreamAckType";
			const std::string ErrorCode = "ErrorCode";
			const std::string ExchangeText = "ExchangeText";
		};

		namespace DownstreamAckTypes {
			// Exchange acknowledgment message types for order processing.
			// These strings represent different types of acknowledgments received 
			// from the exchange in response to order-related actions.
			const std::string ErrorOrderAck = "ErrorOrderAck";       // Acknowledgment for an erroneous order.
			const std::string NewOrderAck = "NewOrderAck";           // Confirmation of a successfully placed new order.
			const std::string CancelOrderAck = "CancelOrderAck";     // Acknowledgment of a request to cancel an order.
			const std::string ReplaceOrderAck = "ReplaceOrderAck";   // Acknowledgment of a request to modify an existing order.
			const std::string QueryOrderAck = "QueryOrderAck";       // Response to an order status query.
			const std::string FilledNewOrderAck = "FilledNewOrderAck"; // Confirmation that a new order has been fully filled.
			const std::string CancelledOrderAck = "CancelledOrderAck"; // Confirmation that an order has been successfully canceled.
			const std::string ReplacedOrderAck = "ReplacedOrderAck"; // Confirmation that an order has been successfully modified.
			const std::string QueriedOrderAck = "QueriedOrderAck"; // Confirmation that an order has been successfully queried.
		};
	};
};