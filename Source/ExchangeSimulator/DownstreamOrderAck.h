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