/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../LibraryUtils/Logger.h"
#include "BinanceMarketDataFeedHandler.h"

#include <iostream>

using namespace MarketData;

BinanceMarketDataFeedHandler::BinanceMarketDataFeedHandler(LibraryUtils::Logger* logger)
	: m_synchronousFeedMgr{ std::make_unique<MarketDataFeedManager>() },
	m_logger(logger) {
}

bool BinanceMarketDataFeedHandler::CreateNewMarketDataFeed(const std::string& symbol)
{
    return m_synchronousFeedMgr->CreateNewSynchronousFeed(symbol);
}

void BinanceMarketDataFeedHandler::RemoveMarketDataFeed(const std::string& symbol)
{
    m_synchronousFeedMgr->RemoveSynchronousFeed(symbol);
}

bool BinanceMarketDataFeedHandler::HandleIndividualBookTickerData(const char* fl,
    int ec, std::string emsg, binapi::ws::book_ticker_t book)
{
    if (ec) 
    {
		m_logger->Error("subscribe book error: fl=" + std::string(fl) + ", ec=" + std::to_string(ec) + ", emsg=" + emsg);
        return false;
    }
    if (auto* feed = m_synchronousFeedMgr->GetSynchronousFeed(book.s))
    {
        feed->UpdateIndividualBookTickerData(book);
        MarketDataSubject::NotifyIndividualBookTickerChange(book.s);
        return true;
    }
	else
    {
        throw std::runtime_error("BinanceMarketDataFeedHandler: sycnchronous feed could not found with symbol=" + book.s);
    }
    return false;
}

bool BinanceMarketDataFeedHandler::HandleTradeData(const char* fl, int ec, std::string emsg, binapi::ws::trade_t trade)
{
    if (ec)
    {
		m_logger->Error("subscribe trade error: fl=" + std::string(fl) + ", ec=" + std::to_string(ec) + ", emsg=" + emsg);
        return false;
    }
    if (auto* feed = m_synchronousFeedMgr->GetSynchronousFeed(trade.s))
    {
        feed->UpdateTradeData(trade);
        MarketDataSubject::NotifyTradeChange(trade.s);
        return true;
    }
    else
    {
        throw std::runtime_error("BinanceMarketDataFeedHandler: sycnchronous feed could not found with symbol=" + trade.s);
    }
    return false;
}

bool BinanceMarketDataFeedHandler::HandleIndividualMarketTickerData(const char* fl, int ec, std::string emsg, binapi::ws::market_ticker_t market)
{
	if (ec)
	{
		m_logger->Error("subscribe market ticker error: fl=" + std::string(fl) + ", ec=" + std::to_string(ec) + ", emsg=" + emsg);
		return false;
	}

	if (auto* feed = m_synchronousFeedMgr->GetSynchronousFeed(market.s))
	{
		feed->UpdateIndividualMarketTickerData(market);
		MarketDataSubject::NotifyIndividualMarketTickerChange(market.s);
		return true;
	}
	else
    {
		throw std::runtime_error("BinanceMarketDataFeedHandler: sycnchronous feed could not found with symbol=" + market.s);
    }

    return false;
}

bool BinanceMarketDataFeedHandler::HandleMiniTickerData(const char* fl, int ec, std::string emsg, binapi::ws::mini_ticker_t mini)
{
	if (ec)
	{
		m_logger->Error("subscribe mini ticker error: fl=" + std::string(fl) + ", ec=" + std::to_string(ec) + ", emsg=" + emsg);
		return false;
	}

	if (auto* feed = m_synchronousFeedMgr->GetSynchronousFeed(mini.s))
	{
		feed->UpdateMiniTickerData(mini);
		MarketDataSubject::NotifyMiniTickerChange(mini.s);
		return true;
	}
	else
	{
		throw std::runtime_error("BinanceMarketDataFeedHandler: sycnchronous feed could not found with symbol=" + mini.s);
	}

    return false;
}

bool BinanceMarketDataFeedHandler::HandleAggregateTradeData(const char* fl, int ec, std::string emsg, binapi::ws::agg_trade_t aggregate)
{
	if (ec)
	{
		m_logger->Error("subscribe aggregate trade error: fl=" + std::string(fl) + ", ec=" + std::to_string(ec) + ", emsg=" + emsg);
		return false;
	}

	if (auto* feed = m_synchronousFeedMgr->GetSynchronousFeed(aggregate.s))
	{
		feed->UpdateAggregateTradeData(aggregate);
		MarketDataSubject::NotifyAggregateTradeChange(aggregate.s);
		return true;
	}
	else
	{
		throw std::runtime_error("BinanceMarketDataFeedHandler: sycnchronous feed could not found with symbol=" + aggregate.s);
	}

    return false;
}

