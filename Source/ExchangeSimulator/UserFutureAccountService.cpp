/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include "UserAccountManager.h"
#include "UserFutureAccountService.h"
#include "ExchangeRuleAndCompliance.h"

#include "../LibraryUtils/Logger.h"
#include "../LibraryUtils/TimeUtils.h"

using namespace ExchangeSimulator;

UserFutureAccountService::UserFutureAccountService(UserAccountManager* userAccountManager)
    : m_logger{ std::make_unique<LibraryUtils::Logger>("UserFutureAccountService") }
    , m_userAccountManager(userAccountManager) {
}

UserFutureAccountService::~UserFutureAccountService() {}

grpc::Status UserFutureAccountService::GetUserFutureAccount(
    grpc::ServerContext* context,
    const futureaccount::GetUserFutureAccountRequest* request,
    futureaccount::GetUserFutureAccountResponse* response)
{
	m_logger->Info("Received GetUserFutureAccount request for User ID=" + request->useraccountid());
	const auto* userFutureAccount = m_userAccountManager->LookupFutureUserAccount(request->useraccountid());

	// populate response
	futureaccount::UserFutureAccount* account = response->mutable_account();

	account->set_useraccountid(userFutureAccount->GetUserAccountId());
    account->set_feetier(userFutureAccount->GetFeeTier());
    account->set_cantrade(userFutureAccount->CanTrade());
    account->set_candeposit(userFutureAccount->CanDeposit());
    account->set_canwithdraw(userFutureAccount->CanWithdraw());
    account->set_updatetime(static_cast<int64_t>(TimeUtils::GetEpochTimeTickNow()));

    account->set_totalinitialmargin(userFutureAccount->GetTotalInitialMargin());
    account->set_totalmaintmargin(userFutureAccount->GetTotalMaintMargin());
    account->set_totalwalletbalance(userFutureAccount->GetTotalWalletBalance());
    account->set_totalunrealizedprofit(userFutureAccount->GetTotalUnrealizedProfit());
    account->set_totalmarginbalance(userFutureAccount->GetTotalMarginBalance());
    account->set_totalpositioninitialmargin(userFutureAccount->GetTotalPositionInitialMargin());
    account->set_totalopenorderinitialmargin(userFutureAccount->GetTotalOpenOrderInitialMargin());
    account->set_totalcrosswalletbalance(userFutureAccount->GetTotalCrossWalletBalance());
    account->set_totalcrossunpnl(userFutureAccount->GetTotalCrossUnPnl());
    account->set_availablebalance(userFutureAccount->GetAvailableBalance());
    account->set_maxwithdrawamount(userFutureAccount->GetMaxWithdrawAmount());

    // Add assets
    for (const auto& a : userFutureAccount->GetAssets()) {
        auto* asset = account->add_assets();
        asset->set_asset(a.asset);
        asset->set_walletbalance(a.walletBalance);
        asset->set_unrealizedprofit(a.unrealizedProfit);
        asset->set_marginbalance(a.marginBalance);
        asset->set_maintmargin(a.maintMargin);
        asset->set_initialmargin(a.initialMargin);
        asset->set_positioninitialmargin(a.positionInitialMargin);
        asset->set_openorderinitialmargin(a.openOrderInitialMargin);
        asset->set_crosswalletbalance(a.crossWalletBalance);
        asset->set_crossunpnl(a.crossUnPnl);
        asset->set_availablebalance(a.availableBalance);
        asset->set_maxwithdrawamount(a.maxWithdrawAmount);
        asset->set_marginavailable(a.marginAvailable);
        asset->set_updatetime(a.updateTime);
    }

    // Add positions
    for (const auto& p : userFutureAccount->GetPositions()) {
        auto* pos = account->add_positions();
        pos->set_symbol(p.symbol);
        pos->set_initialmargin(p.initialMargin);
        pos->set_maintmargin(p.maintMargin);
        pos->set_unrealizedprofit(p.unrealizedProfit);
        pos->set_positioninitialmargin(p.positionInitialMargin);
        pos->set_openorderinitialmargin(p.openOrderInitialMargin);
        pos->set_leverage(p.leverage);
        pos->set_isolated(p.isolated);
        pos->set_entryprice(p.entryPrice);
        pos->set_maxnotional(p.maxNotional);
        pos->set_positionside(p.positionSide);
        pos->set_positionamt(p.positionAmt);
        pos->set_notional(p.notional);
        pos->set_isolatedwallet(p.isolatedWallet);
        pos->set_updatetime(p.updateTime);
    }

	return grpc::Status::OK;
}