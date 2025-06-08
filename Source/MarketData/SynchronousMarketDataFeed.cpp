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
	m_syncMarketData->m_individualBookTickerData.m_updateId->SetData(book.u);
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

void MarketData::SynchronousMarketDataFeed::UpdateIndividualMarketTickerData(const binapi::ws::market_ticker_t& market)
{
	m_syncMarketData->m_individualMarketTickerData.m_eventTimeMs->SetData(market.E);
	m_syncMarketData->m_individualMarketTickerData.m_priceChange->SetData(market.p);
	m_syncMarketData->m_individualMarketTickerData.m_priceChangePercent->SetData(market.P);
	m_syncMarketData->m_individualMarketTickerData.m_weightedAvgPrice->SetData(market.w);
	m_syncMarketData->m_individualMarketTickerData.m_firstTradePrice->SetData(market.x);
	m_syncMarketData->m_individualMarketTickerData.m_lastPrice->SetData(market.c);
	m_syncMarketData->m_individualMarketTickerData.m_lastQuantity->SetData(market.Q);
	m_syncMarketData->m_individualMarketTickerData.m_bestBidPrice->SetData(market.b);
	m_syncMarketData->m_individualMarketTickerData.m_bestBidQty->SetData(market.B);
	m_syncMarketData->m_individualMarketTickerData.m_bestAskPrice->SetData(market.a);
	m_syncMarketData->m_individualMarketTickerData.m_bestAskQty->SetData(market.A);
	m_syncMarketData->m_individualMarketTickerData.m_openPrice->SetData(market.o);
	m_syncMarketData->m_individualMarketTickerData.m_highPrice->SetData(market.h);
	m_syncMarketData->m_individualMarketTickerData.m_lowPrice->SetData(market.l);
	m_syncMarketData->m_individualMarketTickerData.m_totalTradedBaseAssetVolume->SetData(market.v);
	m_syncMarketData->m_individualMarketTickerData.m_totalTradedQuoteAssetVolume->SetData(market.q);
	m_syncMarketData->m_individualMarketTickerData.m_statisticsOpenTimeMs->SetData(market.O);
	m_syncMarketData->m_individualMarketTickerData.m_statisticsCloseTimeMs->SetData(market.C);
	m_syncMarketData->m_individualMarketTickerData.m_firstTradeId->SetData(market.F);
	m_syncMarketData->m_individualMarketTickerData.m_lastTradeId->SetData(market.L);
	m_syncMarketData->m_individualMarketTickerData.m_totalNumberOfTrades->SetData(market.n);
}

void MarketData::SynchronousMarketDataFeed::UpdateAllMarketTickersData(const binapi::ws::markets_tickers_t& market)
{
	m_syncMarketData->m_allMarketTickerData.m_eventTimeMs->SetData(
		TimeUtils::GetCurrentTimeChrono(TimeUtils::TimeUnit::Milliseconds));
	
	TableMarketData<IndividualMarketTickerData> table;
	for (const auto& [key, ticker] : market.tickers)
	{
		IndividualMarketTickerData individualMarketTicker;
		individualMarketTicker.m_eventTimeMs->SetData(ticker.E);
		individualMarketTicker.m_priceChange->SetData(ticker.p);
		individualMarketTicker.m_priceChangePercent->SetData(ticker.P);
		individualMarketTicker.m_weightedAvgPrice->SetData(ticker.w);
		individualMarketTicker.m_firstTradePrice->SetData(ticker.x);
		individualMarketTicker.m_lastPrice->SetData(ticker.c);
		individualMarketTicker.m_lastQuantity->SetData(ticker.Q);
		individualMarketTicker.m_bestBidPrice->SetData(ticker.b);
		individualMarketTicker.m_bestBidQty->SetData(ticker.B);
		individualMarketTicker.m_bestAskPrice->SetData(ticker.a);
		individualMarketTicker.m_bestAskQty->SetData(ticker.A);
		table.emplace(key, individualMarketTicker);
	}

	m_syncMarketData->m_allMarketTickerData.m_allIndividualMarketTicker->SetData(table);
}

