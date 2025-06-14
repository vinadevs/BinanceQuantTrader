#pragma once
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

namespace ExchangeSimulator {

	// Enum class to represent different types of balance change events
	enum class BalanceChangeEvent : unsigned {
		PROFIT, // Indicates a profit event, such as realized profit from a trade
		LOSS,   // Indicates a loss event, such as realized loss from a trade
		DEPOSIT, // Indicates a deposit event, such as adding funds to the account
		WITHDRAWAL // Indicates a withdrawal event, such as removing funds from the account
	};

    // Holds detailed info about a user's balance in a specific asset (e.g. USDT, BUSD)
    struct AssetInfo {
        std::string asset;               // Asset name (e.g. "USDT")
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
        int64_t updateTime{0};              // Last update time in milliseconds
    };

    // Represents the user's position for a trading symbol (e.g. BTCUSDT)
    struct PositionInfo {
        std::string symbol;              // Trading pair symbol
        double initialMargin{0.0};            // Initial margin for this position
        double maintMargin{0.0};              // Maintenance margin for this position
        double unrealizedProfit{0.0};         // Unrealized profit/loss
        double positionInitialMargin{0.0};    // Initial margin for position size
        double openOrderInitialMargin{0.0};   // Initial margin reserved for open orders
        double leverage{0.0};                 // Leverage used (e.g. 10x)
        bool isolated{0.0};                   // Whether this is isolated margin
        double entryPrice{0.0};               // Average entry price
        double maxNotional{0.0};              // Max notional value allowed
        std::string positionSide;        // LONG, SHORT or BOTH
        double positionAmt{0.0};              // Position quantity
        double notional{0.0};                 // Position value in quote asset
        double isolatedWallet{0.0};           // Balance used for isolated margin
        int64_t updateTime{0};              // Last update time in milliseconds
    };

    // Root class to represent the entire user account futures response from Binance
    class UserAccountFuture {
    public:
		// Default constructor
		UserAccountFuture() = default;
		// Constructor that initializes from a user ID and JSON object
		UserAccountFuture(const std::string& userID, const nlohmann::json& j) {
			FromJson(j);
		}

        const AssetInfo* LookupFutureAssetInfo(const std::string& currency) const;

		bool IsAccountHavingSufficientCashBalance(const std::string& currency, const double requiredMarginCash) const;

		void UpdateBalanceCash(const std::string& currency, const double pnl, const BalanceChangeEvent event);

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

    private:
        // Parses the Binance JSON response into this class
        void FromJson(const nlohmann::json& j);

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
