/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../MarketData/RealTimeMarketData.h"
#include "../StaticData/StaticDataManager.h"
#include "../HistoricalData/HistoricalDataManager.h"
#include "../PortfolioManager/PortfolioInvestmentBinance.h"
#include "../UserAccount/BinanceTrader.h"
#include "../ComplianceNRegulatory/BinanceTradingRules.h"
#include "../RiskManagement/RiskManager.h"
#include "../TradingStrategies/TradingStrategyBase.h"
#include "../TradingStrategies/SingleStrategyHost.h"
#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/Logger.h"
#include "../LibraryUtils/SourceBuildFlags.h"
#include "../RestAPI/ApiKeyInfoManager.h"
#include "../RestAPI/BinanceAPI.h"

#if USE_TEST_TRADING
#include "../TradingStrategies/StrategyMessageServer.h"
#include "../ExchangeConnectivity/ExchangeSimulatorConnectivity.h"
#endif

#include "StrategyFactory.h"
#include "TraderFactory.h"
#include "TradingModel.h"

#include <exception>
#include <string>

using namespace BinanceQuantTrader;
using namespace PortfolioManager;
using namespace TradingStrategies;
using namespace ComplianceNRegulatory;
using namespace RiskManagement;
using namespace MarketData;
using namespace StaticData;
using namespace HistoricalData;
using namespace UserAccount;
using namespace tinyxml2;
using namespace LibraryUtils;
using namespace RestAPI;

TradingModel::TradingModel(
	const XMLDocument* configBQTXml,
	const XMLDocument* configAccessKeyXml)
	: m_strategyHost{ std::make_unique<SingleStrategyHost>() },
	  m_logger{ std::make_unique<Logger>("TradingModel") }
{
	PrepareTradingComponents(configBQTXml, configAccessKeyXml);
	// To this point, we have prepared all trading components but
	// the model is not started yet, we will need to call RunModel()
}

TradingModel::~TradingModel()
{
	// IF YOU ARE SEEING THIS BUILD ERROR:
	// Error	C2027	use of undefined type 'namespace::Class'
	// Error	C2338	static_assert failed: 'can't delete an incomplete type'	
	// SOLUTION: EXPLICITLY DEFINE A DESTUCTOR OF THE ERROR CLASS AND PUT
	// IMPLEMENTATION CODE INTO SOURCE FILE.

	// EXPLANATION:
	// There are certain operations on the pointer which require a complete type;
	// in particular, when the object will actually be destructed
	// if TradingModel::~TradingModel() were inline, 
	// this might cause problems. (Note that if you don't declare the destructor yourself,
	// it will be inline in header file so the definitions of other classes can be incompleted)
	// Also if the destructor is inline (i.e., defined in the header file),
	// it will be included in every translation unit that includes the header.
	// This can lead to larger binary sizes because the destructor is essentially 
	// copied across all translation units that use the class, rather than having 
	// a single implementation in one translation unit.
}

