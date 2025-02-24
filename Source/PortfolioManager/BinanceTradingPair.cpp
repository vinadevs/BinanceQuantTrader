/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "BinanceTradingPair.h"
#include "../MarketData/RealTimeMarketData.h"

using namespace PortfolioManager;
using namespace MarketData;

BinanceTradingPair::BinanceTradingPair(const std::string& tradingPair,
	const RealTimeMarketData* marketData,
	const BinanceBalance& balance)
	: m_tradingPairName(tradingPair), m_marketData(marketData), m_balance(balance) {}

void BinanceTradingPair::UpdateTradingPair(const BinanceBalance& balance)
{
	m_balance = balance;
}

binapi::double_type BinanceTradingPair::GetTradingPairValue(const std::string& tradingPair) const
{
	const auto* syncedData = m_marketData->GetFeedHandler()->GetSynchronousMarketData(tradingPair);
	if (syncedData)
	{
		const auto* data = syncedData->GetFeed(FeedID::BEST_ASK_PRICE); // TODO: replace BEST_ASK_PRICE  by MARKET_PRICE
		return data->GetDoubleMultiprecisionData() * m_balance.free; // current asset value = market price * asset quantity
	}
	throw std::runtime_error("BinanceTradingPair: sycnchronous feed does not exit with symbol=" + tradingPair);
}

binapi::double_type BinanceTradingPair::GetQuantity() const
{
	return m_balance.free;
}
