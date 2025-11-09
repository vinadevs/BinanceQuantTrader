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

#include "BinanceTradeProfile.h"

using namespace ExchangeConnectivity;

BinanceTradeProfile::BinanceTradeProfile(const tinyxml2::XMLElement* binanceTradeProfileXmlCfg)
    : m_logger{ std::make_unique<LibraryUtils::Logger>("BinanceTradeProfile") }
{
    assert(binanceTradeProfileXmlCfg);
    const auto* connectionXml = binanceTradeProfileXmlCfg->FirstChildElement("Connection");
    assert(connectionXml);
    m_logger->Info("Creating new Http connection...");
    m_grpcConnection.m_serverIpAddress = std::string(connectionXml->Attribute("ServerIpAddress"));
    m_grpcConnection.m_serverPort = std::string(connectionXml->Attribute("ServerPort"));
    m_grpcConnection.m_serverConnection = m_grpcConnection.m_serverIpAddress + ":" + m_grpcConnection.m_serverPort;
    m_grpcConnection.m_grpcChannel = grpc::CreateChannel(m_grpcConnection.m_serverConnection, grpc::InsecureChannelCredentials());
    m_grpcConnection.m_grpcStub = usertradeprofile::TradeService::NewStub(m_grpcConnection.m_grpcChannel);
}

BinanceTradeProfile::~BinanceTradeProfile() {}

bool BinanceTradeProfile::UpdateUserTradeProfileData(
    const std::string& userId,
    const double leverageRate,
    std::string& resultMessage)
{
    m_logger->Info("Sending request UserTradeProfile data for user account id=" + userId);

    usertradeprofile::UpdateUserTradeProfileRequest request;
    request.set_user_account_id(userId);
	request.set_leverage(leverageRate);

    usertradeprofile::UpdateUserTradeProfileResponse response;
    grpc::ClientContext context;

    const grpc::Status status = m_grpcConnection.m_grpcStub->UpdateUserTradeProfile(&context, request, &response);
    resultMessage = response.message();
    return (status.ok() && response.success());
}
