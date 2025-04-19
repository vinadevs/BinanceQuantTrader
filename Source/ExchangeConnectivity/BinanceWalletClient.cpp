/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../KernelTrading/types.h"
#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/Logger.h"

#include "BinanceWalletClient.h"

using namespace ExchangeConnectivity;

BinanceWalletClient::BinanceWalletClient(const tinyxml2::XMLElement* binanceWalletClientXmlCfg)
    : m_logger{ std::make_unique<LibraryUtils::Logger>("BinanceWalletClient") }
{
    assert(binanceWalletClientXmlCfg);
    const auto* connectionXml = binanceWalletClientXmlCfg->FirstChildElement("Connection");
    assert(connectionXml);
    m_logger->Info("Creating new Http connection...");
    m_grpcConnection.m_serverIpAddress = std::string(connectionXml->Attribute("ServerIpAddress"));
    m_grpcConnection.m_serverPort = std::string(connectionXml->Attribute("ServerPort"));
    m_grpcConnection.m_serverConnection = m_grpcConnection.m_serverIpAddress + ":" + m_grpcConnection.m_serverPort;
    m_grpcConnection.m_grpcChannel = grpc::CreateChannel(m_grpcConnection.m_serverConnection, grpc::InsecureChannelCredentials());
    m_grpcConnection.m_grpcStub = account::UserAccountService::NewStub(m_grpcConnection.m_grpcChannel);
}

BinanceWalletClient::~BinanceWalletClient() {}

bool BinanceWalletClient::GetUserAccountDataResponse(
    const std::string& userId,
    binapi::rest::account_info_t* account,
    std::string& errorMessage)
{
    m_logger->Info("Sending request account_info_t data for user account id=" + userId);

    account::UserAccountDataRequest request;
    request.set_user_id(userId);

    account::UserAccountDataResponse response;
    grpc::ClientContext context;

    const grpc::Status status = m_grpcConnection.m_grpcStub->GetUserAccountData(&context, request, &response);

    if (status.ok()) 
    {
        if (response.user_id() == userId)
        {
            account->makerCommission = response.maker_commission();
            account->takerCommission = response.taker_commission();
            account->buyerCommission = response.buyer_commission();
            account->sellerCommission = response.seller_commission();
            account->canTrade = response.can_trade();
            account->canWithdraw = response.can_withdraw();
            account->canDeposit = response.can_deposit();
            account->updateTime = response.update_time();
#ifdef USE_BACK_TEST_TRADING
            account->stableCoinAmount = response.stable_coin_amount();
#endif
            for (const auto& balancePair : response.balances()) {
                const auto& balance = balancePair.second;
                binapi::rest::account_info_t::balance_t value {
                    balance.asset_symbol(),
                    balance.free_amount(),
                    balance.locked_amount() };
                account->balances.insert_or_assign(balance.asset_symbol(), value);
            }
            return true;
        }
        errorMessage = "Account user ID does not match.";
        return false;
    }
    else 
    {
        errorMessage = status.error_message();
        return false;
    }
}
