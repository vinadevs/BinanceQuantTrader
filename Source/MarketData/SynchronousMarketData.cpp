/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "SynchronousMarketData.h"

using namespace MarketData;

SynchronousMarketData::SynchronousMarketData(const std::string& symbol)
	: m_symbol(symbol) {}

SingleMarketDataFeed* SynchronousMarketData::GetFeed(const FeedID id) const
{
	switch (id)
	{
	case FeedID::BEST_BID_PRICE:
		return m_individualBookTickerData.m_bestBidPrice.get();
	case FeedID::BEST_BID_QUANTITY:
		return m_individualBookTickerData.m_bestBidQty.get();
	case FeedID::BEST_ASK_PRICE:
		return m_individualBookTickerData.m_bestAskPrice.get();
	case FeedID::BEST_ASK_QUANTITY:
		return m_individualBookTickerData.m_bestAskQty.get();
	case FeedID::EVENT_TIME:
		return m_individualBookTickerData.m_eventTimeMs.get();
	default:
		break;
	}
	return nullptr;
}
