/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "MarketDataSubject.h"
#include "MarketDataObserver.h"

using namespace MarketData;

#define IMPLEMENT_NOTIFY_MARKET_DATA_CHANGE(callback) \
int updated{ 0 };  \
for (auto& obverver : m_marketDataObservers)  \
{  \
	if (obverver->callback(this, symbol))  \
	{ \
		updated++;  \
	} \
} \
return updated;

MarketDataSubject::~MarketDataSubject()
{
	ClearMarketDataObservers();
}

void MarketDataSubject::AttachMarketDataObserver(MarketDataObserver* observer)
{
	if (observer)
	{
		if(m_marketDataObservers.end() == std::find(m_marketDataObservers.begin(),
			m_marketDataObservers.end(), observer))
		{
			m_marketDataObservers.emplace_back(observer);
		}
	}
}

void MarketDataSubject::DettachMarketDataObserver(MarketDataObserver* observer)
{
	if (observer)
	{
		m_marketDataObservers.remove(observer);
	}
}

void MarketDataSubject::ClearMarketDataObservers() 
{
	m_marketDataObservers.clear();
}

int MarketDataSubject::NotifyIndividualBookTickerChange(const std::string& symbol)
{
	IMPLEMENT_NOTIFY_MARKET_DATA_CHANGE(OnIndividualBookTickerChange);
}

int MarketDataSubject::NotifyTradeChange(const std::string& symbol)
{
	IMPLEMENT_NOTIFY_MARKET_DATA_CHANGE(OnTradeChange);
}

int MarketDataSubject::NotifyIndividualMarketTickerChange(const std::string& symbol)
{
	IMPLEMENT_NOTIFY_MARKET_DATA_CHANGE(OnIndividualMarketTickerChange);
}

int MarketDataSubject::NotifyMiniTickerChange(const std::string& symbol)
{
	IMPLEMENT_NOTIFY_MARKET_DATA_CHANGE(OnMiniTickerChange);
}

int MarketData::MarketDataSubject::NotifyKlineCandleStickChange(const std::string& symbol)
{
	IMPLEMENT_NOTIFY_MARKET_DATA_CHANGE(OnKlineCandleStickChange);
}

int MarketDataSubject::NotifyAggregateTradeChange(const std::string& symbol)
{
	IMPLEMENT_NOTIFY_MARKET_DATA_CHANGE(OnAggregateTradeChange);
}

int MarketDataSubject::NotifyAllMarketTickersChange(const std::string& symbol)
{
	IMPLEMENT_NOTIFY_MARKET_DATA_CHANGE(OnAllMarketTickersChange);
}

int MarketDataSubject::NotifyAllMiniTickersChange(const std::string& symbol)
{
	IMPLEMENT_NOTIFY_MARKET_DATA_CHANGE(OnAllMiniTickersChange);
}

int MarketDataSubject::NotifyAllPartDepthChange(const std::string& symbol)
{
	IMPLEMENT_NOTIFY_MARKET_DATA_CHANGE(OnAllMarketDepthChange);
}

int MarketDataSubject::NotifyAllDiffDepthChange(const std::string& symbol)
{
	IMPLEMENT_NOTIFY_MARKET_DATA_CHANGE(OnAllMarketDepthDiffChange);
}
