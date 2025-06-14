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
#include "RTMarketSpotParticipant.h"
#include "UserAccountManager.h"
#include "TradeUtils.h"

using namespace ExchangeSimulator;
using namespace OrderManagement;
using namespace MarketData;

static constexpr double ZERO_DOUBLE_VALUE = 0;

RTMarketSpotParticipant::RTMarketSpotParticipant(
    const size_t maxDownstreamOrderBookSize,
    UserAccountManager* userAccountManager)
    : Participant(ParticipantType::REAL_TIME_SPOT_MARKET_DATA, userAccountManager),
    m_maxDownstreamOrderBookSize(maxDownstreamOrderBookSize)
{
    m_bidDownstreamOrderBooks = std::make_unique<DownstreamOrderBook>();
    m_askDownstreamOrderBooks = std::make_unique<DownstreamOrderBook>();
}

RTMarketSpotParticipant::~RTMarketSpotParticipant() {}

bool RTMarketSpotParticipant::TryToMatchOrder(OrderManagement::BinanceNewOrder& newUpstreamOrder)
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
        //  Look at the sell-side of the order book (asks).
        //	Find the lowest-price sell order that satisfies the buy price and volume.
        if (auto* exchangeBidOrderBook = m_bidDownstreamOrderBooks->LookupOrderBook(newUpstreamOrder.GetSymbol()))
        {
            auto bestExchangeBidOrder = exchangeBidOrderBook->GetAllItems().front();

			if (newUpstreamOrder.GetOrderType() == binapi::e_type::limit)
			{
				// If the order type is LIMIT, we need to check the order book for matching orders.
				// Find the best exchange bid order that satisfies the buy price.
				// The best exchange bid order is the one with the highest price that is less than or equal to the buy price.
                for (const auto& exchangeOrder : exchangeBidOrderBook->GetAllItems())
                {
                    // Determine the best exchange bid order that satisfies the buy price from upstream order.
                    if (exchangeOrder.m_price <= newUpstreamOrder.GetPrice())
                    {
                        bestExchangeBidOrder = exchangeOrder;
                        hasLiquidity = true;
                    }
                }
			}
            else if (newUpstreamOrder.GetOrderType() == binapi::e_type::market)
            {
				// If the order type is MARKET, we need to find the best exchange bid order match market upstream order.
                for (const auto& exchangeOrder : exchangeBidOrderBook->GetAllItems())
                {
                    if (exchangeOrder.m_price <= bestExchangeBidOrder.m_price)
                    {
                        bestExchangeBidOrder = exchangeOrder;
                        hasLiquidity = true;
                    }
                }
            }
			else if (newUpstreamOrder.GetOrderType() == binapi::e_type::stop_loss)
			{
			}

            if (hasLiquidity)
            {
                //Execute Trades:
                //  If a match is found (hasLiquidity), execute the trade and update the order book.
                // open edit session for user wallet
                auto* userAccount = m_userAccountManager->OpenEditSessionForSpotUserAccount(newUpstreamOrder.GetUserAccountID());
                // update order ack to upstream
                newUpstreamOrder.SetFilledPrice(bestExchangeBidOrder.m_price);
                if (bestExchangeBidOrder.m_quantity >= newUpstreamOrder.GetAmount()) // FULL FILL ORDER
                {
                    // update amount for traded asset
                    // 1. increasing crypto asset coin amount
                    auto& assetBalance = userAccount->LookupAssetBalance(newUpstreamOrder.GetSymbol());
                    assetBalance.m_free += newUpstreamOrder.GetAmount();
                    // 2. decreasing stable coin amount
                    userAccount->m_usdtBalance.m_usdtAmount
                        -= (Finance::CalculateTradeValue(newUpstreamOrder.GetAmount(), bestExchangeBidOrder.m_price)
						+ (Finance::CalculateTradeValue(newUpstreamOrder.GetAmount(), bestExchangeBidOrder.m_price)
                        * ExchangeRuleMgr->GetMakerCommission() * ExchangeRuleMgr->GetTakerCommission()));
                    // 3. update filled ack
                    newUpstreamOrder.SetFilledAmount(newUpstreamOrder.GetAmount());
					newUpstreamOrder.SetRemainingAmount(ZERO_DOUBLE_VALUE);
                    newUpstreamOrder.SetOrderStatus(BinanceNewOrderStatus::FULL_FILLED);
                }
                else if (bestExchangeBidOrder.m_quantity < newUpstreamOrder.GetAmount()) // PARTIAL FILL ORDER
                {
                    // update amount for traded asset
                    // 1. increasing crypto asset coin amount
                    auto& assetBalance = userAccount->LookupAssetBalance(newUpstreamOrder.GetSymbol());
                    assetBalance.m_free += bestExchangeBidOrder.m_quantity;
                    // 2. decreasing stable coin amount
                    userAccount->m_usdtBalance.m_usdtAmount
                        -= (Finance::CalculateTradeValue(bestExchangeBidOrder.m_quantity, bestExchangeBidOrder.m_price)
                            + (Finance::CalculateTradeValue(bestExchangeBidOrder.m_quantity, bestExchangeBidOrder.m_price)
                                * ExchangeRuleMgr->GetMakerCommission() * ExchangeRuleMgr->GetTakerCommission()));
                    // 3. update filled ack
                    newUpstreamOrder.SetFilledAmount(bestExchangeBidOrder.m_quantity);
                    newUpstreamOrder.SetRemainingAmount(newUpstreamOrder.GetAmount() - bestExchangeBidOrder.m_quantity);
                    newUpstreamOrder.SetOrderStatus(BinanceNewOrderStatus::PRTIAL_FILLED);
                }
            }
            else // process for unfilled order
            {
                newUpstreamOrder.SetRemainingAmount(newUpstreamOrder.GetAmount());
            }
            newUpstreamOrder.SetUpdateTime(TimeUtils::GetEpochTimeTickNow());
        }
    }
    else if (newUpstreamOrder.GetSide() == binapi::e_side::sell) // short positions
    {
        // For a Sell Side:
        //  Look at the buy-side of the order book (bids).
        //  Find the highest-price buy order that satisfies the sell price.
        if (auto* exchangeAskOrderBook = m_askDownstreamOrderBooks->LookupOrderBook(newUpstreamOrder.GetSymbol()))
        {
            auto bestExchangeAskOrder = exchangeAskOrderBook->GetAllItems().front();

			if (newUpstreamOrder.GetOrderType() == binapi::e_type::limit)
			{
				// If the order type is LIMIT or MARKET, we need to check the order book for matching orders.
				// Find the best exchange ask order that satisfies the sell price.
				// The best exchange ask order is the one with the lowest price that is greater than or equal to the sell price.
				for (const auto& exchangeOrder : exchangeAskOrderBook->GetAllItems())
				{
					// Determine the best exchange ask order that satisfies the sell price from upstream order.
					if (exchangeOrder.m_price >= newUpstreamOrder.GetPrice())
					{
						bestExchangeAskOrder = exchangeOrder;
						hasLiquidity = true;
					}
				}
			}
            else if (newUpstreamOrder.GetOrderType() == binapi::e_type::market)
            {
                // If the order type is MARKET, we need to find the best exchange ask order match market upstream order.
                for (const auto& exchangeOrder : exchangeAskOrderBook->GetAllItems())
                {
                    if (exchangeOrder.m_price <= bestExchangeAskOrder.m_price)
                    {
                        bestExchangeAskOrder = exchangeOrder;
                        hasLiquidity = true;
                    }
                }
            }
			else if (newUpstreamOrder.GetOrderType() == binapi::e_type::stop_loss)
			{		
			}

            if (hasLiquidity)
            {
                //Execute Trades:
                //  If a match is found (hasLiquidity), execute the trade and update the order book.
                // open edit session for user wallet
                auto* userAccount = m_userAccountManager->OpenEditSessionForSpotUserAccount(newUpstreamOrder.GetUserAccountID());
                // update order ack to upstream
                newUpstreamOrder.SetFilledPrice(bestExchangeAskOrder.m_price);
                if (bestExchangeAskOrder.m_quantity >= newUpstreamOrder.GetAmount()) // FULL FILL ORDER
                {
                    // update amount for traded asset
                    // 1. decreasing crypto asset coin amount
                    auto& assetBalance = userAccount->LookupAssetBalance(newUpstreamOrder.GetSymbol());
                    assetBalance.m_free -= newUpstreamOrder.GetAmount();
                    // 2. increasing stable coin amount
                    userAccount->m_usdtBalance.m_usdtAmount
                        += (Finance::CalculateTradeValue(newUpstreamOrder.GetAmount(), bestExchangeAskOrder.m_price) 
                        - (Finance::CalculateTradeValue(newUpstreamOrder.GetAmount(), bestExchangeAskOrder.m_price) 
                    * ExchangeRuleMgr->GetMakerCommission() * ExchangeRuleMgr->GetTakerCommission()));
                    newUpstreamOrder.SetFilledAmount(newUpstreamOrder.GetAmount());
                    newUpstreamOrder.SetRemainingAmount(ZERO_DOUBLE_VALUE);
                    newUpstreamOrder.SetOrderStatus(BinanceNewOrderStatus::FULL_FILLED);
                }
                else if (bestExchangeAskOrder.m_quantity < newUpstreamOrder.GetAmount()) // PARTIAL FILL ORDER
                {
                    // update amount for traded asset
                // 1. decreasing crypto asset coin amount
                    auto& assetBalance = userAccount->LookupAssetBalance(newUpstreamOrder.GetSymbol());
                    assetBalance.m_free -= bestExchangeAskOrder.m_quantity;
                    // 2. increasing stable coin amount
                    userAccount->m_usdtBalance.m_usdtAmount
                        += (Finance::CalculateTradeValue(bestExchangeAskOrder.m_quantity, bestExchangeAskOrder.m_price)
                            - (Finance::CalculateTradeValue(bestExchangeAskOrder.m_quantity, bestExchangeAskOrder.m_price)
                                * ExchangeRuleMgr->GetMakerCommission() * ExchangeRuleMgr->GetTakerCommission()));
                    newUpstreamOrder.SetFilledAmount(bestExchangeAskOrder.m_quantity);
                    newUpstreamOrder.SetRemainingAmount(newUpstreamOrder.GetAmount() - bestExchangeAskOrder.m_quantity);
                    newUpstreamOrder.SetOrderStatus(BinanceNewOrderStatus::PRTIAL_FILLED);
                }
            }
            else // process for unfilled order
            {
                newUpstreamOrder.SetRemainingAmount(newUpstreamOrder.GetAmount());
            }
            newUpstreamOrder.SetUpdateTime(TimeUtils::GetEpochTimeTickNow());
        }
    }
    return hasLiquidity;
}

