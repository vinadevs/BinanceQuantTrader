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
#include "../TradingStrategies/ExternalController.h"
#include "../SettingNConfig/tinyxml2.h"
#include "../SettingNConfig/BqtXmlUtils.h"
#include "../LibraryUtils/Logger.h"
#include "../LibraryUtils/SourceBuildFlags.h"
#include "../RestAPI/ApiKeyInfoManager.h"
#include "../RestAPI/BinanceAPI.h"
#include "../SettingNConfig/BqtGlobalSettings.h"

#if USE_BACK_TEST_TRADING
#include "../TradingStrategies/StrategyMessageServer.h"
#include "../ExchangeConnectivity/ExchangeSimulatorConnectivity.h"
#endif

#include "StrategyFactory.h"
#include "TraderFactory.h"
#include "TradingModel.h"

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
using namespace SettingNConfig;
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
	m_logger->Info("Initiating Global Settings.");
	const auto* globalSettingsCfg = configBQTXml->FirstChildElement("GlobalSettings");
	BqtGlobalSettingsMgr->InitGlobalSetting(globalSettingsCfg);

	m_logger->Info("Initiating User Account And Binance API.");
	const auto* userAccountXml = configAccessKeyXml->FirstChildElement("UserAccount");
	assert(userAccountXml);
	const auto* keyXml = userAccountXml->FirstChildElement("Key");
	assert(keyXml);
	const auto* sk = keyXml->Attribute("SK"); // can be empty for test trading
	const auto* pk = keyXml->Attribute("PK"); // can be empty for test trading
	const auto* userID = keyXml->Attribute("UserID"); // must be always exist
	ApiKeyInfoMgr->InitApiKeyInfo(userID, pk, sk);

#if USE_BACK_TEST_TRADING
	m_logger->Info("Initiating Test Message Transporter.");
	const auto* messageTransporterCfg = configBQTXml->FirstChildElement("MessageTransporter");
	ExchangeSimulatorGateWay->InitMessageTransporter(messageTransporterCfg);

	m_logger->Info("Initiating Test Binance Wallet Client.");
	const auto* binanceWalletClientCfg = configBQTXml->FirstChildElement("BinanceWalletClient");
	ExchangeSimulatorGateWay->InitBinanceWalletClient(binanceWalletClientCfg);

	m_logger->Info("Initiating Test Binance Exchange Client.");
	const auto* binanceExchangeClientCfg = configBQTXml->FirstChildElement("BinanceExchangeClient");
	ExchangeSimulatorGateWay->InitBinanceExchangeClient(binanceExchangeClientCfg);
#else
	const auto* binanceAPICfg = configBQTXml->FirstChildElement("BinanceAPI");
	assert(binanceAPICfg);
	const auto* connectionXml = binanceAPICfg->FirstChildElement("Connection");
	assert(connectionXml);
	const auto* apiBinanceCom = connectionXml->Attribute("ApiBinanceCom");
	const auto* apiBinancePort = connectionXml->Attribute("ApiBinancePort");
	const auto* connectionTimeoutMs = connectionXml->Attribute("ConnectionTimeoutMs");
	BinanceAPI::GetInstance()->InitiateAPI(apiBinanceCom, apiBinancePort, pk, sk, connectionTimeoutMs);