void TradingModel::PrepareTradingComponents(
	const XMLDocument* configBQTXml,
	const XMLDocument* configAccessKeyXml)
{
	// PREPARES TRADING COMPONENTS
	m_logger->Info("Initiating User Account And Binance API.");
	const auto* userAccountXml = configAccessKeyXml->FirstChildElement("UserAccount");
	assert(userAccountXml);
	const auto* keyXml = userAccountXml->FirstChildElement("Key");
	assert(keyXml);
	const auto* sk = keyXml->Attribute("SK");
	const auto* pk = keyXml->Attribute("PK");
#if USE_TEST_TRADING
	const auto* userID = keyXml->Attribute("UserID");
#endif
	const auto* binanceAPICfg = configBQTXml->FirstChildElement("BinanceAPI");
	assert(binanceAPICfg);
	const auto* connectionXml = binanceAPICfg->FirstChildElement("Connection");
	assert(connectionXml);
	const auto* apiBinanceCom = connectionXml->Attribute("ApiBinanceCom");
	const auto* apiBinancePort = connectionXml->Attribute("ApiBinancePort");
	const auto* connectionTimeoutMs = connectionXml->Attribute("ConnectionTimeoutMs");
#if USE_TEST_TRADING
	ApiKeyInfoMgr->InitApiKeyInfo(userID, pk, sk);
#endif
	BinanceAPI::GetInstance()->InitiateAPI(apiBinanceCom, apiBinancePort, pk, sk, connectionTimeoutMs);

	m_logger->Info("Initiating Static Data.");
	const auto* staticDataCfg = configBQTXml->FirstChildElement("StaticData");
	StaticDataMgr->LoadStaticDatabase(staticDataCfg);

	m_logger->Info("Initiating Historical Data.");
	const auto* historicalDataCfg = configBQTXml->FirstChildElement("HistoricalData");
	HistoricalDataMgr->LoadHistoricalDatabase(historicalDataCfg);

	m_logger->Info("Initiating Real Time Market Data.");
	const auto* realTimeMarketDataCfg = configBQTXml->FirstChildElement("RealTimeMarketData");
	m_marketData = std::make_unique<RealTimeMarketData>(realTimeMarketDataCfg);

	m_logger->Info("Initiating Portfolio Investment.");
	const auto* portfolioCfg = configBQTXml->FirstChildElement("PortfolioInvestment");
	m_portfolio = std::make_unique<PortfolioInvestmentBinance>(portfolioCfg, m_marketData.get());

	m_logger->Info("Initiating Risk Management.");
	const auto* riskManagementCfg = configBQTXml->FirstChildElement("RiskManagement");
	m_riskManager = std::make_unique<RiskManager>(riskManagementCfg);

	m_logger->Info("Initiating Trader.");
	const auto* traderXmlCfg = configBQTXml->FirstChildElement("TraderInfo");
	m_trader = TraderFactory::CreateSmartTrader(m_portfolio.get(), m_riskManager.get(), traderXmlCfg);

	m_logger->Info("Initiating Compliance And Regulatory .");
	const auto* complianceCfg = configBQTXml->FirstChildElement("ComplianceNRegulatory");
	m_tradingRules = std::make_unique<BinanceTradingRules>(complianceCfg);

	m_logger->Info("Initiating Trading Strategy.");
	const auto* strategyCfg = configBQTXml->FirstChildElement("TradingStrategy");
	m_strategy = StrategyFactory::CreateTargetStrategy(strategyCfg,
		m_marketData.get(),
		m_trader.get(),
		m_tradingRules.get());

#if USE_TEST_TRADING
	m_logger->Info("Initiating Message Server.");
	const auto* messageServerCfg = configBQTXml->FirstChildElement("MessageServer");
	m_strategyMessageServer = std::make_unique<StrategyMessageServer>(messageServerCfg);
	m_strategyMessageServer->RegisterMessageHandler(m_strategy.get());

	m_logger->Info("Initiating Message Transporter.");
	const auto* messageTransporterCfg = configBQTXml->FirstChildElement("MessageTransporter");
	ExchangeSimulatorGateWay->InitMessageTransporter(messageTransporterCfg);

	m_logger->Info("Initiating Binance Wallet Client.");
	const auto* binanceWalletClientCfg = configBQTXml->FirstChildElement("BinanceWalletClient");
	ExchangeSimulatorGateWay->InitBinanceWalletClient(binanceWalletClientCfg);
#endif
}

void TradingModel::RunModel()
{
#if USE_TEST_TRADING
	// -This message receiver is a BQT Json Message server
	// We will use it to receive acks from the Exchange Simulator
	// for testing trade
	// -We dont have a Binance message server, because Binance API 
	// is using REST protocol so we will use HTTP/REST protocol instead
	m_strategyMessageServer->Start();
#endif
	m_logger->Info(USE_MULTITHREADING_MESSAGE);
#if USE_MULTITHREADING
	// If Strategies and Trading Services (Market Data,...) want to run in mutiple threads
	// to avoid stale trading signals but the trade might be slower than single thread mode
	if (m_strategy && m_strategyHost)
	{
		m_strategyHost->StartStrategyThread(m_strategy.get());
	}
#else
	//If Strategies and Trading Services (Market Data,...) want to run in single thread
	if (m_strategy)
	{
		m_strategy->StartLive();
	}
#endif
	if (m_marketData)
	{
		// Start receive real time market data and analyze to find trading opportunity signals
		m_marketData->StartStreamingData();
	}
}
