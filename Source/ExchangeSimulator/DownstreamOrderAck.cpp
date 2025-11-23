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
#include "../LibraryUtils/TimeUtils.h"

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
	ack.AddTag(FieldLabels::SimulatorAck::AckType, FieldLabels::DownstreamAckTypes::ErrorOrderAck);
	ack.AddTag(FieldLabels::Symbol, symbol);
	ack.AddTag(FieldLabels::ClientOrderId, clientOrderId);
	ack.AddTag(FieldLabels::SimulatorAck::ExchangeText, errorMsg);
	ack.AddTag(FieldLabels::SimulatorAck::ErrorCode, ErrorOrderCodeStr);
	ack.AddTag(FieldLabels::OrderStatus, "REJECTED");
	ack.AddTag(FieldLabels::UpdateTime, std::to_string(TimeUtils::GetEpochTimeTickNow()));
	return ack;
}

DownstreamOrderAck AckUtils::CreateNewOrderAck(
	const BinanceNewOrder& order,
	const std::string& text)
{
	DownstreamOrderAck ack = order.ToBqtJsonMessageOrderAck();
	ack.AddTag(FieldLabels::SimulatorAck::AckType, FieldLabels::DownstreamAckTypes::NewOrderAck);
	ack.AddTag(FieldLabels::SimulatorAck::ExchangeText, text);
	return ack;
}

DownstreamOrderAck AckUtils::CreateFilledOrderAck(
	const BinanceNewOrder& order,
	const std::string& text)
{
	DownstreamOrderAck ack = order.ToBqtJsonMessageOrderAck();
	ack.AddTag(FieldLabels::SimulatorAck::AckType, FieldLabels::DownstreamAckTypes::FilledNewOrderAck);
	ack.AddTag(FieldLabels::SimulatorAck::ExchangeText, text);
	return ack;
}

DownstreamOrderAck AckUtils::CreateCancelOrderAck(
	const BinanceCancelOrder& order,
	const std::string& text)
{
	DownstreamOrderAck ack = order.ToBqtJsonMessageOrderAck();
	ack.AddTag(FieldLabels::SimulatorAck::AckType, FieldLabels::DownstreamAckTypes::CancelOrderAck);
	ack.AddTag(FieldLabels::SimulatorAck::ExchangeText, text);
	return ack;
}

DownstreamOrderAck AckUtils::CreateCancelledOrderAck(
	const BinanceCancelOrder& order,
	const std::string& text)
{
	DownstreamOrderAck ack = order.ToBqtJsonMessageOrderAck();
	ack.AddTag(FieldLabels::SimulatorAck::AckType, FieldLabels::DownstreamAckTypes::CancelledOrderAck);
	ack.AddTag(FieldLabels::SimulatorAck::ExchangeText, text);
	return ack;
}

DownstreamOrderAck AckUtils::CreateReplaceOrderAck(
	const BinanceReplaceOrder& order,
	const std::string& text)
{
	DownstreamOrderAck ack = order.ToBqtJsonMessageOrderAck();
	ack.AddTag(FieldLabels::SimulatorAck::AckType, FieldLabels::DownstreamAckTypes::ReplaceOrderAck);
	ack.AddTag(FieldLabels::SimulatorAck::ExchangeText, text);
	return ack;
}

DownstreamOrderAck AckUtils::CreateReplacedOrderAck(
	const BinanceReplaceOrder& order,
	const std::string& text)
{
	DownstreamOrderAck ack = order.ToBqtJsonMessageOrderAck();
	ack.AddTag(FieldLabels::SimulatorAck::AckType, FieldLabels::DownstreamAckTypes::ReplacedOrderAck);
	ack.AddTag(FieldLabels::SimulatorAck::ExchangeText, text);
	return ack;
}

DownstreamOrderAck AckUtils::CreateQueryOrderAck(
	const BinanceQueryOrder& order,
	const std::string& text)
{
	DownstreamOrderAck ack = order.ToBqtJsonMessageOrderAck();
	ack.AddTag(FieldLabels::SimulatorAck::AckType, FieldLabels::DownstreamAckTypes::QueryOrderAck);
	ack.AddTag(FieldLabels::SimulatorAck::ExchangeText, text);
	return ack;
}