void MarketData::SynchronousMarketDataFeed::UpdateAllMiniTickersData(const binapi::ws::mini_tickers_t& mini)
{
	m_syncMarketData->m_allMiniTickerData.m_eventTimeMs->SetData(
		TimeUtils::GetCurrentTimeChrono(TimeUtils::TimeUnit::Milliseconds));
	TableMarketData<IndividualMiniTickerData> table;

	for (const auto& [key, ticker] : mini.tickers)
	{
		IndividualMiniTickerData individualMiniTicker;
		individualMiniTicker.m_eventTimeMs->SetData(ticker.E);
		individualMiniTicker.m_closePrice->SetData(ticker.c);
		individualMiniTicker.m_openPrice->SetData(ticker.o);
		individualMiniTicker.m_highPrice->SetData(ticker.h);
		individualMiniTicker.m_lowPrice->SetData(ticker.l);
		individualMiniTicker.m_totalTradedBaseAssetVolume->SetData(ticker.v);
		individualMiniTicker.m_totalTradedQuoteAssetVolume->SetData(ticker.q);
		table.emplace(key, individualMiniTicker);
	}
	m_syncMarketData->m_allMiniTickerData.m_tableMarketTicker->SetData(table);
}

void MarketData::SynchronousMarketDataFeed::UpdateAllPartDepthData(const binapi::ws::part_depths_t& depth)
{
	m_syncMarketData->m_allPartDepthData.m_eventTimeMs->SetData(
		TimeUtils::GetCurrentTimeChrono(TimeUtils::TimeUnit::Milliseconds));
	
	ArrayMarketData<DepthData> asks;
	ArrayMarketData<DepthData> bids;

	std::transform(std::begin(depth.a), std::end(depth.a), std::back_inserter(asks),
		[](const binapi::ws::part_depths_t::depth_t d) {
			DepthData ask;
			ask.m_price->SetData(d.price);
			ask.m_amount->SetData(d.amount);
			return ask;
		});
	std::transform(std::begin(depth.b), std::end(depth.b), std::back_inserter(bids),
		[](const binapi::ws::part_depths_t::depth_t d) {
			DepthData bid;
			bid.m_price->SetData(d.price);
			bid.m_amount->SetData(d.amount);
			return bid;
		});

	m_syncMarketData->m_allPartDepthData.m_bids->SetData(bids);
	m_syncMarketData->m_allPartDepthData.m_asks->SetData(asks);

}

void MarketData::SynchronousMarketDataFeed::UpdateAllDiffDepthData(const binapi::ws::diff_depths_t& depth)
{
	m_syncMarketData->m_allDiffDepthData.m_eventTimeMs->SetData(
		depth.E);

	ArrayMarketData<DepthData> asks;
	ArrayMarketData<DepthData> bids;

	std::transform(std::begin(depth.a), std::end(depth.a), std::back_inserter(asks),
		[](const binapi::ws::diff_depths_t::depth_t d) {
			DepthData ask;
			ask.m_price->SetData(d.price);
			ask.m_amount->SetData(d.amount);
			return ask;
		});
	std::transform(std::begin(depth.b), std::end(depth.b), std::back_inserter(bids),
		[](const binapi::ws::diff_depths_t::depth_t d) {
			DepthData bid;
			bid.m_price->SetData(d.price);
			bid.m_amount->SetData(d.amount);
			return bid;
		});

	m_syncMarketData->m_allDiffDepthData.m_firstUpdateID->SetData(depth.U);
	m_syncMarketData->m_allDiffDepthData.m_finalUpdateID->SetData(depth.u);
	m_syncMarketData->m_allDiffDepthData.m_bids->SetData(bids);
	m_syncMarketData->m_allDiffDepthData.m_asks->SetData(asks);
}

