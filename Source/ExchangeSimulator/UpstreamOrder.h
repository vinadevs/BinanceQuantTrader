/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "../OrderManagement/BinanceNewOrder.h"
#include "../OrderManagement/BinanceCancelOrder.h"
#include "../OrderManagement/BinanceReplaceOrder.h"
#include "../OrderManagement/BinanceQueryOrder.h"

#include <variant>
#include <string>

namespace ExchangeSimulator {

	using UpstreamOrder = std::variant<
		 OrderManagement::BinanceNewOrder,
		OrderManagement::BinanceCancelOrder,
		OrderManagement::BinanceReplaceOrder,
		OrderManagement::BinanceQueryOrder
	>;

	namespace UpstreamOrderUtils {

		const std::string& GetOrderClientId(const UpstreamOrder& order);

		const std::string& GetOrderSymbol(const UpstreamOrder& order);

		const std::string& GetOrderTypeName(const UpstreamOrder& order);
	};
};