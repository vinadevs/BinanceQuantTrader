/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "DownstreamOrderAck.h"
#include "../OrderManagement/FieldLabels.h"
#include "../OrderManagement/TypeToStringUtils.h"

using namespace ExchangeSimulator;
using namespace AckUtils;
using namespace OrderManagement;
using namespace MiddlewareMQ;

DownstreamOrderAck AckUtils::CreateErrorOrderAck(
	const std::string& symbol,
	const std::string& clientOrderId,
	const std::string& errorMsg)
{
	DownstreamOrderAck ack;
	ack.AddPair(FieldLabels::SimulatorAck::AckType, DownstreamAckTypes::ErrorOrderAck);
	ack.AddPair(FieldLabels::Symbol, symbol);
	ack.AddPair(FieldLabels::ClientOrderId, clientOrderId);
	ack.AddPair(FieldLabels::SimulatorAck::ErrorMessage, errorMsg);
	ack.AddPair(FieldLabels::SimulatorAck::ErrorCode, ErrorOrderCodeStr);
	return ack;
}

DownstreamOrderAck AckUtils::CreateNewOrderAck(const OrderManagement::BinanceNewOrder& order)
{
	return DownstreamOrderAck();
}

DownstreamOrderAck AckUtils::CreateFilledOrderAck(const OrderManagement::BinanceNewOrder& order)
{
	return DownstreamOrderAck();
}

DownstreamOrderAck AckUtils::CreateCancelOrderAck(const OrderManagement::BinanceCancelOrder& order)
{
	return DownstreamOrderAck();
}

DownstreamOrderAck AckUtils::CreateCancelledOrderAck(const OrderManagement::BinanceCancelOrder& order)
{
	return DownstreamOrderAck();
}

DownstreamOrderAck AckUtils::CreateReplaceOrderAck(const OrderManagement::BinanceReplaceOrder& order)
{
	return DownstreamOrderAck();
}

DownstreamOrderAck AckUtils::CreateReplacedOrderAck(const OrderManagement::BinanceReplaceOrder& order)
{
	return DownstreamOrderAck();
}

DownstreamOrderAck AckUtils::CreateQueryOrderAck(const OrderManagement::BinanceQueryOrder& order)
{
	return DownstreamOrderAck();
}
