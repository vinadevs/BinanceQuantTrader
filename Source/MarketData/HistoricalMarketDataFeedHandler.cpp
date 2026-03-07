/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include "HistoricalMarketDataFeedHandler.h"
#include "../LibraryUtils/TimeUtils.h"
#include "../LibraryUtils/Logger.h"

#include <iostream>

using namespace MarketData;

HistoricalMarketDataFeedHandler::HistoricalMarketDataFeedHandler(LibraryUtils::Logger* logger)
	: m_synchronousFeedMgr{ std::make_unique<MarketDataFeedManager>() },
      m_logger(logger) {
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

void HistoricalMarketDataFeedHandler::HandleDataItem(
	const std::string& symbol, 
	const std::string& subscriptionType,
	const std::unordered_map<std::string, std::string>& dataItem)
{
	if (auto* feed = m_synchronousFeedMgr->GetSynchronousFeed(symbol))
	{
		if (subscriptionType == "IndividualBookTickerData")
		{
			const auto individualBookTickerData = BuildIndividualBookTickerData(symbol, dataItem);
			feed->UpdateIndividualBookTickerData(individualBookTickerData);
			MarketDataSubject::NotifyIndividualBookTickerChange(symbol);
		}
		else if (subscriptionType == "TradeData")
		{
			const auto tradeData = BuildTradeData(symbol, dataItem);
			feed->UpdateTradeData(tradeData);
			MarketDataSubject::NotifyTradeChange(symbol);
		}
		else if (subscriptionType == "AggregateTradeData")
		{
			const auto aggregateTradeData = BuildAggregateTradeData(symbol, dataItem);
			feed->UpdateAggregateTradeData(aggregateTradeData);
			MarketDataSubject::NotifyAggregateTradeChange(symbol);
		}
		else if (subscriptionType == "IndividualMiniTickerData")
		{
			const auto miniTickerData = BuildIndividualMiniTickerData(symbol, dataItem);
			feed->UpdateMiniTickerData(miniTickerData);
			MarketDataSubject::NotifyMiniTickerChange(symbol);
		}
		// more subscription types can be added here...
		return;
	}
	else
	{
		throw std::runtime_error("HistoricalMarketDataFeedHandler: sycnchronous feed could not found with symbol=" + symbol);
	}
}

binapi::ws::book_ticker_t HistoricalMarketDataFeedHandler::BuildIndividualBookTickerData(
	const std::string& symbol, const std::unordered_map<std::string, std::string>& dataItem)
{
	binapi::ws::book_ticker_t bookTicker;
	bookTicker.s = symbol;
	bookTicker.u = TimeUtils::StringDateTimeToEpochSeconds(dataItem.at("EVENT_TIME_MS"));
	bookTicker.b = std::stod(dataItem.at("BEST_BID_PRICE"));
	bookTicker.B = std::stod(dataItem.at("BEST_BID_QTY"));
	bookTicker.a = std::stod(dataItem.at("BEST_ASK_PRICE"));
	bookTicker.A = std::stod(dataItem.at("BEST_ASK_QTY"));
	return bookTicker;
}

binapi::ws::trade_t HistoricalMarketDataFeedHandler::BuildTradeData(
	const std::string& symbol, const std::unordered_map<std::string, std::string>& dataItem)
{
	binapi::ws::trade_t trade;
	trade.s = symbol;
	trade.E = TimeUtils::StringDateTimeToEpochSeconds(dataItem.at("EVENT_TIME_MS"));
	trade.t = std::stoull(dataItem.at("TRADE_ID"));
	trade.p = std::stod(dataItem.at("PRICE"));
	trade.q = std::stod(dataItem.at("QUANTITY"));
	trade.b = std::stoull(dataItem.at("BUYER_ORDER_ID"));
	trade.a = std::stoull(dataItem.at("SELLER_ORDER_ID"));
	trade.T = TimeUtils::StringDateTimeToEpochSeconds(dataItem.at("TRADE_TIME"));
	trade.m = (dataItem.at("IS_BUYER_MARKET_MAKER") == "true");
	return trade;
}

binapi::ws::mini_ticker_t HistoricalMarketDataFeedHandler::BuildIndividualMiniTickerData(
	const std::string& symbol, const std::unordered_map<std::string, std::string>& dataItem)
{
	binapi::ws::mini_ticker_t miniTicker;
	miniTicker.s = symbol;
	miniTicker.E = TimeUtils::StringDateTimeToEpochSeconds(dataItem.at("EVENT_TIME_MS"));
	miniTicker.c = std::stod(dataItem.at("CLOSE_PRICE"));
	miniTicker.o = std::stod(dataItem.at("OPEN_PRICE"));
	miniTicker.h = std::stod(dataItem.at("HIGH_PRICE"));
	miniTicker.l = std::stod(dataItem.at("LOW_PRICE"));
	miniTicker.v = std::stod(dataItem.at("TOTAL_TRADED_BASE_ASSET_VOLUME"));
	miniTicker.q = std::stod(dataItem.at("TOTAL_TRADED_QUOTE_ASSET_VOLUME"));
	return miniTicker;
}

binapi::ws::agg_trade_t HistoricalMarketDataFeedHandler::BuildAggregateTradeData(
	const std::string& symbol, const std::unordered_map<std::string, std::string>& dataItem)
{
	binapi::ws::agg_trade_t aggregateTrade;
	aggregateTrade.s = symbol;
	aggregateTrade.E = TimeUtils::StringDateTimeToEpochSeconds(dataItem.at("EVENT_TIME_MS"));
	aggregateTrade.a = std::stoull(dataItem.at("AGGREGATE_TRADE_ID"));
	aggregateTrade.p = std::stod(dataItem.at("PRICE"));
	aggregateTrade.q = std::stod(dataItem.at("QUANTITY"));
	aggregateTrade.f = std::stoull(dataItem.at("FIRST_TRADE_ID"));
	aggregateTrade.l = std::stoull(dataItem.at("LAST_TRADE_ID"));
	aggregateTrade.T = TimeUtils::StringDateTimeToEpochSeconds(dataItem.at("TRADE_TIME"));
	aggregateTrade.m = (dataItem.at("IS_BUYER_MARKET_MAKER") == "true");
	return aggregateTrade;
}
