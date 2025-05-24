/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include "../MarketData/MarketDataSubject.h"
#include "../MarketData/SynchronousMarketData.h"
#include "../LibraryUtils/Logger.h"
#include "../LibraryUtils/TimeUtils.h"

#include "ExchangeRuleAndCompliance.h"
#include "RTMarketFutureParticipant.h"
#include "UserAccountManager.h"
#include "TradeUtils.h"

using namespace ExchangeSimulator;
using namespace OrderManagement;
using namespace MarketData;

static constexpr double ZERO_DOUBLE_VALUE = 0;

RTMarketFutureParticipant::RTMarketFutureParticipant(
    const size_t maxDownstreamOrderBookSize,
    UserAccountManager* userAccountManager)
    : Participant(ParticipantType::REAL_TIME_FUTURE_MARKET_DATA, userAccountManager)
{
}

RTMarketFutureParticipant::~RTMarketFutureParticipant() {}

bool RTMarketFutureParticipant::TryToMatchOrder(OrderManagement::BinanceNewOrder& newUpstreamOrder)
{
    // at here, the order matching logic happened...

    /* Order Matching Algorithms: First-in, first-out (FIFO).
    Also known as "first-come, first-serve" (FCFS),
    FIFO represents the classic algorithm that prioritizes orders based
    on their priceand creation time.When multiple orders are created at the same price,
    the order that arrived first gets matched first, ensuring fairness in execution.*/

    /* When Does an Order Get Filled ?
    Market Order :
        Executes immediately at the best available price.
        You “take liquidity” from the order book.
        You’ll get filled instantly, possibly at multiple prices if your order is large.
    Limit Order :
        Executes only if the market reaches your specified price.
        You “provide liquidity”.
        Order sits in the book until matched by a market or opposing limit order.
    Stop Orders(e.g., Stop - Limit) :
        Becomes active when a trigger price is reached.
        Then acts as a regular market or limit order. */

    std::unique_lock<std::mutex> lock(m_mutex);

    //Check the Order Book:
    //  Compare the upstream incoming order against the existing orders in the order book of the target symbol.
    bool hasLiquidity{ false };
    if (newUpstreamOrder.GetSide() == binapi::e_side::buy) // long positions
    {
        // For a Buy Side:
       
    }
    else if (newUpstreamOrder.GetSide() == binapi::e_side::sell) // short positions
    {
        // For a Sell Side:
    }
    return hasLiquidity;
}

bool RTMarketFutureParticipant::OnTradeChange(MarketData::MarketDataSubject* marketData, const std::string& symbol)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    if (const auto* data = marketData->GetSynchronousMarketData(symbol))
    {
        UpdateCurrentMarketPrice(symbol, data->GetSingleFeed(TradeID::PRICE)->GetDoubleData());
        return true;
    }
    m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    return false;
}

void RTMarketFutureParticipant::CreateDownstreamFuturePriceManagers(const std::unordered_set<std::string>& subcribedSymbols)
{
    for (const auto& symbol : subcribedSymbols)
    {
        m_downstreamFuturePriceManagers.emplace(symbol, std::make_unique<DownstreamFuturePriceManager>(symbol, ZERO_DOUBLE_VALUE));
    }
}

void RTMarketFutureParticipant::UpdateCurrentMarketPrice(const std::string& symbol, const double price)
{
    auto* futurePrice = m_downstreamFuturePriceManagers[symbol].get();
	if (futurePrice)
	{
		futurePrice->SetNewMarketPrice(price);
	}
	else
	{
		m_logger->Warning("Could not found downstream future price manager for symbol=" + symbol);
	}
}
