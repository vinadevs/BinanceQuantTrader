/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "SmartLongShortStrategy.h"

#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/StringUtils.h"
#include "../MarketData/RealTimeMarketData.h"
#include "../MarketData/SynchronousMarketData.h"
#include "../StaticData/StaticDataManager.h"
#include "../UserAccount/FutureTrader.h"
#include "../ComplianceNRegulatory/BinanceTradingRules.h"
#include "../ComplianceNRegulatory/BinanceExchangeProfile.h"
#include "../QuantitativeModel/QuantOrderParammeter.h"
#include "../LibraryUtils/PathUtils.h"
#include "../LibraryUtils/FileUtils.h"

using namespace TradingStrategies;
using namespace QuantitativeModel;
using namespace MarketData;
using namespace UserAccount;
using namespace ComplianceNRegulatory;
using namespace LibraryUtils;
using namespace tinyxml2;

SmartLongShortStrategy::SmartLongShortStrategy(
	const std::string& strategyCfgPath,
	RealTimeMarketData* marketData,
	Trader* trader,
	BinanceTradingRules* tradingRules)
	: TradingStrategyBase("SmartLongShortStrategy", "Create future smart orders...",
		strategyCfgPath, marketData, trader, tradingRules)
{
	SetStrategyType(StrategyType::FULL_AUTO);
	InitializeParameters(strategyCfgPath);
	m_logger->Info("Completed initialization for the strategy.");
}

SmartLongShortStrategy::~SmartLongShortStrategy()
{
	m_marketData->UnRegisterDataListener(this); // I earn enough money, leave the market now!
}

bool SmartLongShortStrategy::OnIndividualBookTickerChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		// Test future orders
		QuantOrderParammeter futureOrder;
		futureOrder.m_symbol = symbol;
		futureOrder.m_side = binapi::e_side::buy; // default to buy/long
		futureOrder.m_type = binapi::e_type::market; // default to market order
		futureOrder.m_time = binapi::e_time::GTC;// default to GTC
		futureOrder.m_amount = 1; // default to 1 coin
		futureOrder.m_price = syncedData->m_individualBookTickerData.m_bestBidPrice->GetDoubleData(); // use best ask price as market price
		futureOrder.m_tradeType = OrderManagement::BinanceNewOrderTradingType::FUTURE; // set to future trading type
		futureOrder.m_stableCurrency = "USDT"; // default stable currency is USDT

		// Set leverage ratio
		futureOrder.m_leverageRatio = 50; // default leverage ratio is x50

		//std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // simulate some delay

		const auto newFutureOrder = m_futureTrader->CreateNewPosition(futureOrder);
		if (newFutureOrder.first)
		{
			if (futureOrder.m_side == binapi::e_side::buy)
			{
				m_logger->Info("Created a new [Long Position] for symbol=" + symbol);
			}
			else if (futureOrder.m_side == binapi::e_side::sell)
			{
				m_logger->Info("Created a new [Short Position] for symbol=" + symbol);
			}
			IncreaseComplianceRestAPIRequestCounter(1); // register a sent http request to ComplianceNRegulatory
			ReportTradeResults(symbol);
			IncreaseComplianceRestAPIRequestCounter(2); // register a sent http request to ComplianceNRegulatory

			// test cancel order
			//const auto newFutureCancelOrder = m_futureTrader->CancelOpenPosition(newFutureOrder.second);
			//if (newFutureCancelOrder.first)
			//{
			//	m_logger->Info("Canceled the open position for symbol=" + symbol);
			//}
			//else
			//{
			//	m_logger->Error("Failed to cancel the open position for symbol=" + symbol);
			//}
			//IncreaseComplianceRestAPIRequestCounter(1); // register a sent http request to ComplianceNRegulatory
		}
		return true;
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

void SmartLongShortStrategy::ReportTradeResults(const std::string& symbol)
{
	m_futureTrader->ReportTradeResults(symbol);
}