#endif
	m_logger->Info("Initiating Static Data.");
	const auto* staticDataCfg = configBQTXml->FirstChildElement("StaticData");
	StaticDataMgr->LoadStaticDatabase(staticDataCfg);

	m_logger->Info("Initiating Historical Data.");
	const auto* historicalDataCfg = configBQTXml->FirstChildElement("HistoricalData");
	HistoricalDataMgr->LoadHistoricalDatabase(historicalDataCfg);

	m_logger->Info("Initiating Real Time Market Data.");
	const auto* realTimeMarketDataCfg = configBQTXml->FirstChildElement("RealTimeMarketData");
	m_binanceMarketDataConfig = BqtXmlUtils::GetBinanceMarketDataConfig(realTimeMarketDataCfg);
	const auto* binanceRealTimeMarketDataCfg = m_binanceMarketDataConfig->FirstChildElement("RealTimeMarketData");
	m_marketData = std::make_unique<RealTimeMarketData>(binanceRealTimeMarketDataCfg);

	m_logger->Info("Initiating Portfolio Investment.");
	const auto* portfolioCfg = configBQTXml->FirstChildElement("PortfolioInvestment");
	m_portfolio = std::make_unique<PortfolioInvestmentBinance>(portfolioCfg, m_marketData.get());

	m_logger->Info("Initiating Compliance And Regulatory.");
	const auto* complianceCfg = configBQTXml->FirstChildElement("ComplianceNRegulatory");
	m_tradingRules = std::make_unique<BinanceTradingRules>(complianceCfg);

	m_logger->Info("Initiating Risk Management.");
	const auto* riskManagementCfg = configBQTXml->FirstChildElement("RiskManagement");
	m_riskManager = std::make_unique<RiskManager>(riskManagementCfg);

	m_logger->Info("Initiating Trader.");
	const auto* traderXmlCfg = configBQTXml->FirstChildElement("TraderInfo");
	m_trader = TraderFactory::CreateSmartTrader(
		m_portfolio.get(),
		m_tradingRules.get(),
		m_riskManager.get(),
		traderXmlCfg);

	m_logger->Info("Initiating Trading Strategy.");
	const auto* strategyCfg = configBQTXml->FirstChildElement("TradingStrategy");
	m_strategy = StrategyFactory::CreateTargetStrategy(
		strategyCfg,
		m_marketData.get(),
		m_trader.get(),
		m_tradingRules.get());

#if USE_BACK_TEST_TRADING
	m_logger->Info("Initiating Test Message Server.");
	const auto* messageServerCfg = configBQTXml->FirstChildElement("MessageServer");
	m_strategyMessageServer = std::make_unique<StrategyMessageServer>(messageServerCfg);
	m_strategyMessageServer->RegisterMessageHandler(m_strategy.get());
#endif

	m_logger->Info("Starting External Controller.");
	const auto* externalControllerCfg = configBQTXml->FirstChildElement("ExternalController");
	assert(externalControllerCfg);
	m_externalController = std::make_unique<ExternalController>(externalControllerCfg);

	m_logger->Info("Initiating Trading Model.");
	const auto* tradingModelCfg = configBQTXml->FirstChildElement("TradingModel");
	m_allowMutipleThreadTrade = tradingModelCfg->FirstChildElement("MultipleThreads")->BoolAttribute("Enable");
}

void TradingModel::RunModel()
{
	// Start the external controller, this is for setup parameter control from external applications
	m_externalController->Start(); // this is a child thread
	// Start the strategy message server
	// This is a child thread
	// It will be used to receive messages from the exchange simulator
	// for testing trade
#if USE_BACK_TEST_TRADING
	// -This message receiver is a BQT Json Message server
	// We will use it to receive acks from the Exchange Simulator
	// for testing trade
	// -We dont have a Binance message server, because Binance API 
	// is using REST protocol so we will use HTTP/REST protocol instead
	m_strategyMessageServer->Start(); // this is a child thread
	m_logger->Info(USE_BACK_TEST_TRADING_MESSAGE);
#elif USE_BINANCE_TEST_TRADING
	m_logger->Info(USE_BINANCE_TEST_TRADING_MESSAGE);
#else
	m_logger->Info(USE_REAL_TRADING_MESSAGE);
#endif
	m_logger->Info(USE_MULTITHREADING_MESSAGE);
#if USE_MULTITHREADING // control from build setup
	// If Strategies and Trading Services (Market Data,...) want to run in mutiple threads
	// to avoid stale trading signals but the trade might be slower than single thread mode
	if (m_strategy && m_strategyHost && m_allowMutipleThreadTrade)
	{
		if (m_strategyHost && m_allowMutipleThreadTrade) // control from configuration setup
		{
			m_strategyHost->StartStrategyThread(m_strategy.get());  // this is a child thread
		}
		else //If Strategies and Trading Services (Market Data,...) want to run in single thread
		{
			m_strategy->StartLive();
		}
	}
#else
	// If Strategies and Trading Services (Market Data,...) want to run in single thread
	if (m_strategy)
	{
		m_strategy->StartLive();
	}
#endif
	// Start market data streaming
	if (m_marketData)
	{
		// Start receive real time market data and analyze to find trading opportunity signals
		m_marketData->StartStreamingData();  // this is a child thread
	}
}
