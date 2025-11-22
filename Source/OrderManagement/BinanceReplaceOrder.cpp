/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "BinanceReplaceOrder.h"
#include "FieldLabels.h"
#include "TypeToStringUtils.h"

using namespace OrderManagement;
using namespace MiddlewareMQ;

BinanceReplaceOrder::BinanceReplaceOrder(
	const std::string& clientOrderId
	, const std::string& symbol
	, std::size_t orderId
	, const std::string& origClientOrderId)
	: Order(symbol, clientOrderId, MessageType::REPLACE),
	m_origClientOrderId(origClientOrderId) {
	SetOrderId(orderId);
}

BinanceReplaceOrder::~BinanceReplaceOrder() {}

BinanceReplaceOrderStatus BinanceReplaceOrder::GetOrderStatus() const
{
	return m_orderStatus;
}

std::string BinanceReplaceOrder::GetOrderStatusStr() const
{
	switch (m_orderStatus)
	{
	case BinanceReplaceOrderStatus::NEW:
		return "NEW";
	case BinanceReplaceOrderStatus::WAITING_FOR_REPLACE:
		return "WAITING_FOR_REPLACE";
	case BinanceReplaceOrderStatus::REPLACED:
		return "REPLACED";
	case BinanceReplaceOrderStatus::REJECTED:
		return "REJECTED";
	default:
		return "UNDEF";
	};
}

BinanceReplaceOrderStatus BinanceReplaceOrder::GetOrderStatusEnum(const std::string status)
{
	if (status == "NEW") return BinanceReplaceOrderStatus::NEW;
	else if (status == "WAITING_FOR_REPLACE") return BinanceReplaceOrderStatus::WAITING_FOR_REPLACE;
	else if (status == "REPLACED") return BinanceReplaceOrderStatus::REPLACED;
	else if (status == "REJECTED") return BinanceReplaceOrderStatus::REJECTED;
	return BinanceReplaceOrderStatus::UNDEF;
}

void BinanceReplaceOrder::SetOrderStatus(const BinanceReplaceOrderStatus status)
{
	m_orderStatus = status;
}

std::string BinanceReplaceOrder::ToStringOrder() const
{
	return "BinanceReplaceOrder("
		"Symbol: " + m_symbol +
		", UserAccountID: " + m_userAccountID +
		", OrderId: " + std::to_string(m_orderId) +
		", OrigClientOrderId: " + m_origClientOrderId +
		", ClientOrderId: " + m_clientOrderId +
		", OrderStatus: " + GetOrderStatusStr() +
		", UpdateTime: " + GetUpdateTimeStr() +
		")";
}

std::string BinanceReplaceOrder::ToStringAck() const
{
	return "BinanceReplaceOrderAck("
		"Symbol: " + m_symbol +
		", UserAccountID: " + m_userAccountID +
		", OrderId: " + std::to_string(m_orderId) +
		", OrigClientOrderId: " + m_origClientOrderId +
		", ClientOrderId: " + m_clientOrderId +
		", OrderStatus: " + GetOrderStatusStr() +
		", UpdateTime: " + GetUpdateTimeStr() +
		")";
}

BqtJsonMessage BinanceReplaceOrder::ToBqtJsonMessageOrder() const
{
	BqtJsonMessage message;
	message.AddPair(FieldLabels::MessageType, "BinanceReplaceOrder");
	message.AddPair(FieldLabels::UserAccountID, m_userAccountID);
	message.AddPair(FieldLabels::Symbol, m_symbol);
	message.AddPair(FieldLabels::OrderId, TypeToStringUtils::ToString(m_orderId));
	message.AddPair(FieldLabels::OrigClientOrderId, m_origClientOrderId);
	message.AddPair(FieldLabels::ClientOrderId, m_clientOrderId);
	message.AddPair(FieldLabels::OrderStatus, GetOrderStatusStr());
	message.AddPair(FieldLabels::UpdateTime, GetUpdateTimeStr());
	return message;
}

BqtJsonMessage BinanceReplaceOrder::ToBqtJsonMessageOrderAck() const
{
	BqtJsonMessage message;
	message.AddPair(FieldLabels::MessageType, "BinanceReplaceOrderAck");
	message.AddPair(FieldLabels::UserAccountID, m_userAccountID);
	message.AddPair(FieldLabels::Symbol, m_symbol);
	message.AddPair(FieldLabels::OrderId, TypeToStringUtils::ToString(m_orderId));
	message.AddPair(FieldLabels::OrigClientOrderId, m_origClientOrderId);
	message.AddPair(FieldLabels::ClientOrderId, m_clientOrderId);
	message.AddPair(FieldLabels::OrderStatus, GetOrderStatusStr());
	message.AddPair(FieldLabels::UpdateTime, GetUpdateTimeStr());
	return message;
}
