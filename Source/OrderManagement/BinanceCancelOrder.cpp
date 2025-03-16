/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "BinanceCancelOrder.h"
#include "FieldLabels.h"
#include "TypeToStringUtils.h"

using namespace OrderManagement;
using namespace MiddlewareMQ;

BinanceCancelOrder::BinanceCancelOrder(
	  const std::string& clientOrderId
	, const std::string& symbol
	, std::size_t orderId
	, const std::string& origClientOrderId)
	: Order(symbol, clientOrderId, MessageType::CANCEL),
	m_orderId(orderId),
	m_origClientOrderId(origClientOrderId) {}

BinanceCancelOrder::~BinanceCancelOrder() {}

BinanceCancelOrderStatus BinanceCancelOrder::GetOrderStatus() const
{
	return m_orderStatus;
}

std::string BinanceCancelOrder::GetOrderStatusStr() const
{
	switch (m_orderStatus)
	{
	case BinanceCancelOrderStatus::NEW:
		return "NEW";
	case BinanceCancelOrderStatus::WAITING_FOR_CANCEL:
		return "WAITING_FOR_CANCEL";
	case BinanceCancelOrderStatus::FILLED:
		return "FILLED";
	case BinanceCancelOrderStatus::REJECTED:
		return "REJECTED";
	default:
		return "UNDEF";
	};
}

BinanceCancelOrderStatus BinanceCancelOrder::GetOrderStatusEnum(const std::string status)
{
	if (status == "NEW") return BinanceCancelOrderStatus::NEW;
	else if (status == "WAITING_FOR_CANCEL") return BinanceCancelOrderStatus::WAITING_FOR_CANCEL;
	else if (status == "FILLED") return BinanceCancelOrderStatus::FILLED;
	else if (status == "REJECTED") return BinanceCancelOrderStatus::REJECTED;
	return BinanceCancelOrderStatus::UNDEF;
}

void BinanceCancelOrder::SetOrderStatus(const BinanceCancelOrderStatus status)
{
	m_orderStatus = status;
}

std::string BinanceCancelOrder::ToStringOrder() const
{
	return "BinanceCancelOrder("
		"Symbol: " + m_symbol +
		", UserAccountID: " + m_userAccountID +
		", OrderId: " + std::to_string(m_orderId) +
		", OrigClientOrderId: " + m_origClientOrderId +
		", ClientOrderId: " + m_clientOrderId +
		", OrderStatus: " + GetOrderStatusStr() +
		")";
}

std::string BinanceCancelOrder::ToStringAck() const
{
	return "BinanceCancelOrderAck("
		"Symbol: " + m_symbol +
		", UserAccountID: " + m_userAccountID +
		", OrderId: " + std::to_string(m_orderId) +
		", OrigClientOrderId: " + m_origClientOrderId +
		", ClientOrderId: " + m_clientOrderId +
		", OrderStatus: " + GetOrderStatusStr() +
		")";
}

BqtJsonMessage BinanceCancelOrder::ToBqtJsonMessageOrder() const
{
	BqtJsonMessage message;
	message.AddPair(FieldLabels::MessageType, "BinanceCancelOrder");
	message.AddPair(FieldLabels::Symbol, m_symbol);
	message.AddPair(FieldLabels::OrderId, TypeToStringUtils::ToString(m_orderId));
	message.AddPair(FieldLabels::OrigClientOrderId, m_origClientOrderId);
	message.AddPair(FieldLabels::ClientOrderId, m_clientOrderId);
	return message;
}

BqtJsonMessage BinanceCancelOrder::ToBqtJsonMessageOrderAck() const
{
	BqtJsonMessage message;
	message.AddPair(FieldLabels::MessageType, "BinanceCancelOrderAck");
	message.AddPair(FieldLabels::Symbol, m_symbol);
	message.AddPair(FieldLabels::OrderId, TypeToStringUtils::ToString(m_orderId));
	message.AddPair(FieldLabels::OrigClientOrderId, m_origClientOrderId);
	message.AddPair(FieldLabels::ClientOrderId, m_clientOrderId);
	return message;
}
