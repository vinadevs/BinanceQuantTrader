/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "TestOrder.h"

using namespace OrderManagement;

TestOrder::TestOrder(const std::string& clientOrderId, 
					 const std::string& symbol,
					 const binapi::double_type quality,
					 const binapi::double_type referencePrice,
					 const TestOrderSide side)
	: Order(m_symbol, clientOrderId, BinanceOrderType::TEST),
	m_assetQty(quality),
	m_assetReferencePrice(referencePrice),
	m_orderSide(side)
{
}