bool BinanceMarketDataFeedHandler::HandleKlineCandleStickData(const char* fl, int ec, std::string emsg, binapi::ws::kline_t kline)
{
    if (ec)
    {
		m_logger->Error("subscribe kline candle stick error: fl=" + std::string(fl) + ", ec=" + std::to_string(ec) + ", emsg=" + emsg);
		return false;
    }

	if (auto* feed = m_synchronousFeedMgr->GetSynchronousFeed(kline.s))
	{
		feed->UpdateKlineCandleStickData(kline);
		MarketDataSubject::NotifyKlineCandleStickChange(kline.s);
		return true;
	}
	else
	{
		throw std::runtime_error("BinanceMarketDataFeedHandler: sycnchronous feed could not found with symbol=" + kline.s);
	}
    return false;
}

bool BinanceMarketDataFeedHandler::HandleAllMiniTickerData(const char* fl, int ec, std::string emsg, binapi::ws::mini_tickers_t mini)
{
	if (ec)
	{
		m_logger->Error("subscribe all mini tickers error: fl=" + std::string(fl) + ", ec=" + std::to_string(ec) + ", emsg=" + emsg);
		return false;
	}
	for (const auto& ticker : mini.tickers)
	{
		if (auto* feed = m_synchronousFeedMgr->GetSynchronousFeed(ticker.first))
		{
			feed->UpdateMiniTickerData(ticker.second);
			MarketDataSubject::NotifyMiniTickerChange(ticker.first);
		}
		else
		{
			if (m_synchronousFeedMgr->CreateNewSynchronousFeed(ticker.first))
			{
				if (auto* newFeed = m_synchronousFeedMgr->GetSynchronousFeed(ticker.first))
				{
					newFeed->UpdateMiniTickerData(ticker.second);
					MarketDataSubject::NotifyMiniTickerChange(ticker.first);
				}
				else
				{
					throw std::runtime_error("BinanceMarketDataFeedHandler: sycnchronous feed could not found with symbol=" + ticker.first);
				}
			}
			else
			{
				std::cerr << "Failed to create new synchronous feed for symbol: " << ticker.first << std::endl;
			}
		}
		
	}
	return false;
}

bool BinanceMarketDataFeedHandler::HandleAllMarketTickersData(const char* fl, int ec, std::string emsg, binapi::ws::markets_tickers_t market)
{
	if (ec)
	{
		m_logger->Error("subscribe all market tickers error: fl=" + std::string(fl) + ", ec=" + std::to_string(ec) + ", emsg=" + emsg);
		return false;
	}
	for (const auto& ticker : market.tickers)
	{
		if (auto* feed = m_synchronousFeedMgr->GetSynchronousFeed(ticker.first))
		{
			feed->UpdateIndividualMarketTickerData(ticker.second);
			MarketDataSubject::NotifyIndividualMarketTickerChange(ticker.first);
		}
		else
		{
			if (m_synchronousFeedMgr->CreateNewSynchronousFeed(ticker.first))
			{
				if (auto* newFeed = m_synchronousFeedMgr->GetSynchronousFeed(ticker.first))
				{
					newFeed->UpdateIndividualMarketTickerData(ticker.second);
					MarketDataSubject::NotifyIndividualMarketTickerChange(ticker.first);
				}
				else
				{
					throw std::runtime_error("BinanceMarketDataFeedHandler: sycnchronous feed could not found with symbol=" + ticker.first);
				}
			}
			else
			{
				std::cerr << "Failed to create new synchronous feed for symbol: " << ticker.first << std::endl;
			}
		}
	}
	return false;
}

bool BinanceMarketDataFeedHandler::HandlePartDepthData(const char* fl, int ec, std::string emsg, binapi::ws::part_depths_t depth)
{
	if (ec)
	{
		m_logger->Error("subscribe part depth error: fl=" + std::string(fl) + ", ec=" + std::to_string(ec) + ", emsg=" + emsg);
		return false;
	}
	if (auto* feed = m_synchronousFeedMgr->GetSynchronousFeed(m_partDiffSymbol))
	{
		feed->UpdateAllPartDepthData(depth);
		MarketDataSubject::NotifyAllPartDepthChange(m_partDiffSymbol);
		return true;
	}
	else
	{
		throw std::runtime_error("BinanceMarketDataFeedHandler: sycnchronous feed could not found with symbol=" + m_partDiffSymbol);
	}
	return false;
}

bool BinanceMarketDataFeedHandler::HandleDiffDepthData(const char* fl, int ec, std::string emsg, binapi::ws::diff_depths_t depth)
{
	if (ec)
	{
		m_logger->Error("subscribe diff depth error: fl=" + std::string(fl) + ", ec=" + std::to_string(ec) + ", emsg=" + emsg);
		return false;
	}

	if (auto* feed = m_synchronousFeedMgr->GetSynchronousFeed(depth.s))
	{
		feed->UpdateAllDiffDepthData(depth);
		MarketDataSubject::NotifyAllDiffDepthChange(depth.s);
		return true;
	}
	else
	{
		throw std::runtime_error("BinanceMarketDataFeedHandler: sycnchronous feed could not found with symbol=" + depth.s);
	}

	return false;
}

