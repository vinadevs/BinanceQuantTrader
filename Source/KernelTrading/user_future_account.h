/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

// SAMPLING OF FUTURE ACCOUNT INFO RESPONSE FROM BINANCE API
//{
//    "feeTier": 0,
//        "canTrade" : true,
//        "canDeposit" : true,
//        "canWithdraw" : true,
//        "updateTime" : 1653471234567,
//        "totalInitialMargin" : "0.00000000",
//        "totalMaintMargin" : "0.00000000",
//        "totalWalletBalance" : "1000.00000000",
//        "totalUnrealizedProfit" : "5.00000000",
//        "totalMarginBalance" : "1005.00000000",
//        "totalPositionInitialMargin" : "10.00000000",
//        "totalOpenOrderInitialMargin" : "0.00000000",
//        "totalCrossWalletBalance" : "995.00000000",
//        "totalCrossUnPnl" : "0.00000000",
//        "availableBalance" : "990.00000000",
//        "maxWithdrawAmount" : "990.00000000",
//        "assets" : [
//    {
//        "asset": "USDT",
//            "walletBalance" : "1000.00000000",
//            "unrealizedProfit" : "5.00000000",
//            "marginBalance" : "1005.00000000",
//            "maintMargin" : "0.00000000",
//            "initialMargin" : "10.00000000",
//            "positionInitialMargin" : "10.00000000",
//            "openOrderInitialMargin" : "0.00000000",
//            "crossWalletBalance" : "995.00000000",
//            "crossUnPnl" : "0.00000000",
//            "availableBalance" : "990.00000000",
//            "maxWithdrawAmount" : "990.00000000",
//            "marginAvailable" : true,
//            "updateTime" : 1653471234567
//    }
//        ],
//        "positions": [
//    {
//        "symbol": "BTCUSDT",
//            "initialMargin" : "10.00000000",
//            "maintMargin" : "0.50000000",
//            "unrealizedProfit" : "2.00000000",
//            "positionInitialMargin" : "10.00000000",
//            "openOrderInitialMargin" : "0.00000000",
//            "leverage" : "10",
//            "isolated" : false,
//            "entryPrice" : "25000.00000",
//            "maxNotional" : "250000.00000000",
//            "positionSide" : "BOTH",
//            "positionAmt" : "0.00400000",
//            "notional" : "100.00000000",
//            "isolatedWallet" : "0.00000000",
//            "updateTime" : 1653471234567
//    }
//        ]
//}

namespace KernelTrading {

	// Enum class to represent different types of balance change events
	enum class BalanceChangeEvent : unsigned {
		PROFIT, // Indicates a profit event, such as realized profit from a trade
		LOSS,   // Indicates a loss event, such as realized loss from a trade
		DEPOSIT, // Indicates a deposit event, such as adding funds to the account
		WITHDRAWAL // Indicates a withdrawal event, such as removing funds from the account
	};

    // Holds detailed info about a user's balance in a specific asset (e.g. USDT, BUSD)
    struct DLL_CLASS AssetInfo {
        AssetInfo() = default; // Default constructor
        AssetInfo(
            const std::string& asset_,
            double walletBalance_,
            double unrealizedProfit_,
            double marginBalance_,
            double maintMargin_,
            double initialMargin_,
            double positionInitialMargin_,
            double openOrderInitialMargin_,
            double crossWalletBalance_,
            double crossUnPnl_,
            double availableBalance_,
            double maxWithdrawAmount_,
            bool marginAvailable_,
            int64_t updateTime_
        )
            : asset(asset_),
              walletBalance(walletBalance_),
              unrealizedProfit(unrealizedProfit_),
              marginBalance(marginBalance_),
              maintMargin(maintMargin_),
              initialMargin(initialMargin_),
              positionInitialMargin(positionInitialMargin_),
              openOrderInitialMargin(openOrderInitialMargin_),
              crossWalletBalance(crossWalletBalance_),
              crossUnPnl(crossUnPnl_),
              availableBalance(availableBalance_),
              maxWithdrawAmount(maxWithdrawAmount_),
              marginAvailable(marginAvailable_),
              updateTime(updateTime_)
        {}

