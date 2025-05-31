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
#include "FutureTradeManager.h"
#include "UserTradeProfile.h"
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

    /* 1. Formula for PNL(no fees) For One - Way Mode and no partial fills :
    Long Position : PNL = (Exit Price − Entry Price) × Quantity
    Short Position : PNL = (Entry Price − Exit Price) × Quantity*/
    
    /* Fee = Entry Price×Quantity×Fee Rate + Exit Price×Quantity×Fee Rate
    
    Typical fee rate(for USDT - M futures) :
        Maker: 0.02 % = 0.0002
        Taker : 0.04 % = 0.0004

    Final PNL formula(with fees) : Net PNL = Gross PNL − Total Fee */ 

    std::unique_lock<std::mutex> lock(m_mutex);

    //Check the Order Book:
    //  Compare the upstream incoming order against the existing orders in the order book of the target symbol.
    bool hasLiquidity{ false };
    if (newUpstreamOrder.GetSide() == binapi::e_side::buy) // long positions
    {
        // For a Buy Side:
        // Required margin = quantity × price / leverage
		const auto currentMarketPrice = m_downstreamFuturePriceManagers[newUpstreamOrder.GetSymbol()]->GetCurrentMarketPrice();
		// Calculate the required margin
		const auto leverageRate = m_futureTradeManager->LookupUserTradeProfile(newUpstreamOrder.GetUserAccountID())->GetLeverageRate();
		const auto requiredMarginCash = (newUpstreamOrder.GetAmount() * currentMarketPrice) / leverageRate;
		const auto hasSufficientMarginCash = m_userAccountManager->IsAccountHavingSufficientMargin(newUpstreamOrder.GetSymbol(), requiredMarginCash);
        if (hasSufficientMarginCash)
        {

        }
        else
        {
			m_logger->Warning("User account does not have sufficient margin cash to open a new position for order: " +
				newUpstreamOrder.ToStringOrder());
			return false; // Insufficient margin, cannot proceed with the order
        }
       
    }
    else if (newUpstreamOrder.GetSide() == binapi::e_side::sell) // short positions
    {
        // For a Sell Side:
		// Required margin = quantity × price / leverage
    }
    return hasLiquidity;
}

bool RTMarketFutureParticipant::OnTradeChange(MarketData::MarketDataSubject* marketData, const std::string& symbol)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    if (const auto* data = marketData->GetSynchronousMarketData(symbol))
    {
		const auto& singleFeedData = data->GetSingleFeed(TradeID::PRICE);
        if (singleFeedData->GetDataStatus() == MarketData::MarketDataFeedStatus::UPDATED)
        {
            UpdateCurrentMarketPrice(symbol, data->GetSingleFeed(TradeID::PRICE)->GetDoubleData());
            return true;
        }
        else
        {
			m_logger->Warning("Market data for symbol=" + symbol + " is not updated, status=" +
				MarketData::SingleMarketDataFeed::MarketDataFeedStatusToString(singleFeedData->GetDataStatus()));
			return false;
        }
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
	if (auto futurePrice = m_downstreamFuturePriceManagers.find(symbol);
        m_downstreamFuturePriceManagers.end() != futurePrice)
	{
        futurePrice->second->SetNewMarketPrice(price);
	}
    else
    {
        m_logger->Warning("Could not found downstream future price manager for symbol=" + symbol);
    }
}
