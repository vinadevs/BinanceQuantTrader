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
#include "BinanceWalletServer.h"

using namespace ExchangeSimulator;

BinanceWalletServer::BinanceWalletServer(
    const tinyxml2::XMLElement* binanceWalletServerXmlCfg,
    UserAccountManager* userAccountManager)
	: m_logger{ std::make_unique<LibraryUtils::Logger>("BinanceWalletServer") }
    , m_userAccountHttpService{ std::make_unique<UserAccountHttpService>(userAccountManager) }
{
    assert(binanceWalletServerXmlCfg);
    const auto* connectionXml = binanceWalletServerXmlCfg->FirstChildElement("Connection");
    assert(connectionXml);
    m_logger->Info("Initiating BinanceWalletServer.");
    m_serverIpAddress = std::string(connectionXml->Attribute("ServerIpAddress"));
    m_serverPort = std::string(connectionXml->Attribute("ServerPort"));
    m_serverConnection = m_serverIpAddress + ":" + m_serverPort;
}

BinanceWalletServer::~BinanceWalletServer()
{
}

void BinanceWalletServer::WaitForIncomingMessage()
{
    grpc::ServerBuilder builder;
    builder.AddListeningPort(m_serverConnection, grpc::InsecureServerCredentials());
    builder.RegisterService(m_userAccountHttpService.get());

    m_grpcServer = builder.BuildAndStart();
    m_logger->Info("BinanceWalletServer listening on " + m_serverConnection);

    m_grpcServer->Wait(); // wait call
}

void BinanceWalletServer::Start()
{
    m_grpcThread = std::thread(&BinanceWalletServer::WaitForIncomingMessage, this);
    m_grpcThread.detach();
}

void BinanceWalletServer::Stop()
{
    m_grpcServer->Shutdown(); // unblock release Wait()
}