void SmartLongShortStrategy::InitializeParameters(const std::string& strategyCfgPath)
{
	m_strategyCfgXml = std::make_unique<XMLDocument>();
	const auto errLoadFileXml = m_strategyCfgXml->LoadFile(strategyCfgPath.c_str());
	if (errLoadFileXml != XML_SUCCESS)
	{
		throw std::runtime_error("SmartLongShortStrategy: Load file Xml error="
			+ std::string(XMLDocument::ErrorIDToName(errLoadFileXml)) + ", error path:" + strategyCfgPath);
	}
	SetupStrategyLifeTime(m_strategyCfgXml.get());
}

void SmartLongShortStrategy::StartLive()
{
	// Change Strategy state to live
	m_strategyRunStatus = StrategyRunStatus::LIVE;
	// Prepare target symbols list
	m_logger->Info("Prepare target symbols list.");
	PrepareTargetMonitorSymbols();
	// Create exchange filter profile
	m_logger->Info("Create binance exchange profile.");
	CreateBinanceExchangeProfile();
	// Create portfolio management
	m_logger->Info("Create portfolio management.");
	CreatePortfolioManagement();
	// Subscribe target symbols to receive real time market data
	m_logger->Info("Subscribe target symbols.");
	SubscribeTargetSymbols();
}

void SmartLongShortStrategy::StopLive()
{
	m_strategyRunStatus = StrategyRunStatus::STOP;
	// Unsubscribe target symbols to stop receiving real time market data
	m_logger->Info("Unsubscribe target symbols.");
	UnsubscribeTargetSymbols();
}

void SmartLongShortStrategy::CreateBinanceExchangeProfile()
{
	for (const auto& symbol : m_targetFutureTradeSymbols)
	{
		m_tradingRules->GetExchangeProfileMgr()->UpdateRemoteExchangeProfiles(symbol, true);
		IncreaseComplianceRestAPIRequestCounter(1); // register a sent http request to ComplianceNRegulatory
	}
}

void SmartLongShortStrategy::CreatePortfolioManagement()
{
	m_futureTrader->CreatePortfolioManagement(m_targetFutureTradeSymbols);
	IncreaseComplianceRestAPIRequestCounter(1); // register a sent http request to ComplianceNRegulatory
}

void SmartLongShortStrategy::PrepareTargetMonitorSymbols()
{
	const auto* targetSymbolXml = m_strategyCfgXml->FirstChildElement("TargetSymbol");
	assert(targetSymbolXml);
	const XMLElement* symbolsXml = targetSymbolXml->FirstChildElement("AllSymbols");
	assert(symbolsXml);
	const bool useRemoteExchangeList = symbolsXml->BoolAttribute("UseRemoteExchangeList");
	if (useRemoteExchangeList)
	{
		m_logger->Info("Querying remote binance exchange listing symbols info...");
		//m_targetFutureTradeSymbols = StaticDataMgr->GetAllRemoteListingSymbols(true);
		m_targetFutureTradeSymbols.emplace_back("BTCUSDT");
		m_targetFutureTradeSymbols.emplace_back("ETHUSDT");
		m_targetFutureTradeSymbols.emplace_back("BNBUSDT");
#ifdef SAVE_BINANCE_LISTINGS // remove this macro to saving binance listings
		FileUtils::FromVectorStringToFile(m_targetFutureTradeSymbols, PathUtils::GetApplicationFolderPath()
			+ "\\Configurations\\Common\\BinanceListings.txt");
#endif // DEBUG
	}
	else
	{
		std::string localListingFile(symbolsXml->Attribute("LocalListingFile"));
		PathUtils::ReplaceSubString(localListingFile, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());
		m_targetFutureTradeSymbols = FileUtils::ReadFileContentToLines(localListingFile, true);
	}
}

void SmartLongShortStrategy::SubscribeTargetSymbols()
{
	if (m_targetFutureTradeSymbols.empty())
	{
		throw std::runtime_error("No target symbols to subscribe market data.");
	}
	// register this class with market data to receive real time data
	m_marketData->RegisterDataListener(this);
	// subscibe all target symbols
	for (const auto& symbol : m_targetFutureTradeSymbols)
	{
		m_marketData->SubscribeSymbol(symbol);
	}
	m_marketData->StartIOContext();
}

void SmartLongShortStrategy::UnsubscribeTargetSymbols()
{
	for (const auto& symbol : m_targetFutureTradeSymbols)
	{
		m_marketData->UnsubscribeSymbol(symbol);
	}
}
