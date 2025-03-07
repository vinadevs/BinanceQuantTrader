/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../LibraryUtils/TimeUtils.h"
#include "SynchronousMarketDataFeed.h"

#include <iostream>

using namespace MarketData;

SynchronousMarketDataFeed::SynchronousMarketDataFeed(const std::string& symbol) :
	m_syncMarketData {std::make_unique<SynchronousMarketData>(symbol)} {}

////////////////////////////////// BINANCE MARKET DATA FEEDS ///////////////////////////////////////////////////

void SynchronousMarketDataFeed::UpdateIndividualBookTickerData(const binapi::ws::book_ticker_t& book)
{
	m_syncMarketData->m_individualBookTickerData.m_eventTimeMs->SetData(
		TimeUtils::GetCurrentTimeChrono(TimeUtils::TimeUnit::Milliseconds));
	m_syncMarketData->m_individualBookTickerData.m_bestBidPrice->SetData(book.b);
	m_syncMarketData->m_individualBookTickerData.m_bestBidQty->SetData(book.B);
	m_syncMarketData->m_individualBookTickerData.m_bestAskPrice->SetData(book.a);
	m_syncMarketData->m_individualBookTickerData.m_bestAskQty->SetData(book.A);
}

void SynchronousMarketDataFeed::UpdateTradeData(const binapi::ws::trade_t& trade)
{
	m_syncMarketData->m_tradeData.m_eventTimeMs->SetData(trade.E);
	m_syncMarketData->m_tradeData.m_tradeId->SetData(trade.t);
	m_syncMarketData->m_tradeData.m_price->SetData(trade.p);
	m_syncMarketData->m_tradeData.m_quantity->SetData(trade.q);
	m_syncMarketData->m_tradeData.m_buyerOrderID->SetData(trade.b);
	m_syncMarketData->m_tradeData.m_sellerOrderID->SetData(trade.a);
	m_syncMarketData->m_tradeData.m_tradeTime->SetData(trade.T);
	m_syncMarketData->m_tradeData.m_isBuyerTheMarketMaker->SetData(trade.m);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SynchronousMarketData* SynchronousMarketDataFeed::GetSynchronousData()
{
	return m_syncMarketData.get();
}

SingleMarketDataFeed* SynchronousMarketDataFeed::GetSingleFeed(const FeedID id) const
{
	return m_syncMarketData->GetFeed(id);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool MarketDataFeedManager::CreateNewSynchronousFeed(const std::string& symbol)
{
	std::unique_lock<std::mutex> lock(m_threadSafeMutex);
	return m_smdFeedStorage.emplace(symbol, std::make_unique<SynchronousMarketDataFeed>(symbol)).second;
}

SynchronousMarketDataFeed* MarketDataFeedManager::GetSynchronousFeed(const std::string& symbol)
{
	std::unique_lock<std::mutex> lock(m_threadSafeMutex);
	if (const auto it = m_smdFeedStorage.find(symbol); it != m_smdFeedStorage.end())
	{
		return it->second.get();
	}
	return nullptr;
}

SingleMarketDataFeed* MarketDataFeedManager::GetSingleFeed(const std::string& symbol, const FeedID id)
{
	std::unique_lock<std::mutex> lock(m_threadSafeMutex);
	if (const auto it = m_smdFeedStorage.find(symbol); it != m_smdFeedStorage.end())
	{
		return it->second.get()->GetSingleFeed(id);
	}
	return nullptr;
}

bool MarketDataFeedManager::RemoveSynchronousFeed(const std::string& symbol)
{
	std::unique_lock<std::mutex> lock(m_threadSafeMutex);
	if (const auto it = m_smdFeedStorage.find(symbol); it != m_smdFeedStorage.end())
	{
		m_smdFeedStorage.erase(it);
		return true;
	}
	return false;
}
