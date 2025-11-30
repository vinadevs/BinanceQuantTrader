/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "BinanceQueryOrder.h"
#include "FieldLabels.h"
#include "TypeToStringUtils.h"

using namespace OrderManagement;
using namespace MiddlewareMQ;

BinanceQueryOrder::BinanceQueryOrder(
	const std::string& clientOrderId
	, const std::string& symbol
	, std::size_t orderId
	, const std::string& origClientOrderId)
	: Order(symbol, clientOrderId, MessageType::QUERY),
	m_origClientOrderId(origClientOrderId) {
	SetOrderId(orderId);
}

BinanceQueryOrder::~BinanceQueryOrder() {}

BinanceQueryOrderStatus BinanceQueryOrder::GetOrderStatus() const
{
	return m_orderStatus;
}

std::string BinanceQueryOrder::GetOrderStatusStr() const
{
	switch (m_orderStatus)
	{
	case BinanceQueryOrderStatus::NEW:
		return "NEW";
	case BinanceQueryOrderStatus::WAITING_FOR_QUERY:
		return "WAITING_FOR_QUERY";
	case BinanceQueryOrderStatus::QERRIED:
		return "QERRIED";
	case BinanceQueryOrderStatus::REJECTED:
		return "REJECTED";
	default:
		return "UNDEF";
	};
}

BinanceQueryOrderStatus BinanceQueryOrder::GetOrderStatusEnum(const std::string status)
{
	if (status == "NEW") return BinanceQueryOrderStatus::NEW;
	else if (status == "WAITING_FOR_QUERY") return BinanceQueryOrderStatus::WAITING_FOR_QUERY;
	else if (status == "QERRIED") return BinanceQueryOrderStatus::QERRIED;
	else if (status == "REJECTED") return BinanceQueryOrderStatus::REJECTED;
	return BinanceQueryOrderStatus::UNDEF;
}

void BinanceQueryOrder::SetOrderStatus(const BinanceQueryOrderStatus status)
{
	m_orderStatus = status;
}

std::string BinanceQueryOrder::ToStringOrder() const
{
	return "BinanceQueryOrder("
		"Symbol: " + m_symbol +
		", UserAccountID: " + m_userAccountID +
		", OrderId: " + std::to_string(m_orderId) +
		", OrigClientOrderId: " + m_origClientOrderId +
		", ClientOrderId: " + m_clientOrderId +
		", OrderStatus: " + GetOrderStatusStr() +
		", UpdateTime: " + GetUpdateTimeStr() +
		")";
}

std::string BinanceQueryOrder::ToStringAck() const
{
	return "BinanceQueryOrderAck("
		"Symbol: " + m_symbol +
		", UserAccountID: " + m_userAccountID +
		", OrderId: " + std::to_string(m_orderId) +
		", OrigClientOrderId: " + m_origClientOrderId +
		", ClientOrderId: " + m_clientOrderId +
		", OrderStatus: " + GetOrderStatusStr() +
		", UpdateTime: " + GetUpdateTimeStr() +
		")";
}

BqtJsonMessage BinanceQueryOrder::ToBqtJsonMessageOrder() const
{
	BqtJsonMessage message;
	message.AddTag(FieldLabels::MessageType, "BinanceQueryOrder");
	message.AddTag(FieldLabels::UserAccountID, m_userAccountID);
	message.AddTag(FieldLabels::Symbol, m_symbol);
	message.AddTag(FieldLabels::OrderId, TypeToStringUtils::ToString(m_orderId));
	message.AddTag(FieldLabels::OrigClientOrderId, m_origClientOrderId);
	message.AddTag(FieldLabels::ClientOrderId, m_clientOrderId);
	message.AddTag(FieldLabels::OrderStatus, GetOrderStatusStr());
	message.AddTag(FieldLabels::UpdateTime, GetUpdateTimeStr());
	return message;
}

BqtJsonMessage BinanceQueryOrder::ToBqtJsonMessageOrderAck() const
{
	BqtJsonMessage message;
	message.AddTag(FieldLabels::MessageType, "BinanceQueryOrderAck");
	message.AddTag(FieldLabels::UserAccountID, m_userAccountID);
	message.AddTag(FieldLabels::Symbol, m_symbol);
	message.AddTag(FieldLabels::OrderId, TypeToStringUtils::ToString(m_orderId));
	message.AddTag(FieldLabels::OrigClientOrderId, m_origClientOrderId);
	message.AddTag(FieldLabels::ClientOrderId, m_clientOrderId);
	message.AddTag(FieldLabels::OrderStatus, GetOrderStatusStr());
	message.AddTag(FieldLabels::UpdateTime, GetUpdateTimeStr());
	return message;
}
