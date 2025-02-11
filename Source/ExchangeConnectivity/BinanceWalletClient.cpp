/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

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
    m_serverIpAddress = std::string(connectionXml->Attribute("ServerIpAddress"));
    m_serverPort = std::string(connectionXml->Attribute("ServerPort"));
    m_serverConnection = m_serverIpAddress + ":" + m_serverPort;
    m_grpcChannel = grpc::CreateChannel(m_serverConnection, grpc::InsecureChannelCredentials());
    m_grpcStub = UserAccountService::NewStub(m_grpcChannel);
}

BinanceWalletClient::~BinanceWalletClient()
{
}

void BinanceWalletClient::GetUserAccountDataResponse(const std::string& user_id)
{
    UserAccountDataRequest request;
    request.set_user_id(user_id);

    UserAccountDataResponse response;
    grpc::ClientContext context;

    const grpc::Status status = m_grpcStub->GetUserAccountData(&context, request, &response);

    if (status.ok()) {
        std::cout << "Response received from server:\n"
            << "User ID: " << response.user_id() << "\n"
            << "Maker Commission: " << response.maker_commission() << "\n"
            << "Taker Commission: " << response.taker_commission() << "\n"
            << "Buyer Commission: " << response.buyer_commission() << "\n"
            << "Seller Commission: " << response.seller_commission() << "\n"
            << "Update Time: " << response.update_time() << "\n"
            << "Can Trade: " << (response.can_trade() ? "true" : "false") << "\n"
            << "Can Withdraw: " << (response.can_withdraw() ? "true" : "false") << "\n"
            << "Can Deposit: " << (response.can_deposit() ? "true" : "false") << "\n";

        for (const auto& balance_pair : response.balances()) {
            const auto& balance = balance_pair.second;
            std::cout << "Asset: " << balance.asset_symbol() << "\n"
                << "  Free Amount: " << balance.free_amount() << "\n"
                << "  Locked Amount: " << balance.locked_amount() << "\n";
        }
    }
    else {
        std::cout << "RPC failed: " << status.error_message() << std::endl;
    }
}
