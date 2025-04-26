/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "MarketDataObserver.h"

using namespace MarketData;

bool MarketDataObserver::OnIndividualBookTickerChange(MarketDataSubject* marketDataconst, const std::string& symbol)
{
	return true;
}

bool MarketDataObserver::OnTradeChange(MarketDataSubject* marketDataconst, const std::string& symbol)
{
	return true;
}

bool MarketDataObserver::OnIndividualMarketTickerChange(MarketDataSubject* marketDataconst, const std::string& symbol)
{
	return true;
}

bool MarketDataObserver::OnMiniTickerChange(MarketDataSubject* marketDataconst, const std::string& symbol)
{
	return true;
}

bool MarketDataObserver::OnAggregateTradeChange(MarketDataSubject* marketDataconst, const std::string& symbol)
{
	return true;
}

bool MarketData::MarketDataObserver::OnKlineCandleStickChange(MarketDataSubject* marketData, const std::string& symbol)
{
	return true;
}

bool MarketDataObserver::OnAllMarketTickersChange(MarketDataSubject* marketDataconst, const std::string& symbol)
{
	return true;
}

bool MarketDataObserver::OnAllMiniTickersChange(MarketDataSubject* marketDataconst, const std::string& symbol)
{
	return true;
}

bool MarketDataObserver::OnAllMarketDepthChange(MarketDataSubject* marketDataconst, const std::string& symbol)
{
	return true;
}

bool MarketDataObserver::OnAllMarketDepthDiffChange(MarketDataSubject* marketData, const std::string& symbol)
{
	return true;
}