void MarketData::SynchronousMarketDataFeed::UpdateMiniTickerData(const binapi::ws::mini_ticker_t& mini)
{
	m_syncMarketData->m_individualMiniTickerData.m_eventTimeMs->SetData(mini.E);
	m_syncMarketData->m_individualMiniTickerData.m_closePrice->SetData(mini.c);
	m_syncMarketData->m_individualMiniTickerData.m_openPrice->SetData(mini.o);
	m_syncMarketData->m_individualMiniTickerData.m_highPrice->SetData(mini.h);
	m_syncMarketData->m_individualMiniTickerData.m_lowPrice->SetData(mini.l);
	m_syncMarketData->m_individualMiniTickerData.m_totalTradedBaseAssetVolume->SetData(mini.v);
	m_syncMarketData->m_individualMiniTickerData.m_totalTradedQuoteAssetVolume->SetData(mini.q);
}

void MarketData::SynchronousMarketDataFeed::UpdateAggregateTradeData(const binapi::ws::agg_trade_t& aggregate)
{
	m_syncMarketData->m_aggregateTradeData.m_eventTimeMs->SetData(aggregate.E);
	m_syncMarketData->m_aggregateTradeData.m_aggregateTradeId->SetData(aggregate.a);
	m_syncMarketData->m_aggregateTradeData.m_price->SetData(aggregate.p);
	m_syncMarketData->m_aggregateTradeData.m_quantity->SetData(aggregate.q);
	m_syncMarketData->m_aggregateTradeData.m_firstTradeId->SetData(aggregate.f);
	m_syncMarketData->m_aggregateTradeData.m_lastTradeId->SetData(aggregate.l);
	m_syncMarketData->m_aggregateTradeData.m_tradeTimeMs->SetData(aggregate.T);
	m_syncMarketData->m_aggregateTradeData.m_isBuyerMarketMaker->SetData(aggregate.m);
}

void MarketData::SynchronousMarketDataFeed::UpdateKlineCandleStickData(const binapi::ws::kline_t& kline)
{
	m_syncMarketData->m_klineCandleStickData.m_eventTimeMs->SetData(kline.E);
	m_syncMarketData->m_klineCandleStickData.m_klineStartTime->SetData(kline.t);
	m_syncMarketData->m_klineCandleStickData.m_klineCloseTime->SetData(kline.T);
	m_syncMarketData->m_klineCandleStickData.m_interval->SetData(kline.i);
	m_syncMarketData->m_klineCandleStickData.m_firstTradeId->SetData(kline.f);
	m_syncMarketData->m_klineCandleStickData.m_lastTradeId->SetData(kline.L);
	m_syncMarketData->m_klineCandleStickData.m_openPrice->SetData(kline.o);
	m_syncMarketData->m_klineCandleStickData.m_closePrice->SetData(kline.c);
	m_syncMarketData->m_klineCandleStickData.m_highPrice->SetData(kline.h);
	m_syncMarketData->m_klineCandleStickData.m_lowPrice->SetData(kline.l);
	m_syncMarketData->m_klineCandleStickData.m_baseAssetVolume->SetData(kline.v);
	m_syncMarketData->m_klineCandleStickData.m_numberOfTrades->SetData(kline.n);
	m_syncMarketData->m_klineCandleStickData.m_isThisKlineClosed->SetData(kline.x);
	m_syncMarketData->m_klineCandleStickData.m_quoteAssetVolume->SetData(kline.q);
	m_syncMarketData->m_klineCandleStickData.m_takerBuyBaseAssetVolume->SetData(kline.V);
	m_syncMarketData->m_klineCandleStickData.m_takerBuyQuoteAssetVolume->SetData(kline.Q);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SynchronousMarketData* SynchronousMarketDataFeed::GetSynchronousData()
{
	return m_syncMarketData.get();
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