        std::string asset;                    // Asset name (e.g. "USDT")
        double walletBalance{0.0};            // Total wallet balance
        double unrealizedProfit{0.0};         // Unrealized profit/loss
        double marginBalance{0.0};            // Wallet + unrealized profit
        double maintMargin{0.0};              // Maintenance margin required
        double initialMargin{0.0};            // Total initial margin
        double positionInitialMargin{0.0};    // Initial margin from positions
        double openOrderInitialMargin{0.0};   // Initial margin from open orders
        double crossWalletBalance{0.0};       // Wallet balance in cross margin mode
        double crossUnPnl{0.0};               // Unrealized PnL in cross margin
        double availableBalance{0.0};         // Free balance available to trade
        double maxWithdrawAmount{0.0};        // Max amount allowed to withdraw
        bool marginAvailable{0.0};            // Whether margin is available
        int64_t updateTime{0};                // Last update time in milliseconds
    };

    // Represents the user's position for a trading symbol (e.g. BTCUSDT)
    struct DLL_CLASS PositionInfo {
        PositionInfo() = default; // Default constructor
        PositionInfo(
            const std::string& symbol_,
            double initialMargin_,
            double maintMargin_,
            double unrealizedProfit_,
            double positionInitialMargin_,
            double openOrderInitialMargin_,
            double leverage_,
            bool isolated_,
            double entryPrice_,
            double maxNotional_,
            const std::string& positionSide_,
            double positionAmt_,
            double notional_,
            double isolatedWallet_,
            int64_t updateTime_
        )
            : symbol(symbol_),
              initialMargin(initialMargin_),
              maintMargin(maintMargin_),
              unrealizedProfit(unrealizedProfit_),
              positionInitialMargin(positionInitialMargin_),
              openOrderInitialMargin(openOrderInitialMargin_),
              leverage(leverage_),
              isolated(isolated_),
              entryPrice(entryPrice_),
              maxNotional(maxNotional_),
              positionSide(positionSide_),
              positionAmt(positionAmt_),
              notional(notional_),
              isolatedWallet(isolatedWallet_),
              updateTime(updateTime_)
        {}

        std::string symbol;                   // Trading pair symbol
        double initialMargin{0.0};            // Initial margin for this position
        double maintMargin{0.0};              // Maintenance margin for this position
        double unrealizedProfit{0.0};         // Unrealized profit/loss
        double positionInitialMargin{0.0};    // Initial margin for position size
        double openOrderInitialMargin{0.0};   // Initial margin reserved for open orders
        double leverage{0.0};                 // Leverage used (e.g. 10x)
        bool isolated{0.0};                   // Whether this is isolated margin
        double entryPrice{0.0};               // Average entry price
        double maxNotional{0.0};              // Max notional value allowed
        std::string positionSide;             // LONG, SHORT or BOTH
        double positionAmt{0.0};              // Position quantity
        double notional{0.0};                 // Position value in quote asset
        double isolatedWallet{0.0};           // Balance used for isolated margin
        int64_t updateTime{0};                // Last update time in milliseconds
    };

    // Root class to represent the entire user account futures response from Binance
    class DLL_CLASS UserFutureAccount final {
    public:
		// Default constructor
		UserFutureAccount() = default;
		// Constructor that initializes from a user ID and JSON object
		UserFutureAccount(const std::string& userID, const std::string& userConfigPath, const nlohmann::json& j)
        : m_userAccountId(userID) {
            ReadUserConfig(userConfigPath);
			FromJson(j);
		}

        const AssetInfo* LookupFutureAssetInfo(const std::string& currency) const;
		const PositionInfo* LookupFuturePositionInfo(const std::string& symbol) const;

		bool IsAccountHavingSufficientCashBalance(const std::string& currency, const double requiredMarginCash) const;

		void UpdateAssetBalanceCash(const std::string& currency, const double pnl, const BalanceChangeEvent event);
		void UpdatePositionCash(const std::string& symbol, const double pnl, const BalanceChangeEvent event);
		void RealizedPNLPositions(const std::string& currency);
        void RealizedPNLPosition(const std::string& currency, const std::string& symbol);

        // Setters for account-level basic flags and tier
        void SetFeeTier(int feeTier) { m_feeTier = feeTier; }
        void SetCanTrade(bool canTrade) { m_canTrade = canTrade; }
        void SetCanDeposit(bool canDeposit) { m_canDeposit = canDeposit; }
        void SetCanWithdraw(bool canWithdraw) { m_canWithdraw = canWithdraw; }
        void SetUpdateTime(int64_t updateTime) { m_updateTime = updateTime; }

