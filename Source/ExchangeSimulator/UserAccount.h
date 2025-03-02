/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <mutex>

namespace ExchangeSimulator {

    using AssetSymbol = std::string;

    struct FiatBalance final
    {
        enum class CurrencyType : unsigned
        {
            VND,
            YEN,
            USD,
        };

        static std::string GetCurrencyTypeStr(const CurrencyType currencyType)
        {
            switch (currencyType)
            {
            case CurrencyType::VND:
               return "VND";
            case CurrencyType::YEN:
               return "YEN";
            case CurrencyType::USD:
               return "USD";
            default:
               return "";
            }
        }

        double m_cashAmount{ 0 }; // cash amount fiat
        CurrencyType m_currencyType{ 0 }; // type of money

        FiatBalance() = default;

        FiatBalance(double cashAmount, const CurrencyType currencyType)
            : m_cashAmount(cashAmount), m_currencyType(currencyType)
        {}

        friend std::ostream& operator<<(std::ostream& os, const FiatBalance& balance)
        {
            os << "FiatBalance { "
               << "cashAmount: \"" << balance.m_cashAmount << "\", "
               << "currencyType: " << GetCurrencyTypeStr(balance.m_currencyType)
               << " }";
            return os;
        }
    };
    
    struct StableCoinUSDTBalance final
    {
        double m_usdtAmount{ 10000 }; // amount usdt
       
        StableCoinUSDTBalance() = default;

        StableCoinUSDTBalance(double usdtAmount)
            : m_usdtAmount(usdtAmount)
        {}

        friend std::ostream& operator<<(std::ostream& os, const StableCoinUSDTBalance& balance)
        {
            os << "StableCoinUSDTBalance { "
               << "usdtAmount: " << balance.m_usdtAmount
               << " }";
            return os;
        }
    };

    // A binance asset information
    struct AssetBalance final
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

    using AssetBalances = std::unordered_map<AssetSymbol, AssetBalance>;

    /**
     * @struct UserAccount
     * @brief Represents a user's account details on the Binance platform.
     *
     * This structure holds the essential information for managing and tracking
     * a user's account activities on Binance. It includes fields for personal
     * identification, balance tracking, and trading preferences.
    */

    struct UserAccount final
    {
        UserAccount() = default;

        UserAccount(const std::string& userConfigPath);

        // balances
        AssetBalances m_assetBalances;
        FiatBalance m_fiatBalance;
        StableCoinUSDTBalance m_usdtBalance;

        friend std::ostream& operator<<(std::ostream& os, const UserAccount& account)
        {
            os << "UserAccount { "
               << "userId: " << account.m_userId << ", "
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

        const std::string& GetUserId() const { return m_userId; }
        std::size_t GetUpdateTime() const { return m_updateTime; }
        bool IsAccountEligibleToWithdraw() const;
        bool IsAccountEligibleToDeposit() const;
        bool IsAccountEligibleToTrade() const;
        bool IsAccountHavingAssets() const;

        AssetBalance& LookupAssetBalance(const AssetSymbol& symbol);
    private:
        void EnableUserAccountControls();
        void DisableUserAccountControls();

        std::string m_userId; // must be unique string ID
        std::size_t m_updateTime{ 0 }; // when the user account data changed?
        bool m_canWithdraw{ false };
        bool m_canDeposit{ false };
        bool m_canTrade{ false };
        std::mutex m_mutex;
    };
};