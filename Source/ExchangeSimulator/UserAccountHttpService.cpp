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

#include "../LibraryUtils/Logger.h"

using namespace ExchangeSimulator;

UserAccountHttpService::UserAccountHttpService(UserAccountManager* userAccountManager)
    : m_logger{ std::make_unique<LibraryUtils::Logger>("UserAccountHttpService") }
    , m_userAccountManager(userAccountManager) {}

UserAccountHttpService::~UserAccountHttpService() {}

grpc::Status UserAccountHttpService::GetUserAccountData(grpc::ServerContext* context, const UserAccountDataRequest* request, UserAccountDataResponse* response)
{
    m_logger->Info("Received UserAccountData request for User ID: " + request->user_id());
    const auto userAccount = m_userAccountManager->LookupUserAccount(request->user_id());

    // Populate response
    m_logger->Info("Preparing UserAccountData response for User ID: " + request->user_id());
    response->set_user_id(request->user_id());
    response->set_maker_commission(userAccount.m_makerCommission);
    response->set_taker_commission(userAccount.m_takerCommission);
    response->set_buyer_commission(userAccount.m_buyerCommission);
    response->set_seller_commission(userAccount.m_sellerCommission);
    response->set_update_time(userAccount.m_updateTime);
    response->set_can_trade(userAccount.IsAccountEligibleToTrade());
    response->set_can_withdraw(userAccount.m_canWithdraw);
    response->set_can_deposit(userAccount.m_canDeposit);

    for (const auto& asset : userAccount.m_assetBalances)
    {
        Balance balance;
        balance.set_asset_symbol(asset.second.m_symbol);
        balance.set_free_amount(asset.second.m_free);
        balance.set_locked_amount(asset.second.m_locked);
        response->mutable_balances()->emplace(asset.second.m_symbol, balance);
    }

    return grpc::Status::OK;
}
