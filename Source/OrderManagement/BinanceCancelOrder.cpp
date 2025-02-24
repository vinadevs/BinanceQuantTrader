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

BinanceCancelOrder::BinanceCancelOrder(
	  const std::string& clientOrderId
	, const std::string& symbol
	, std::size_t orderId
	, const std::string& origClientOrderId)
	: Order(symbol, clientOrderId, BinanceOrderType::CANCEL),
	m_orderId(orderId),
	m_origClientOrderId(origClientOrderId) {}

BinanceCancelOrder::~BinanceCancelOrder() {}

#if USE_TEST_TRADING
MiddlewareMQ::BqtJsonMessage BinanceCancelOrder::ToBqtJsonMessage()
{
	MiddlewareMQ::BqtJsonMessage message;
	message.AddPair(FieldLabels::BinanceOrderType, "BinanceCancelOrder");
	message.AddPair(FieldLabels::Symbol, m_symbol);
	message.AddPair(FieldLabels::OrderId, TypeToStringUtils::ToString(m_orderId));
	message.AddPair(FieldLabels::OrigClientOrderId, m_origClientOrderId);
	message.AddPair(FieldLabels::ClientOrderId, m_clientOrderId);
	return message;
}
#endif
