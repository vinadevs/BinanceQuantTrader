/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include "UserAccountFuture.h"

using namespace ExchangeSimulator;

// Helper to safely parse a double from JSON string fields
static double ParseDouble(const nlohmann::json& j, const std::string& key) {
    return j.contains(key) ? std::stod(j.value(key, "0")) : 0.0;
}

void UserAccountFuture::FromJson(const nlohmann::json& j) {
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

    // Parse position details
    m_positions.clear();
    if (j.contains("positions")) {
        for (const auto& p : j["positions"]) {
            PositionInfo pos;
            pos.symbol = p.value("symbol", "");
            pos.initialMargin = ParseDouble(p, "initialMargin");
            pos.maintMargin = ParseDouble(p, "maintMargin");
            pos.unrealizedProfit = ParseDouble(p, "unrealizedProfit");
            pos.positionInitialMargin = ParseDouble(p, "positionInitialMargin");
            pos.openOrderInitialMargin = ParseDouble(p, "openOrderInitialMargin");
            pos.leverage = ParseDouble(p, "leverage");
            pos.isolated = p.value("isolated", false);
            pos.entryPrice = ParseDouble(p, "entryPrice");
            pos.maxNotional = ParseDouble(p, "maxNotional");
            pos.positionSide = p.value("positionSide", "");
            pos.positionAmt = ParseDouble(p, "positionAmt");
            pos.notional = ParseDouble(p, "notional");
            pos.isolatedWallet = ParseDouble(p, "isolatedWallet");
            pos.updateTime = p.value("updateTime", 0);
            m_positions.push_back(pos);
        }
    }
}

const AssetInfo* UserAccountFuture::LookupFutureAssetInfo(const std::string& currency) const {
    for (const auto& asset : m_assets) {
        if (asset.asset == currency) {
            return &asset;
        }
    }
    return nullptr; // Not found
}

bool UserAccountFuture::IsAccountHavingSufficientCashBalance(
    const std::string& currency,
    const double requiredMarginCash) const
{
	const auto* assetInfo = LookupFutureAssetInfo(currency);
	if (assetInfo) {
		return assetInfo->availableBalance >= requiredMarginCash;
	}
	return false; // Asset not found
}

void UserAccountFuture::UpdateBalanceCash(const std::string& currency, const double pnl, const BalanceChangeEvent event)
{
	auto* assetInfo = const_cast<AssetInfo*>(LookupFutureAssetInfo(currency));
	if (assetInfo) {
		switch (event) {
		case BalanceChangeEvent::PROFIT:
			assetInfo->availableBalance += pnl;
			break;
		case BalanceChangeEvent::LOSS:
			assetInfo->availableBalance -= pnl;
			break;
		case BalanceChangeEvent::DEPOSIT:
			assetInfo->availableBalance += pnl; // pnl is the deposit amount
			break;
		case BalanceChangeEvent::WITHDRAWAL:
			assetInfo->availableBalance -= pnl; // pnl is the withdrawal amount
			break;
		}
	}
}
