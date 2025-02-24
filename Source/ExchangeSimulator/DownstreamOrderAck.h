/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include <variant>
#include <string>
#include <ostream>

#include "../MiddlewareMQ/BqtJsonMessage.h"
#include "UpstreamOrder.h"

namespace ExchangeSimulator {

	/////////////////////////////////////////////////////////////////////////////////////
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

	using DownstreamOrderAck = MiddlewareMQ::BqtJsonMessage;

	namespace AckUtils {

		static constexpr std::size_t ErrorOrderCode = -1;
		static std::string ErrorOrderCodeStr = "-1";

		DownstreamOrderAck CreateErrorRejectOrderAck(
			const std::string& symbol,
			const std::string& clientOrderId,
			const std::string& errorMsg);

		DownstreamOrderAck CreateNewOrderAck(const OrderManagement::BinanceNewOrder& order, const std::string& text);
		DownstreamOrderAck CreateFilledOrderAck(const OrderManagement::BinanceNewOrder& order, const std::string& text);

		DownstreamOrderAck CreateCancelOrderAck(const OrderManagement::BinanceCancelOrder& order, const std::string& text);
		DownstreamOrderAck CreateCancelledOrderAck(const OrderManagement::BinanceCancelOrder& order, const std::string& text);

		DownstreamOrderAck CreateReplaceOrderAck(const OrderManagement::BinanceReplaceOrder& order, const std::string& text);
		DownstreamOrderAck CreateReplacedOrderAck(const OrderManagement::BinanceReplaceOrder& order, const std::string& text);

		DownstreamOrderAck CreateQueryOrderAck(const OrderManagement::BinanceQueryOrder& order, const std::string& text);
	};
};