bool BinanceMarketDataFeedHandler::HandleUserDataAccountUpdate(const char* fl, int ec, std::string emsg, binapi::userdata::account_update_t accountUpdate)
{
	if (ec)
	{
		std::cerr << "subscribe user data account update error: fl=" << fl << ", ec=" << ec << ", emsg=" << emsg << std::endl;
		return false;
	}
	for (const auto& symbol : accountUpdate.B)
	{
		if (auto* feed = m_synchronousFeedMgr->GetSynchronousFeed(symbol.second.a))
		{
			feed->UpdateUserDataAccountUpdate(accountUpdate);
			MarketDataSubject::NotifyUserDataAccountUpdateChange(symbol.second.a);
		}
	}
	return true;
}

bool BinanceMarketDataFeedHandler::HandleUserDataBalanceUpdate(const char* fl, int ec, std::string emsg, binapi::userdata::balance_update_t balanceUpdate)
{
	if (ec)
	{
		m_logger->Error("subscribe user data balance update error: fl=" + std::string(fl) + ", ec=" + std::to_string(ec) + ", emsg=" + emsg);
		return false;
	}
	if (auto* feed = m_synchronousFeedMgr->GetSynchronousFeed(balanceUpdate.a))
	{
		feed->UpdateUserDataBalanceUpdate(balanceUpdate);
		MarketDataSubject::NotifyUserDataBalanceUpdateChange(balanceUpdate.a);
	}
	return true;
}

bool BinanceMarketDataFeedHandler::HandleUserDataOrderUpdate(const char* fl, int ec, std::string emsg, binapi::userdata::order_update_t orderUpdate)
{
	if (ec)
	{
		m_logger->Error("subscribe user data order update error: fl=" + std::string(fl) + ", ec=" + std::to_string(ec) + ", emsg=" + emsg);
		return false;
	}
	if (auto* feed = m_synchronousFeedMgr->GetSynchronousFeed(orderUpdate.s))
	{
		feed->UpdateUserDataOrderUpdate(orderUpdate);
		MarketDataSubject::NotifyUserDataOrderUpdateChange(orderUpdate.s);
		return true;
	}
	else
	{
		throw std::runtime_error("BinanceMarketDataFeedHandler: sycnchronous feed could not found with symbol=" + orderUpdate.s);
	}
	return false;
}

bool BinanceMarketDataFeedHandler::HandleTradeDataFuture(const char* fl, int ec, std::string emsg, binapi::ws::future_trade_t trade)
{
	if (ec)
	{
		m_logger->Error("subscribe future trade error: fl=" + std::string(fl) + ", ec=" + std::to_string(ec) + ", emsg=" + emsg);
		return false;
	}
	if (auto* feed = m_synchronousFeedMgr->GetSynchronousFeed(trade.s))
	{
		feed->UpdateFutureTradeData(trade);
		MarketDataSubject::NotifyFutureTradeChange(trade.s);
		return true;
	}
	else
	{
		throw std::runtime_error("BinanceMarketDataFeedHandler: sycnchronous feed could not found with symbol=" + trade.s);
	}
	return false;
}

bool BinanceMarketDataFeedHandler::HandleBookDataFuture(const char* fl, int ec, std::string emsg, binapi::ws::future_book_t book)
{
	if (ec)
	{
		m_logger->Error("subscribe future book data error: fl=" + std::string(fl) + ", ec=" + std::to_string(ec) + ", emsg=" + emsg);
		return false;
	}
	if (auto* feed = m_synchronousFeedMgr->GetSynchronousFeed(book.s))
	{
		feed->UpdateFutureBookData(book);
		MarketDataSubject::NotifyFutureBookChange(book.s);
		return true;
	}
	else
	{
		throw std::runtime_error("BinanceMarketDataFeedHandler: sycnchronous feed could not found with symbol=" + book.s);
	}
	return false;
}

bool BinanceMarketDataFeedHandler::HandleFundingDataFuture(const char* fl, int ec, std::string emsg, binapi::ws::future_funding_rate_t funding)
{
	if (ec)
	{
		m_logger->Error("subscribe future funding data error: fl=" + std::string(fl) + ", ec=" + std::to_string(ec) + ", emsg=" + emsg);
		return false;
	}
	if (auto* feed = m_synchronousFeedMgr->GetSynchronousFeed(funding.s))
	{
		feed->UpdateFutureFundingData(funding);
		MarketDataSubject::NotifyFutureFundingChange(funding.s);
		return true;
	}
	else
	{
		throw std::runtime_error("BinanceMarketDataFeedHandler: sycnchronous feed could not found with symbol=" + funding.s);
	}
	return false;
}

SynchronousMarketData* BinanceMarketDataFeedHandler::GetSynchronousMarketData(const std::string& symbol)
{
    if (auto* feed = m_synchronousFeedMgr->GetSynchronousFeed(symbol))
    {
        return feed->GetSynchronousData();
    }
    return nullptr;
}
