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
		const std::string ErrorOrderAck = "ErrorOrderAck";
		const std::string NewOrderAck = "NewOrderAck";
		const std::string CancelOrderAck = "CancelOrderAck";
		const std::string ReplaceOrderAck = "ReplaceOrderAck";
		const std::string QueryOrderAck = "QueryOrderAck";
		const std::string FilledNewOrderAck = "FilledNewOrderAck";
		const std::string CancelledOrderAck = "CancelledOrderAck";
		const std::string ReplacedOrderAck = "ReplacedOrderAck";
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