        // Setters for aggregated margin and wallet info
        void SetTotalInitialMargin(double value) { m_totalInitialMargin = value; }
        void SetTotalMaintMargin(double value) { m_totalMaintMargin = value; }
        void SetTotalWalletBalance(double value) { m_totalWalletBalance = value; }
        void SetTotalUnrealizedProfit(double value) { m_totalUnrealizedProfit = value; }
        void SetTotalMarginBalance(double value) { m_totalMarginBalance = value; }
        void SetTotalPositionInitialMargin(double value) { m_totalPositionInitialMargin = value; }
        void SetTotalOpenOrderInitialMargin(double value) { m_totalOpenOrderInitialMargin = value; }
        void SetTotalCrossWalletBalance(double value) { m_totalCrossWalletBalance = value; }
        void SetTotalCrossUnPnl(double value) { m_totalCrossUnPnl = value; }
        void SetAvailableBalance(double value) { m_availableBalance = value; }
        void SetMaxWithdrawAmount(double value) { m_maxWithdrawAmount = value; }

        // Setters for detailed lists
        void SetAssets(const std::vector<AssetInfo>& assets) { m_assets = assets; }
        void SetPositions(const std::vector<PositionInfo>& positions) { m_positions = positions; }
		void AddAsset(const AssetInfo& asset) { m_assets.emplace_back(asset); }
		void AddPosition(const PositionInfo& position) { m_positions.emplace_back(position); }
        void ClearAssets() { m_assets.clear(); }
		void ClearPositions() { m_positions.clear(); }
        bool RemoteAsset(const std::string& asset);
		bool RemotePosition(const std::string& symbol);

        // Accessor functions for main account info
        int GetFeeTier() const { return m_feeTier; }
        bool CanTrade() const { return m_canTrade; }
        bool CanDeposit() const { return m_canDeposit; }
        bool CanWithdraw() const { return m_canWithdraw; }
        int64_t GetUpdateTime() const { return m_updateTime; }

        // Accessors for margin and balance fields
        double GetTotalInitialMargin() const { return m_totalInitialMargin; }
        double GetTotalMaintMargin() const { return m_totalMaintMargin; }
        double GetTotalWalletBalance() const { return m_totalWalletBalance; }
        double GetTotalUnrealizedProfit() const { return m_totalUnrealizedProfit; }
        double GetTotalMarginBalance() const { return m_totalMarginBalance; }
        double GetTotalPositionInitialMargin() const { return m_totalPositionInitialMargin; }
        double GetTotalOpenOrderInitialMargin() const { return m_totalOpenOrderInitialMargin; }
        double GetTotalCrossWalletBalance() const { return m_totalCrossWalletBalance; }
        double GetTotalCrossUnPnl() const { return m_totalCrossUnPnl; }
        double GetAvailableBalance() const { return m_availableBalance; }
        double GetMaxWithdrawAmount() const { return m_maxWithdrawAmount; }

        // Lists of asset-level and position-level info
        const std::vector<AssetInfo>& GetAssets() const { return m_assets; }
        const std::vector<PositionInfo>& GetPositions() const { return m_positions; }

		// User account ID accessor
		const std::string& GetUserAccountId() const { return m_userAccountId; }
		void SetUserAccountId(const std::string& userAccountId) { m_userAccountId = userAccountId; }

        // Parses the Binance JSON response into this class
        void FromJson(const nlohmann::json& j);

    private:
		// Reads user configuration from a file
        void ReadUserConfig(const std::string& userConfigPath) {}

        // Account-level basic flags and tier
        int m_feeTier{0};                // Binance fee tier level
        bool m_canTrade{false};          // Whether trading is allowed
        bool m_canDeposit{false};        // Whether deposits are allowed
        bool m_canWithdraw{false};       // Whether withdrawals are allowed
        int64_t m_updateTime{0};         // Last update time (ms)

        // Aggregated margin and wallet info
        double m_totalInitialMargin{0.0};
        double m_totalMaintMargin{0.0};
        double m_totalWalletBalance{0.0};
        double m_totalUnrealizedProfit{0.0};
        double m_totalMarginBalance{0.0};
        double m_totalPositionInitialMargin{0.0};
        double m_totalOpenOrderInitialMargin{0.0};
        double m_totalCrossWalletBalance{0.0};
        double m_totalCrossUnPnl{0.0};
        double m_availableBalance{0.0};
        double m_maxWithdrawAmount{0.0};

        // Detailed lists
        std::vector<AssetInfo> m_assets;           // Per-asset balances
        std::vector<PositionInfo> m_positions;     // Open positions

		// User account ID
		std::string m_userAccountId; // Unique identifier for the user account
    };
} // namespace ExchangeSimulator