bool RTMarketSpotParticipant::OnIndividualBookTickerChange(MarketDataSubject* marketData, const std::string& symbol)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    if (const auto* data = marketData->GetSynchronousMarketData(symbol))
    {
        UpdateBestMarketBidOrderBook(symbol,
            data->GetSingleFeed(IndividualBookTickerID::BEST_BID_PRICE)->GetDoubleData(),
            data->GetSingleFeed(IndividualBookTickerID::BEST_BID_QUANTITY)->GetDoubleData());
        UpdateBestMarketAskOrderBook(symbol,
            data->GetSingleFeed(IndividualBookTickerID::BEST_ASK_PRICE)->GetDoubleData(),
            data->GetSingleFeed(IndividualBookTickerID::BEST_ASK_QUANTITY)->GetDoubleData());
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}

void RTMarketSpotParticipant::CreateDownstreamOrderBooks(const std::unordered_set<std::string>& subcribedSymbols)
{
    for (const auto& symbol : subcribedSymbols)
    {
        m_bidDownstreamOrderBooks->CreateNewOrderBook(symbol);
        m_askDownstreamOrderBooks->CreateNewOrderBook(symbol);
    }
}

void RTMarketSpotParticipant::UpdateBestMarketBidOrderBook(const std::string& symbol, const double price, const double quantity)
{
    auto* orderBook = m_bidDownstreamOrderBooks->LookupOrderBook(symbol);
    if (!orderBook->IsEmpty() && orderBook->GetSize() >= m_maxDownstreamOrderBookSize)
    {
        orderBook->PopFront(); // Remove the oldest price
    }
    orderBook->EmplaceBack(std::move(DownstreamOrder(price, quantity)));
}

void RTMarketSpotParticipant::UpdateBestMarketAskOrderBook(const std::string& symbol, const double price, const double quantity)
{
    auto* orderBook = m_askDownstreamOrderBooks->LookupOrderBook(symbol);
    if (!orderBook->IsEmpty() && orderBook->GetSize() >= m_maxDownstreamOrderBookSize)
    {
        orderBook->PopFront(); // Remove the oldest price
    }
    orderBook->EmplaceBack(std::move(DownstreamOrder(price, quantity)));
}
