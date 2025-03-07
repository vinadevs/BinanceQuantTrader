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

BinanceQueryOrder::BinanceQueryOrder(
	  const std::string& clientOrderId
	, const std::string& symbol
	, std::size_t orderId
	, const std::string& origClientOrderId)
	: Order(symbol, clientOrderId, BinanceOrderType::QUERY),
	m_orderId(orderId),
	m_origClientOrderId(origClientOrderId) {}

BinanceQueryOrder::~BinanceQueryOrder() {}

#if USE_BACK_TEST_TRADING
MiddlewareMQ::BqtJsonMessage BinanceQueryOrder::ToBqtJsonMessage()
{
	MiddlewareMQ::BqtJsonMessage message;
	message.AddPair(FieldLabels::BinanceOrderType, "BinanceQueryOrder");
	message.AddPair(FieldLabels::Symbol, m_symbol);
	message.AddPair(FieldLabels::OrderId, TypeToStringUtils::ToString(m_orderId));
	message.AddPair(FieldLabels::OrigClientOrderId, m_origClientOrderId);
	message.AddPair(FieldLabels::ClientOrderId, m_clientOrderId);
	return message;
}
#endif