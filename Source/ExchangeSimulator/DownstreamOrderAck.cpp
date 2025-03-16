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

DownstreamOrderAck AckUtils::CreateErrorRejectOrderAck(
	const std::string& symbol,
	const std::string& clientOrderId,
	const std::string& errorMsg)
{
	DownstreamOrderAck ack;
	ack.AddPair(FieldLabels::SimulatorAck::AckType, FieldLabels::DownstreamAckTypes::ErrorOrderAck);
	ack.AddPair(FieldLabels::Symbol, symbol);
	ack.AddPair(FieldLabels::ClientOrderId, clientOrderId);
	ack.AddPair(FieldLabels::SimulatorAck::ExchangeText, errorMsg);
	ack.AddPair(FieldLabels::SimulatorAck::ErrorCode, ErrorOrderCodeStr);
	return ack;
}

DownstreamOrderAck AckUtils::CreateNewOrderAck(
	const BinanceNewOrder& order,
	const std::string& text)
{
	DownstreamOrderAck ack = order.ToBqtJsonMessageAck();
	ack.AddPair(FieldLabels::SimulatorAck::AckType, FieldLabels::DownstreamAckTypes::NewOrderAck);
	ack.AddPair(FieldLabels::SimulatorAck::ExchangeText, text);
	return ack;
}

DownstreamOrderAck AckUtils::CreateFilledOrderAck(
	const BinanceNewOrder& order,
	const std::string& text)
{
	DownstreamOrderAck ack = order.ToBqtJsonMessageAck();
	ack.AddPair(FieldLabels::SimulatorAck::AckType, FieldLabels::DownstreamAckTypes::FilledNewOrderAck);
	ack.AddPair(FieldLabels::SimulatorAck::ExchangeText, text);
	return ack;
}

DownstreamOrderAck AckUtils::CreateCancelOrderAck(
	const BinanceCancelOrder& order,
	const std::string& text)
{
	return DownstreamOrderAck();
}

DownstreamOrderAck AckUtils::CreateCancelledOrderAck(
	const BinanceCancelOrder& order,
	const std::string& text)
{
	return DownstreamOrderAck();
}

DownstreamOrderAck AckUtils::CreateReplaceOrderAck(
	const BinanceReplaceOrder& order,
	const std::string& text)
{
	return DownstreamOrderAck();
}

DownstreamOrderAck AckUtils::CreateReplacedOrderAck(
	const BinanceReplaceOrder& order,
	const std::string& text)
{
	return DownstreamOrderAck();
}

DownstreamOrderAck AckUtils::CreateQueryOrderAck(
	const BinanceQueryOrder& order,
	const std::string& text)
{
	return DownstreamOrderAck();
}
