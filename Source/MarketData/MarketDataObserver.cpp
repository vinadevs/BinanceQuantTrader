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