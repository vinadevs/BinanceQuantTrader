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
#include "UserTradeProfileManager.h"
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
	// Get the user account and related information
    auto* userAccount = m_userAccountManager->LookupFutureUserAccount(newUpstreamOrder.GetUserAccountID());
	// Get the order entry price, current market price, and number of contracts
    const auto orderEntryPrice = newUpstreamOrder.GetPrice();
    const auto currentMarketPrice = m_downstreamFuturePriceManagers[newUpstreamOrder.GetSymbol()]->GetCurrentMarketPrice();
    const auto numberOfContracts = newUpstreamOrder.GetAmount();
    // Calculate the required margin
    const auto* userFutureAssetInfo = userAccount->LookupFutureAssetInfo("USDT");
	// Lookup the user trade profile to get leverage rate and margin
    const auto& userTradeProfile = m_userTradeProfileManager->LookupUserTradeProfile(newUpstreamOrder.GetUserAccountID());
	// Get the leverage rate for the user
    const auto userLeverageRate = userTradeProfile.GetLeverageRate();
	// Get the user initial margin from the user's future asset balance
    const auto userInitialMargin = userFutureAssetInfo->initialMargin;
	// Calculate position value = orderEntryPrice * numberOfContracts
	const double positionValue = orderEntryPrice * numberOfContracts;
    // Calculate Exit Fee = Position Value × Free Rate
	const double exitFee = positionValue * (ExchangeRuleMgr->GetFutureMakerCommission() + ExchangeRuleMgr->GetFutureTakerCommission());
	// Calculate Entry Fee = Position Value × Free Rate
	const double entryFee = positionValue * (ExchangeRuleMgr->GetFutureMakerCommission() + ExchangeRuleMgr->GetFutureTakerCommission());
	// Calculate total fee = Entry Fee + Exit Fee
	const double totalFee = entryFee + exitFee;
    // Calculate the maximum position value = userSymbolMargin * userLeverageRate
    const double maximumPostitionValue = userInitialMargin * userLeverageRate;
    // Required margin = quantity × price / leverage
    const auto requiredMarginCash = positionValue / userLeverageRate;
	// Get future margin rate for the symbol from ExchangeRuleMgr
	const auto& symbolLeverageBracketInfo = ExchangeRuleMgr->GetFutureLeverageBracketByNotional(newUpstreamOrder.GetSymbol(), positionValue);
    // Calculate Maintenance Margin = Maintenance Margin Rate × Quantity × Entry Price
    const double maintenanceMarginRate = symbolLeverageBracketInfo.m_MaintMarginRate;
    const double maintenanceMargin = maintenanceMarginRate * positionValue;
    // Calculate liquidity price = Entry Price × [1 - (Initial Margin - Maintenance Margin - Total Fee) / Position Value]
    const double liquidityPrice = orderEntryPrice * (1 - (requiredMarginCash - maintenanceMargin - totalFee) / positionValue);
	// Check if user position size is within the maximum position size
	if (positionValue > maximumPostitionValue)
	{
		m_logger->Error("User position size exceeds maximum position size for order: " + newUpstreamOrder.ToStringOrder());
		return false; // Position size exceeds maximum allowed, cannot proceed with the order
	}
    // Check if the user account has sufficient margin cash
    bool hasSufficientMarginCash = userAccount->IsAccountHavingSufficientCashBalance("USDT", requiredMarginCash);
	if (!hasSufficientMarginCash)
	{
		m_logger->Warning("User account does not have sufficient margin cash for order: " + newUpstreamOrder.ToStringOrder());
		return false; // Insufficient margin, cannot proceed with the order
	}

    //Check the Order Book:
    //  Compare the upstream incoming order against the existing orders in the order book of the target symbol.
    bool hasLiquidity{ false };
    if (newUpstreamOrder.GetSide() == binapi::e_side::buy) // long positions
    {
        // For a Buy Side:
        // Check if the current market price is greater than the order entry price
        // Long positions get profit when the market price increases
        if (currentMarketPrice > orderEntryPrice)
        {
            // Calculate spread between current market price and order entry price
            const double spread = currentMarketPrice - orderEntryPrice;
            // Calculate profit based on the spread, position value, and number of contracts
            const double profitAndLost = spread * (positionValue / orderEntryPrice) * numberOfContracts;
			// Update the user's future asset balance with profit
			auto* userEditSession = m_userAccountManager->OpenEditSessionForFutureUserAccount(userAccount->GetUserAccountId());
			userEditSession->UpdateBalanceCash("USDT", profitAndLost, BalanceChangeEvent::PROFIT); // Update the margin with profit
        }
        else if (currentMarketPrice < orderEntryPrice)
        {
            // Calculate spread between order entry price and current market price
            const double spread = orderEntryPrice - currentMarketPrice;
            // Calculate loss based on the spread, position value, and number of contracts
            const double profitAndLost = spread * (positionValue / orderEntryPrice) * numberOfContracts;
			// Update the user's future asset balance with loss
			auto* userEditSession = m_userAccountManager->OpenEditSessionForFutureUserAccount(userAccount->GetUserAccountId());
            userEditSession->UpdateBalanceCash("USDT", profitAndLost, BalanceChangeEvent::LOSS); // Update the margin with loss
            // Liquidity price check
            if (currentMarketPrice <= liquidityPrice)
            {
                m_logger->Warning("Liquidity price reached for order: " + newUpstreamOrder.ToStringOrder());
                // Handle liquidation logic here, e.g., close position, notify user, etc.
                userEditSession->UpdateBalanceCash("USDT", requiredMarginCash, BalanceChangeEvent::LOSS); // Liquidate the position
            }
            // Check call margin condition
            else if (userFutureAssetInfo->initialMargin < maintenanceMargin)
            {
                m_logger->Warning("Margin call condition reached for order: " + newUpstreamOrder.ToStringOrder());
                // Handle margin call logic here, e.g., notify user, close position, etc.
            }
        }
        else
        {
            newUpstreamOrder.SetRemainingAmount(numberOfContracts);
        }
    }
    else if (newUpstreamOrder.GetSide() == binapi::e_side::sell) // short positions
    {
        // For a Sell Side:
        // Check if the current market price is less than the order entry price
        // Short positions get profit when the market price decreases
        if (currentMarketPrice < orderEntryPrice)
        {
            // Calculate spread between order entry price and current market price
            const double spread = orderEntryPrice - currentMarketPrice;
            // Calculate profit based on the spread, position value, and number of contracts
            const double profitAndLost = spread * (positionValue / orderEntryPrice) * numberOfContracts;
			// Update the user's future asset balance with profit
			auto* userEditSession = m_userAccountManager->OpenEditSessionForFutureUserAccount(userAccount->GetUserAccountId());
			userEditSession->UpdateBalanceCash("USDT", profitAndLost, BalanceChangeEvent::PROFIT); // Update the margin with profit
        }
        else if (currentMarketPrice > orderEntryPrice)
        {
            // Calculate spread between current market price and order entry price
            const double spread = currentMarketPrice - orderEntryPrice;
            // Calculate loss based on the spread, position value, and number of contracts
            const double profitAndLost = spread * (positionValue / orderEntryPrice) * numberOfContracts;
			// Update the user's future asset balance with loss
			auto* userEditSession = m_userAccountManager->OpenEditSessionForFutureUserAccount(userAccount->GetUserAccountId());
			userEditSession->UpdateBalanceCash("USDT", profitAndLost, BalanceChangeEvent::LOSS); // Update the margin with loss
            // Liquidity price check
            if (currentMarketPrice >= liquidityPrice)
            {
                m_logger->Warning("Liquidity price reached for order: " + newUpstreamOrder.ToStringOrder());
                // Handle liquidation logic here, e.g., close position, notify user, etc.
				userEditSession->UpdateBalanceCash("USDT", requiredMarginCash, BalanceChangeEvent::LOSS); // Liquidate the position
            }
            // Check call margin condition
            else if (userFutureAssetInfo->initialMargin < maintenanceMargin)
            {
                m_logger->Warning("Margin call condition reached for order: " + newUpstreamOrder.ToStringOrder());
                // Handle margin call logic here, e.g., notify user, close position, etc.
            }
        }
        else
        {
            newUpstreamOrder.SetRemainingAmount(numberOfContracts);
        }
    }
    newUpstreamOrder.SetUpdateTime(TimeUtils::GetEpochTimeTickNow());
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
