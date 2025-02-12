/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "UserAccount.h"

#include <algorithm>
#include <chrono>

using namespace ExchangeSimulator;

// Constructor with parameters
UserAccount::UserAccount(
    std::string userId,
    std::size_t makerCommission,
    std::size_t takerCommission,
    std::size_t buyerCommission,
    std::size_t sellerCommission,
    bool canWithdraw,
    bool canDeposit,
    std::size_t updateTime)
    : m_userId(userId),
    m_makerCommission(makerCommission),
    m_takerCommission(takerCommission),
    m_buyerCommission(buyerCommission),
    m_canWithdraw(canWithdraw),
    m_canDeposit(canDeposit),
    m_updateTime(updateTime) 
{
    m_updateTime = static_cast<std::size_t>(std::chrono::system_clock::now().time_since_epoch().count());
}

bool UserAccount::IsAccountEligibleToWithdraw() const
{
    return m_canWithdraw;
}

bool UserAccount::IsAccountEligibleToDeposit() const
{
    return m_canDeposit;
}

bool UserAccount::IsAccountEligibleToTrade() const
{
    return m_canTrade;
}

bool UserAccount::IsAccountHavingAssets() const
{
    return std::any_of(m_assetBalances.begin(), m_assetBalances.end(),
        [](const auto& asset) { return asset.second.m_free > 0; });
}

