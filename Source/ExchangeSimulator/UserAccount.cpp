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
UserAccount::UserAccount(std::string userId)
    : m_userId(std::move(userId))
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

