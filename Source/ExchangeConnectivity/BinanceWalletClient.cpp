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
#include "../KernelTrading/user_future_account.h"

#include "BinanceWalletClient.h"

#include <nlohmann/json.hpp>

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

	m_grpcConnectionFutureAccount.m_serverIpAddress = m_grpcConnection.m_serverIpAddress;
	m_grpcConnectionFutureAccount.m_serverPort = m_grpcConnection.m_serverPort;
	m_grpcConnectionFutureAccount.m_serverConnection = m_grpcConnection.m_serverConnection;
	m_grpcConnectionFutureAccount.m_grpcChannel = grpc::CreateChannel(m_grpcConnectionFutureAccount.m_serverConnection, grpc::InsecureChannelCredentials());
	m_grpcConnectionFutureAccount.m_grpcStubFutureAccount = futureaccount::UserAccountService::NewStub(m_grpcConnectionFutureAccount.m_grpcChannel);
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
#if USE_BACK_TEST_TRADING
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

bool BinanceWalletClient::GetUserFutureAccountDataResponse(const std::string& userId,
    KernelTrading::UserFutureAccount* account,
    std::string& errorMessage)
{
	m_logger->Info("Sending request UserFutureAccount data for user account id=" + userId);
    // 1. Setup stub and request
    grpc::ClientContext context;
    futureaccount::GetUserFutureAccountRequest request;
    futureaccount::GetUserFutureAccountResponse response;

    request.set_useraccountid(userId);

    // 2. Make RPC call
    grpc::Status status = m_grpcConnectionFutureAccount.m_grpcStubFutureAccount->GetUserFutureAccount(&context, request, &response);

    if (!status.ok()) {
        errorMessage = status.error_message();
        return false;
    }

    // 3. Extract protobuf response
    const futureaccount::UserFutureAccount& pb = response.account();

    // 4. Convert protobuf UserFutureAccount -> your ExchangeSimulator::UserFutureAccount
    try {
        // Construct nlohmann::json manually or convert from protobuf
        nlohmann::json j;

        j["feeTier"] = pb.feetier();
        j["canTrade"] = pb.cantrade();
        j["canDeposit"] = pb.candeposit();
        j["canWithdraw"] = pb.canwithdraw();
        j["updateTime"] = pb.updatetime();

        j["totalInitialMargin"] = pb.totalinitialmargin();
        j["totalMaintMargin"] = pb.totalmaintmargin();
        j["totalWalletBalance"] = pb.totalwalletbalance();
        j["totalUnrealizedProfit"] = pb.totalunrealizedprofit();
        j["totalMarginBalance"] = pb.totalmarginbalance();
        j["totalPositionInitialMargin"] = pb.totalpositioninitialmargin();
        j["totalOpenOrderInitialMargin"] = pb.totalopenorderinitialmargin();
        j["totalCrossWalletBalance"] = pb.totalcrosswalletbalance();
        j["totalCrossUnPnl"] = pb.totalcrossunpnl();
        j["availableBalance"] = pb.availablebalance();
        j["maxWithdrawAmount"] = pb.maxwithdrawamount();

        // Assets
        for (const auto& a : pb.assets()) {
            j["assets"].push_back({
                {"asset", a.asset()},
                {"walletBalance", a.walletbalance()},
                {"unrealizedProfit", a.unrealizedprofit()},
                {"marginBalance", a.marginbalance()},
                {"maintMargin", a.maintmargin()},
                {"initialMargin", a.initialmargin()},
                {"positionInitialMargin", a.positioninitialmargin()},
                {"openOrderInitialMargin", a.openorderinitialmargin()},
                {"crossWalletBalance", a.crosswalletbalance()},
                {"crossUnPnl", a.crossunpnl()},
                {"availableBalance", a.availablebalance()},
                {"maxWithdrawAmount", a.maxwithdrawamount()},
                {"marginAvailable", a.marginavailable()},
                {"updateTime", a.updatetime()}
                });
        }

        // Positions
        for (const auto& p : pb.positions()) {
            j["positions"].push_back({
                {"symbol", p.symbol()},
                {"initialMargin", p.initialmargin()},
                {"maintMargin", p.maintmargin()},
                {"unrealizedProfit", p.unrealizedprofit()},
                {"positionInitialMargin", p.positioninitialmargin()},
                {"openOrderInitialMargin", p.openorderinitialmargin()},
                {"leverage", p.leverage()},
                {"isolated", p.isolated()},
                {"entryPrice", p.entryprice()},
                {"maxNotional", p.maxnotional()},
                {"positionSide", p.positionside()},
                {"positionAmt", p.positionamt()},
                {"notional", p.notional()},
                {"isolatedWallet", p.isolatedwallet()},
                {"updateTime", p.updatetime()}
                });
        }

        // 5. Call FromJson on your UserFutureAccount class
        account->FromJson(j);  // assuming this is your own function
        return true;
    }
    catch (const std::exception& ex) {
        errorMessage = ex.what();
        return false;
    }
}
