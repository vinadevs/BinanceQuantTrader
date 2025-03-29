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

#include "BinanceExchangeClient.h"

using namespace ExchangeConnectivity;

BinanceExchangeClient::BinanceExchangeClient(const tinyxml2::XMLElement* BinanceExchangeClientXmlCfg)
    : m_logger{ std::make_unique<LibraryUtils::Logger>("BinanceExchangeClient") }
{
    assert(BinanceExchangeClientXmlCfg);
    const auto* connectionXml = BinanceExchangeClientXmlCfg->FirstChildElement("Connection");
    assert(connectionXml);
    m_logger->Info("Creating new Http connection...");
    m_grpcConnection.m_serverIpAddress = std::string(connectionXml->Attribute("ServerIpAddress"));
    m_grpcConnection.m_serverPort = std::string(connectionXml->Attribute("ServerPort"));
    m_grpcConnection.m_serverConnection = m_grpcConnection.m_serverIpAddress + ":" + m_grpcConnection.m_serverPort;
    m_grpcConnection.m_grpcChannel = grpc::CreateChannel(m_grpcConnection.m_serverConnection, grpc::InsecureChannelCredentials());
    m_grpcConnection.m_grpcStub = exchange::ExchangeService::NewStub(m_grpcConnection.m_grpcChannel);
}

BinanceExchangeClient::~BinanceExchangeClient() {}

bool BinanceExchangeClient::GetExchangeInfoResponse(
    const std::string& symbolId,
    binapi::rest::exchange_info_t* exchangeInfo,
    std::string& errorMessage)
{
    m_logger->Info("Sending request exchange_info_t data for symbol id=" + symbolId);

    exchange::ExchangeInfoRequest request;
    request.set_symbol_id(symbolId);

    exchange::ExchangeInfoResponse response;
    grpc::ClientContext context;
    return false;
}
