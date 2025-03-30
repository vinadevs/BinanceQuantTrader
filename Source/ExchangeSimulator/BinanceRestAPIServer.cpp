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

#include "UserAccountHttpService.h"
#include "ExchangeInfoHttpService.h"
#include "BinanceRestAPIServer.h"

using namespace ExchangeSimulator;

BinanceRestAPIServer::BinanceRestAPIServer(
    const tinyxml2::XMLElement* binanceRestAPIServerXmlCfg,
    UserAccountManager* userAccountManager,
    ExchangeInfoManager* exchangeInfoManager)
	: m_logger{ std::make_unique<LibraryUtils::Logger>("BinanceRestAPIServer") }
    , m_userAccountHttpService{ std::make_unique<UserAccountHttpService>(userAccountManager) }
	, m_exchangeInfoHttpService{ std::make_unique<ExchangeInfoHttpService>(exchangeInfoManager) }
{
    assert(binanceRestAPIServerXmlCfg);
    const auto* connectionXml = binanceRestAPIServerXmlCfg->FirstChildElement("Connection");
    assert(connectionXml);
    m_logger->Info("Initiating BinanceRestAPIServer.");
    m_serverIpAddress = std::string(connectionXml->Attribute("ServerIpAddress"));
    m_serverPort = std::string(connectionXml->Attribute("ServerPort"));
    m_serverConnection = m_serverIpAddress + ":" + m_serverPort;
}

BinanceRestAPIServer::~BinanceRestAPIServer() {}

void BinanceRestAPIServer::WaitForIncomingMessage()
{
    grpc::ServerBuilder builder;
    builder.AddListeningPort(m_serverConnection, grpc::InsecureServerCredentials());
    builder.RegisterService(m_userAccountHttpService.get());
    builder.RegisterService(m_exchangeInfoHttpService.get());

    m_grpcServer = builder.BuildAndStart();
    m_logger->Info("BinanceRestAPIServer listening on " + m_serverConnection);

    m_grpcServer->Wait(); // wait call
}

void BinanceRestAPIServer::Start()
{
    m_grpcThread = std::thread(&BinanceRestAPIServer::WaitForIncomingMessage, this);
    m_grpcThread.detach();
}

void BinanceRestAPIServer::Stop()
{
    m_grpcServer->Shutdown(); // unblock release Wait()
}
