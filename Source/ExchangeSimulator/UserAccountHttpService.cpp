/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "UserAccountManager.h"
#include "UserAccountHttpService.h"
#include "ExchangeRuleAndCompliance.h"

#include "../LibraryUtils/Logger.h"

using namespace ExchangeSimulator;

UserAccountHttpService::UserAccountHttpService(UserAccountManager* userAccountManager)
    : m_logger{ std::make_unique<LibraryUtils::Logger>("UserAccountHttpService") }
    , m_userAccountManager(userAccountManager) {}

UserAccountHttpService::~UserAccountHttpService() {}

grpc::Status UserAccountHttpService::GetUserAccountData(
    grpc::ServerContext* context,
    const account::UserAccountDataRequest* request,
    account::UserAccountDataResponse* response)
{
    m_logger->Info("Received account_info_t request for User ID=" + request->user_id());
   
    const auto* userAccount = m_userAccountManager->LookupUserAccount(request->user_id());

    // Populate response
    response->set_user_id(request->user_id());
    response->set_update_time(userAccount->GetUpdateTime());
    response->set_can_trade(userAccount->IsAccountEligibleToTrade());
    response->set_can_withdraw(userAccount->IsAccountEligibleToTrade());
    response->set_can_deposit(userAccount->IsAccountEligibleToDeposit());
    response->set_stable_coin_amount(userAccount->m_usdtBalance.m_usdtAmount);
    response->set_maker_commission(ExchangeRuleMgr->GetMakerCommission());
    response->set_taker_commission(ExchangeRuleMgr->GetTakerCommission());
    response->set_buyer_commission(ExchangeRuleMgr->GetBuyerCommission());
    response->set_seller_commission(ExchangeRuleMgr->GetSellerCommission());

    for (const auto& asset : userAccount->m_assetBalances)
    {
        account::Balance balance;
        balance.set_asset_symbol(asset.second.m_symbol);
        balance.set_free_amount(asset.second.m_free);
        balance.set_locked_amount(asset.second.m_locked);
        response->mutable_balances()->emplace(asset.second.m_symbol, balance);
    }

    m_logger->Info("Sending UserAccountData response for User ID=" + request->user_id());

    return grpc::Status::OK;
}
