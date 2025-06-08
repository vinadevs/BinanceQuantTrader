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
#include "../StaticData/StaticDataManager.h"

#include "MatchingEngine.h"
#include "BqtJsonMessageServer.h"
#include "UserAccountManager.h"
#include "ExchangeInfoManager.h"
#include "UserTradeProfileManager.h"
#include "BinanceRestAPIServer.h"
#include "BinanceExchangeSimulator.h"
#include "AlgosConnectivity.h"
#include "ExchangeRuleAndCompliance.h"

using namespace ExchangeSimulator;

BinanceExchangeSimulator::BinanceExchangeSimulator(const tinyxml2::XMLDocument* configSimulatorXml)
    : m_logger{ std::make_unique<LibraryUtils::Logger>("BinanceExchangeSimulator") }
{
	PrepareExchangeServices(configSimulatorXml);
}

BinanceExchangeSimulator::~BinanceExchangeSimulator()
{
	Shutdown();
}

void BinanceExchangeSimulator::Run()
{
	// -This RestAPI server is a Google Protobuf HTTP message server
	// We will use it to receive http querry from the Algo Trading System
	m_logger->Info("Starting BinanceRestAPIServer.");
	m_binanceRestAPIServer->Start(); // no wait call
	m_logger->Info("Started BinanceRestAPIServer.");
	// -This message receiver is a BQT Json Message server
	// We will use it to receive trading orders from the Algo Trading System
	m_logger->Info("Starting Bqt Message Server.");
	m_bqtJsonMessageServer->Start(); // no wait call
	m_logger->Info("Started Bqt Message Server.");
	// -A realtime order-matching system
	m_logger->Info("Starting Order Matching Engine. Waiting for simulating orders...");
	m_matchingEngine->Start(); // wait call, so it has to be the final call!
}

void BinanceExchangeSimulator::Shutdown()
{
	m_bqtJsonMessageServer->Stop();
	m_matchingEngine->Stop();
	m_binanceRestAPIServer->Stop();
}

void BinanceExchangeSimulator::PrepareExchangeServices(const tinyxml2::XMLDocument* configSimulatorXml)
{
	// PRELOAD CONFIG XML FILE
	assert(configSimulatorXml);
	const auto* exchangeRuleAndComplianceCfg = configSimulatorXml->FirstChildElement("ExchangeRuleAndCompliance");
	assert(exchangeRuleAndComplianceCfg);
	m_logger->Info("Initiating ExchangeRuleAndCompliance.");
	ExchangeRuleMgr->SetRuleAndCompliance(exchangeRuleAndComplianceCfg);

	m_logger->Info("Initiating Static Data.");
	const auto* staticDataCfg = configSimulatorXml->FirstChildElement("StaticData");
	StaticDataMgr->LoadStaticDatabase(staticDataCfg);

	m_logger->Info("Initiating UserAccountManager.");
	const auto* userAccountManagerCfg = configSimulatorXml->FirstChildElement("UserAccountManager");
	assert(userAccountManagerCfg);
	m_userAccountManager = std::make_unique<UserAccountManager>(userAccountManagerCfg);

	m_logger->Info("Initiating ExchangeInfoManager.");
	const auto* exchangeInfoManagerCfg = configSimulatorXml->FirstChildElement("ExchangeInfoManager");
	assert(exchangeInfoManagerCfg);
	m_exchangeInfoManager = std::make_unique<ExchangeInfoManager>(exchangeInfoManagerCfg);

	m_logger->Info("Initiating UserTradeProfileManager.");
	const auto* userTradeProfileManagerCfg = configSimulatorXml->FirstChildElement("UserTradeProfileManager");
	assert(userTradeProfileManagerCfg);
	m_userTradeProfileManager = std::make_unique<UserTradeProfileManager>(userTradeProfileManagerCfg);

	m_logger->Info("Initiating BinanceRestAPIServer.");
	const auto* binanceRestAPIServerCfg = configSimulatorXml->FirstChildElement("BinanceRestAPIServer");
	assert(binanceRestAPIServerCfg);
	m_binanceRestAPIServer = std::make_unique<BinanceRestAPIServer>(
		binanceRestAPIServerCfg,
		m_userAccountManager.get(),
		m_exchangeInfoManager.get(),
		m_userTradeProfileManager.get());

	m_logger->Info("Initiating Exchange Matching Engine.");
	const auto* matchingEngineCfg = configSimulatorXml->FirstChildElement("MatchingEngine");
	assert(matchingEngineCfg);
	m_matchingEngine = std::make_unique<MatchingEngine>(matchingEngineCfg, m_userAccountManager.get());

	m_logger->Info("Initiating Message Transporter.");
	const auto* messageTransporterCfg = configSimulatorXml->FirstChildElement("MessageTransporter");
	assert(messageTransporterCfg);
	UpstreamGateWay->InitMessageTransporter(messageTransporterCfg);

	m_logger->Info("Initiating Bqt Message Server.");
	const auto* messageServerCfg = configSimulatorXml->FirstChildElement("MessageServer");
	assert(messageServerCfg);
	m_bqtJsonMessageServer = std::make_unique<BqtJsonMessageServer>(messageServerCfg,
		dynamic_cast<MiddlewareMQ::MessageHandler*>(m_matchingEngine.get()));
}
