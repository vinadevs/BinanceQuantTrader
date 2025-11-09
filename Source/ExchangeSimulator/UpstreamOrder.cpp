/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "UpstreamOrder.h"

#define GET_BINANCE_ORDER_ATTRIBUTE(order, getter) \
if (std::holds_alternative<OrderManagement::BinanceNewOrder>(order)) \
{ \
	return std::get<OrderManagement::BinanceNewOrder>(order).getter(); \
} \
else if (std::holds_alternative<OrderManagement::BinanceCancelOrder>(order)) \
{ \
	return std::get<OrderManagement::BinanceCancelOrder>(order).getter(); \
} \
else if (std::holds_alternative<OrderManagement::BinanceReplaceOrder>(order)) \
{ \
	return std::get<OrderManagement::BinanceReplaceOrder>(order).getter(); \
} \
else if (std::holds_alternative<OrderManagement::BinanceQueryOrder>(order)) \
{ \
	return std::get<OrderManagement::BinanceQueryOrder>(order).getter(); \
} \
else \
{ \
	throw std::runtime_error("UpstreamOrder: unsupported UpstreamOrder type"); \
}

const std::string& ExchangeSimulator::UpstreamOrderUtils::GetOrderClientId(const UpstreamOrder& order)
{
	GET_BINANCE_ORDER_ATTRIBUTE(order, GetClientOrderId);
}

const std::string& ExchangeSimulator::UpstreamOrderUtils::GetOrderSymbol(const UpstreamOrder& order)
{
	GET_BINANCE_ORDER_ATTRIBUTE(order, GetSymbol);
}

const std::string& ExchangeSimulator::UpstreamOrderUtils::GetOrderMessageTypeName(const UpstreamOrder& order)
{
	GET_BINANCE_ORDER_ATTRIBUTE(order, GetOrderMessageTypeStr);
}
