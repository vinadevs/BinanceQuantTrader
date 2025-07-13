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
    UserAccountManager* userAccountManager,
    UserTradeProfileManager* userTradeProfileManager)
    : Participant(ParticipantType::REAL_TIME_FUTURE_MARKET_DATA, userAccountManager, userTradeProfileManager)
{
}

RTMarketFutureParticipant::~RTMarketFutureParticipant() {}

// ────────────────────────────────────────────────────────────────────────────────
//  RTMarketFutureParticipant::TryToMatchOrder
//  – validates the order
//  – checks position / margin limits
//  – books PnL, margin‑call, or liquidation
//  – returns true  ⇒ order accepted / kept in book
//            false ⇒ rejected up‑front
// ────────────────────────────────────────────────────────────────────────────────
bool RTMarketFutureParticipant::TryToMatchOrder(OrderManagement::BinanceNewOrder& order)
{
    std::unique_lock<std::mutex> lock(m_mutex);               // critical section

    //---------------------------------------------------------------------------
    // 1. Sanity checks & fast failures
    //---------------------------------------------------------------------------
    auto* userAccount = m_userAccountManager->LookupFutureUserAccount(order.GetUserAccountID());
    if (!userAccount)
    {
		return HandleRejectedOrder("User account not found for order: ", order);
    }

    const auto* priceMgr = m_downstreamFuturePriceManagers[order.GetSymbol()].get();
    if (!priceMgr)
    {
		return HandleRejectedOrder("Price manager not found for order: ", order);
    }

    const auto* assetInfo = userAccount->LookupFutureAssetInfo(order.GetStableCurrency());
    if (!assetInfo)
    {
		return HandleRejectedOrder("Asset info not found for order: ", order);
    }

    //---------------------------------------------------------------------------
    // 2. Pull immutable parameters
    //---------------------------------------------------------------------------
    const double entryPrice = order.GetPrice();
    const double marketPrice = priceMgr->GetCurrentMarketPrice();
    const double contracts = order.GetAmount();

    const auto& tradeProfile = m_userTradeProfileManager->LookupUserTradeProfile(order.GetUserAccountID());
    const double leverage = tradeProfile.GetLeverageRate();

    if (!Finance::IsLeverageRatioValid(leverage)) // defensive: avoid div‑by‑zero
    {
		return HandleRejectedOrder("Invalid leverage ratio for order: ", order);
    }

    //---------------------------------------------------------------------------
    // 3. Risk constants
    //---------------------------------------------------------------------------
    const double notional = entryPrice * contracts;          // position value
    // Calculate Exit Fee = Position Value × Free Rate
    const double exitFee = notional * (ExchangeRuleMgr->GetFutureMakerCommission() + ExchangeRuleMgr->GetFutureTakerCommission());
    // Calculate Entry Fee = Position Value × Free Rate
    const double entryFee = notional * (ExchangeRuleMgr->GetFutureMakerCommission() + ExchangeRuleMgr->GetFutureTakerCommission());
    // Calculate total fee = Entry Fee + Exit Fee
    const double totalFee = entryFee + exitFee;

    const auto& bracket = ExchangeRuleMgr
        ->GetFutureLeverageBracketByNotional(order.GetSymbol(), notional);
    const double maintMargin = bracket.m_MaintMarginRate * notional;
    const double positionInitMargin = notional / leverage;
    const double requiredMargin = notional / leverage;

	order.SetFutureInitialMarginPrice(positionInitMargin);
	order.SetFutureMaintainingMarginPrice(maintMargin);

    //---------------------------------------------------------------------------
    // 4. Pre‑trade checks (size & cash)
    //---------------------------------------------------------------------------
    if (notional > positionInitMargin * leverage)
    {
		return HandleRejectedOrder("Notional value exceeds user initial margin * leverage for order: ", order);
    }

    if (!userAccount->IsAccountHavingSufficientCashBalance(order.GetStableCurrency(), requiredMargin))
    {
		return HandleRejectedOrder("User account does not have sufficient cash balance for required margin for order: ", order);
    }

    //---------------------------------------------------------------------------
    // 5. Liquidation price (sign‑aware)
    //---------------------------------------------------------------------------
    const bool   isLong = (order.GetSide() == binapi::e_side::buy);
    const double bracketTerm = (positionInitMargin - maintMargin - totalFee) / notional;
    const double liqPrice = isLong
        ? entryPrice * (1.0 - bracketTerm)
        : entryPrice * (1.0 + bracketTerm);

    //---------------------------------------------------------------------------
    // 6. PnL booking (mock matching)
    //---------------------------------------------------------------------------
    const double priceDiff = marketPrice - entryPrice;
    const double pnl = isLong ? priceDiff * contracts
        : -priceDiff * contracts;

	if (pnl != 0.0) // Only update balance if there is a PnL change
    {
        auto* session = m_userAccountManager->OpenEditSessionForFutureUserAccount(
            userAccount->GetUserAccountId());

        const auto evt = (pnl > 0.0)
            ? KernelTrading::BalanceChangeEvent::PROFIT
            : KernelTrading::BalanceChangeEvent::LOSS;

        session->UpdateBalanceCash(order.GetStableCurrency(), std::fabs(pnl), evt);
    }

    //---------------------------------------------------------------------------
    // 7. Margin‑call / liquidation checks
    //---------------------------------------------------------------------------
    if ((isLong && marketPrice <= liqPrice) ||
        (!isLong && marketPrice >= liqPrice))
    {
        auto* session = m_userAccountManager->OpenEditSessionForFutureUserAccount(
            userAccount->GetUserAccountId());

        session->UpdateBalanceCash(order.GetStableCurrency(), requiredMargin,
            KernelTrading::BalanceChangeEvent::LOSS);

        m_logger->Warning("Liquidation triggered for order: " + order.ToStringOrder());
		// Update filled ack to upstream
        order.SetRemainingAmount(0.0);
        order.SetFilledAmount(contracts);
		order.SetFutureLiquidationPrice(liqPrice);
		order.SetOrderStatus(BinanceNewOrderStatus::LIQUIDATED);
    }
    else if (positionInitMargin < maintMargin)
    {
        m_logger->Warning("Margin call threshold reached for order: " + order.ToStringOrder());
		order.SetOrderStatus(BinanceNewOrderStatus::MARGIN_CALL);
        order.SetRemainingAmount(contracts);
        order.SetFilledAmount(0.0);
    }
    else
    {
        // No fill in this tick – keep the order on the book
        order.SetRemainingAmount(contracts);
		order.SetFilledAmount(0.0);
    }

    order.SetUpdateTime(TimeUtils::GetEpochTimeTickNow());
    return true; // Order accepted
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

bool RTMarketFutureParticipant::HandleRejectedOrder(const std::string& message, OrderManagement::BinanceNewOrder& order)
{
    m_logger->Error(message + order.ToStringOrder());
    order.SetRemainingAmount(order.GetAmount());
    order.SetFilledAmount(0.0);
    order.SetOrderStatus(BinanceNewOrderStatus::REJECTED);
    return false; // Order rejected
}
