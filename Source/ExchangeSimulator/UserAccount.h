/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include <string>
#include <map>
#include <iostream>

namespace ExchangeSimulator {

    using AssetSymbol = std::string;

    // A binance asset information
    struct AssetBalance
    {
        AssetSymbol m_symbol; // asset name
        double m_free{ 0 }; // amount can be traded
        double m_locked{ 0 }; // amount is being locked

        AssetBalance() = default;

        AssetBalance(const AssetSymbol& symbol, double free, double locked)
            : m_symbol(symbol), m_free(free), m_locked(locked)
        {}

        friend std::ostream& operator<<(std::ostream& os, const AssetBalance& balance)
        {
            os << "AssetBalance { "
                << "symbol: \"" << balance.m_symbol << "\", "
                << "free: " << balance.m_free << ", "
                << "locked: " << balance.m_locked
                << " }";
            return os;
        }
    };

    using AssetBalances = std::map<AssetSymbol, AssetBalance>;

    /**
     * @struct UserAccount
     * @brief Represents a user's account details on the Binance platform.
     *
     * This structure holds the essential information for managing and tracking
     * a user's account activities on Binance. It includes fields for personal
     * identification, balance tracking, and trading preferences.
    */

    struct UserAccount 
    {
        UserAccount() = default;

        UserAccount(
            std::string userId,
            std::size_t makerCommission,
            std::size_t takerCommission,
            std::size_t buyerCommission,
            std::size_t sellerCommission,
            bool canWithdraw,
            bool canDeposit,
            std::size_t updateTime);

        std::string m_userId;
        std::size_t m_makerCommission{ 0 };
        std::size_t m_takerCommission{ 0 };
        std::size_t m_buyerCommission{ 0 };
        std::size_t m_sellerCommission{ 0 };
        bool m_canWithdraw{ false };
        bool m_canDeposit { false };
        std::size_t m_updateTime{ 0 };
  
        AssetBalances m_assetBalances;

        friend std::ostream& operator<<(std::ostream& os, const UserAccount& account)
        {
            os << "UserAccount { "
                << "userId: " << account.m_userId << ", "
                << "makerCommission: " << account.m_makerCommission << ", "
                << "takerCommission: " << account.m_takerCommission << ", "
                << "buyerCommission: " << account.m_buyerCommission << ", "
                << "sellerCommission: " << account.m_sellerCommission << ", "
                << "canTrade: " << std::boolalpha << account.IsAccountEligibleToTrade() << ", "
                << "canWithdraw: " << account.m_canWithdraw << ", "
                << "canDeposit: " << account.m_canDeposit << ", "
                << "updateTime: " << account.m_updateTime << ", ";

            for (const auto& asset : account.m_assetBalances)
            {
                os << asset.second << ", ";
            }

            os << " }";

            return os;
        }

        bool IsAccountEligibleToTrade() const;
    };
};