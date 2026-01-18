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
    const auto* walletFromMarketXml = binanceWalletClientXmlCfg->FirstChildElement("WalletFromMarket");
    assert(walletFromMarketXml);
    const auto* connectionXml = binanceWalletClientXmlCfg->FirstChildElement("Connection");
    assert(connectionXml);
    if (std::string(walletFromMarketXml->Attribute("Market")) == "Spot")
    {
		m_logger->Info("Creating spot account gRPC connection to server.");
        m_grpcConnectionSpotAccount.m_serverIpAddress = std::string(connectionXml->Attribute("ServerIpAddress"));
        m_grpcConnectionSpotAccount.m_serverPort = std::string(connectionXml->Attribute("ServerPort"));
        m_grpcConnectionSpotAccount.m_serverConnection = m_grpcConnectionSpotAccount.m_serverIpAddress + ":" + m_grpcConnectionSpotAccount.m_serverPort;
        m_grpcConnectionSpotAccount.m_grpcChannel = grpc::CreateChannel(m_grpcConnectionSpotAccount.m_serverConnection, grpc::InsecureChannelCredentials());
        m_grpcConnectionSpotAccount.m_grpcStubSpotAccount = account::UserAccountService::NewStub(m_grpcConnectionSpotAccount.m_grpcChannel);
    }
    else if (std::string(walletFromMarketXml->Attribute("Market")) == "Future")
    {
		m_logger->Info("Creating future account gRPC connection to server.");
        m_grpcConnectionFutureAccount.m_serverIpAddress = std::string(connectionXml->Attribute("ServerIpAddress"));
        m_grpcConnectionFutureAccount.m_serverPort = std::string(connectionXml->Attribute("ServerPort"));
        m_grpcConnectionFutureAccount.m_serverConnection = m_grpcConnectionFutureAccount.m_serverIpAddress + ":" + m_grpcConnectionFutureAccount.m_serverPort;
        m_grpcConnectionFutureAccount.m_grpcChannel = grpc::CreateChannel(m_grpcConnectionFutureAccount.m_serverConnection, grpc::InsecureChannelCredentials());
        m_grpcConnectionFutureAccount.m_grpcStubFutureAccount = futureaccount::UserAccountService::NewStub(m_grpcConnectionFutureAccount.m_grpcChannel);
    }
	else
	{
		throw std::runtime_error("BinanceWalletClient: Unsupported Market type in WalletFromMarket configuration.");
	}
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

    const grpc::Status status = m_grpcConnectionSpotAccount.m_grpcStubSpotAccount->GetUserAccountData(&context, request, &response);

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
        // Set fields directly using setters
		account->SetUserAccountId(pb.useraccountid());
        account->SetFeeTier(pb.feetier());
        account->SetCanTrade(pb.cantrade());
        account->SetCanDeposit(pb.candeposit());
        account->SetCanWithdraw(pb.canwithdraw());
        account->SetUpdateTime(pb.updatetime());

        account->SetTotalInitialMargin(pb.totalinitialmargin());
        account->SetTotalMaintMargin(pb.totalmaintmargin());
        account->SetTotalWalletBalance(pb.totalwalletbalance());
        account->SetTotalUnrealizedProfit(pb.totalunrealizedprofit());
        account->SetTotalMarginBalance(pb.totalmarginbalance());
        account->SetTotalPositionInitialMargin(pb.totalpositioninitialmargin());
        account->SetTotalOpenOrderInitialMargin(pb.totalopenorderinitialmargin());
        account->SetTotalCrossWalletBalance(pb.totalcrosswalletbalance());
        account->SetTotalCrossUnPnl(pb.totalcrossunpnl());
        account->SetAvailableBalance(pb.availablebalance());
        account->SetMaxWithdrawAmount(pb.maxwithdrawamount());

        // Assets
        std::vector<KernelTrading::AssetInfo> assets;
		assets.reserve(pb.assets_size());
        for (const auto& a : pb.assets()) {
            assets.emplace_back(
                a.asset(),
                a.walletbalance(),
                a.unrealizedprofit(),
                a.marginbalance(),
                a.maintmargin(),
                a.initialmargin(),
                a.positioninitialmargin(),
                a.openorderinitialmargin(),
                a.crosswalletbalance(),
                a.crossunpnl(),
                a.availablebalance(),
                a.maxwithdrawamount(),
                a.marginavailable(),
                a.updatetime()
            );
        }
        account->SetAssets(assets);

        // Positions
        std::vector<KernelTrading::PositionInfo> positions;
		positions.reserve(pb.positions_size());
        for (const auto& p : pb.positions()) {
            positions.emplace_back(
                p.symbol(),
                p.initialmargin(),
                p.maintmargin(),
                p.unrealizedprofit(),
                p.positioninitialmargin(),
                p.openorderinitialmargin(),
                p.leverage(),
                p.isolated(),
                p.entryprice(),
                p.maxnotional(),
                p.positionside(),
                p.positionamt(),
                p.notional(),
                p.isolatedwallet(),
                p.updatetime()
            );
        }
        account->SetPositions(positions);
        return true;
    }
    catch (const std::exception& ex) {
        errorMessage = ex.what();
        return false;
    }
}
