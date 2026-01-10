/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include "HistoricalMarketDataFeedHandler.h"

#include <iostream>

using namespace MarketData;

HistoricalMarketDataFeedHandler::HistoricalMarketDataFeedHandler()
	: m_synchronousFeedMgr{ std::make_unique<MarketDataFeedManager>() } {
}

void HistoricalMarketDataFeedHandler::RegisterObserver(MarketDataObserver* observer)
{
	MarketDataSubject::AttachMarketDataObserver(observer);
}

void HistoricalMarketDataFeedHandler::UnregisterObserver(MarketDataObserver* observer)
{
	MarketDataSubject::DettachMarketDataObserver(observer);
}

bool HistoricalMarketDataFeedHandler::CreateNewMarketDataFeed(const std::string& symbol)
{
	return m_synchronousFeedMgr->CreateNewSynchronousFeed(symbol);
}

void HistoricalMarketDataFeedHandler::RemoveMarketDataFeed(const std::string& symbol)
{
	m_synchronousFeedMgr->RemoveSynchronousFeed(symbol);
}

SynchronousMarketData* HistoricalMarketDataFeedHandler::GetSynchronousMarketData(const std::string& symbol)
{
	if (auto* feed = m_synchronousFeedMgr->GetSynchronousFeed(symbol))
	{
		return feed->GetSynchronousData();
	}
	return nullptr;
}
