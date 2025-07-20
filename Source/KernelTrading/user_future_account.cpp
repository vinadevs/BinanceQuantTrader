/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include "user_future_account.h"

using namespace KernelTrading;

// Helper to safely parse a double from JSON string fields
static double ParseDouble(const nlohmann::json& j, const std::string& key) {
    return j.contains(key) ? std::stod(j.value(key, "0")) : 0.0;
}

void UserFutureAccount::FromJson(const nlohmann::json& j) {
    // Top-level fields
    m_feeTier = j.value("feeTier", 0);
    m_canTrade = j.value("canTrade", false);
    m_canDeposit = j.value("canDeposit", false);
    m_canWithdraw = j.value("canWithdraw", false);
    m_updateTime = j.value("updateTime", 0);

    // Margin and balance aggregates
    m_totalInitialMargin = ParseDouble(j, "totalInitialMargin");
    m_totalMaintMargin = ParseDouble(j, "totalMaintMargin");
    m_totalWalletBalance = ParseDouble(j, "totalWalletBalance");
    m_totalUnrealizedProfit = ParseDouble(j, "totalUnrealizedProfit");
    m_totalMarginBalance = ParseDouble(j, "totalMarginBalance");
    m_totalPositionInitialMargin = ParseDouble(j, "totalPositionInitialMargin");
    m_totalOpenOrderInitialMargin = ParseDouble(j, "totalOpenOrderInitialMargin");
    m_totalCrossWalletBalance = ParseDouble(j, "totalCrossWalletBalance");
    m_totalCrossUnPnl = ParseDouble(j, "totalCrossUnPnl");
    m_availableBalance = ParseDouble(j, "availableBalance");
    m_maxWithdrawAmount = ParseDouble(j, "maxWithdrawAmount");

    // Parse asset details
    m_assets.clear();
    if (j.contains("assets")) {
        for (const auto& a : j["assets"]) {
            AssetInfo asset;
            asset.asset = a.value("asset", "");
            asset.walletBalance = ParseDouble(a, "walletBalance");
            asset.unrealizedProfit = ParseDouble(a, "unrealizedProfit");
            asset.marginBalance = ParseDouble(a, "marginBalance");
            asset.maintMargin = ParseDouble(a, "maintMargin");
            asset.initialMargin = ParseDouble(a, "initialMargin");
            asset.positionInitialMargin = ParseDouble(a, "positionInitialMargin");
            asset.openOrderInitialMargin = ParseDouble(a, "openOrderInitialMargin");
            asset.crossWalletBalance = ParseDouble(a, "crossWalletBalance");
            asset.crossUnPnl = ParseDouble(a, "crossUnPnl");
            asset.availableBalance = ParseDouble(a, "availableBalance");
            asset.maxWithdrawAmount = ParseDouble(a, "maxWithdrawAmount");
            asset.marginAvailable = a.value("marginAvailable", false);
            asset.updateTime = a.value("updateTime", 0);
            m_assets.push_back(asset);
        }
    }
}

const AssetInfo* UserFutureAccount::LookupFutureAssetInfo(const std::string& currency) const {
    for (const auto& asset : m_assets) {
        if (asset.asset == currency) {
            return &asset;
        }
    }
    return nullptr; // Not found
}

const PositionInfo* UserFutureAccount::LookupFuturePositionInfo(const std::string& symbol) const
{
	for (const auto& position : m_positions) {
		if (position.symbol == symbol) {
			return &position;
		}
	}
	return nullptr; // Not found
}

bool UserFutureAccount::IsAccountHavingSufficientCashBalance(
    const std::string& currency,
    const double requiredMarginCash) const
{
	const auto* assetInfo = LookupFutureAssetInfo(currency);
	if (assetInfo) {
		return assetInfo->availableBalance >= requiredMarginCash;
	}
	return false; // Asset not found
}

void UserFutureAccount::UpdateAssetBalanceCash(const std::string& currency, const double pnl, const BalanceChangeEvent event)
{
	auto* assetInfo = const_cast<AssetInfo*>(LookupFutureAssetInfo(currency));
	if (assetInfo) {
		switch (event) {
		case BalanceChangeEvent::DEPOSIT:
			assetInfo->availableBalance += pnl; // pnl is the deposit amount
			break;
		case BalanceChangeEvent::WITHDRAWAL:
			assetInfo->availableBalance -= pnl; // pnl is the withdrawal amount
			break;
		}
	}
}

void UserFutureAccount::UpdatePositionCash(
    const std::string& symbol, const double pnl, const BalanceChangeEvent event)
{
	auto it = std::find_if(m_positions.begin(), m_positions.end(),
		[&symbol](const PositionInfo& p) { return p.symbol == symbol; });
	if (it != m_positions.end()) {
		switch (event) {
        case BalanceChangeEvent::PROFIT: {
            it->unrealizedProfit += pnl;
        }
			break;
		case BalanceChangeEvent::LOSS:
			it->unrealizedProfit -= pnl;
			it->initialMargin -= pnl; // Adjust initial margin if needed
			break;
		}
	}
}

void UserFutureAccount::RealizedPNLPositions(const std::string& currency)
{
    auto* assetInfo = const_cast<AssetInfo*>(LookupFutureAssetInfo(currency));
    if (assetInfo) {
		for (auto& position : m_positions) {
			// Realize the profit/loss for each position
			if (position.unrealizedProfit > 0.0) {
				assetInfo->availableBalance += position.unrealizedProfit; // Update available balance
			}
		}
    }
}

void UserFutureAccount::RealizedPNLPosition(const std::string& currency, const std::string& symbol)
{
	const auto it = std::find_if(m_positions.begin(), m_positions.end(),
		[&symbol](const PositionInfo& p) { return p.symbol == symbol; });
	if (it != m_positions.end()) {
		auto* assetInfo = const_cast<AssetInfo*>(LookupFutureAssetInfo(currency));
		if (assetInfo) {
			// Realize the profit/loss for the specific position
			if (it->unrealizedProfit > 0.0) {
				assetInfo->availableBalance += it->unrealizedProfit; // Update available balance
			}
		}
	}
}

bool UserFutureAccount::RemoteAsset(const std::string& asset) {
	const auto it = std::remove_if(m_assets.begin(), m_assets.end(),
		[&asset](const AssetInfo& a) { return a.asset == asset; });
	if (it != m_assets.end()) {
		m_assets.erase(it, m_assets.end());
		return true; // Successfully removed
	}
	return false; // Asset not found
}

bool UserFutureAccount::RemotePosition(const std::string& symbol) {
    const auto it = std::remove_if(m_positions.begin(), m_positions.end(),
		[&symbol](const PositionInfo& p) { return p.symbol == symbol; });
	if (it != m_positions.end()) {
		m_positions.erase(it, m_positions.end());
		return true; // Successfully removed
	}
	return false; // Position not found
}
