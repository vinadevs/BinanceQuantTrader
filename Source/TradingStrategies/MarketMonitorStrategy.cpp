/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "MarketMonitorStrategy.h"

#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/StringUtils.h"
#include "../MarketData/RealTimeMarketData.h"
#include "../MarketData/SynchronousMarketData.h"
#include "../StaticData/StaticDataManager.h"
#include "../LibraryUtils/PathUtils.h"
#include "../LibraryUtils/FileUtils.h"
#include "../RestAPI/RestAPI.h"
#include "../RestAPI/BinanceSpotApiGateWay.h"
#include "../QuantitativeModel/MarketDataAnalyzer.h"
#include "../QuantitativeModel/QuantMarketDataAnalyzer.h"

using namespace TradingStrategies;
using namespace MarketData;
using namespace QuantitativeModel;
using namespace LibraryUtils;
using namespace tinyxml2;

MarketMonitorStrategy::MarketMonitorStrategy(
	const std::string& strategyCfgPath,
	MarketData::RealTimeMarketData* marketData)
	: TradingStrategyBase("MarketMonitorStrategy",
		"analysis real time market data and generate trading signals...",
		strategyCfgPath, marketData)
{
	START_STRATEGY_INITIALIZATION_SECTION

	SetStrategyType(StrategyType::ADVISING);
	InitializeParameters(strategyCfgPath);
	m_logger->Info("Completed initialization for the strategy.");

	END_STRATEGY_INITIALIZATION_SECTION
}

MarketMonitorStrategy::~MarketMonitorStrategy()
{
	m_marketData->UnRegisterDataListener(this); // I earn enough money, leave the market now!
}

void MarketMonitorStrategy::InitializeMarketDataAnalyzer()
{
	m_marketDataAnalyzer = std::make_unique<QuantitativeModel::MarketDataAnalyzer>(m_targetMonitorSymbols, m_logger.get());
}

bool MarketMonitorStrategy::OnIndividualBookTickerChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisIndividualBookTicker(syncedData->m_individualBookTickerData);
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool MarketMonitorStrategy::OnTradeChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisTrade(syncedData->m_tradeData);
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool MarketMonitorStrategy::OnIndividualMarketTickerChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisIndividualMarketTicker(syncedData->m_individualMarketTickerData);
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool MarketMonitorStrategy::OnMiniTickerChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisMiniTicker(syncedData->m_individualMiniTickerData);
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool MarketMonitorStrategy::OnAggregateTradeChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisAggregateTrade(syncedData->m_aggregateTradeData);
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool MarketMonitorStrategy::OnKlineCandleStickChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisKlineCandleStick(syncedData->m_klineCandleStickData);
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool MarketMonitorStrategy::OnAllMarketTickersChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisAllMarketTickers(syncedData->m_allMarketTickerData);
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool MarketMonitorStrategy::OnAllMiniTickersChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisAllMiniTickers(syncedData->m_allMiniTickerData);
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool MarketMonitorStrategy::OnAllPartDepthChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisAllMarketDepth(syncedData->m_allPartDepthData);
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool MarketMonitorStrategy::OnAllDiffDepthChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (const auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
		if (auto* analyzer = m_marketDataAnalyzer->GetQuantMarketDataAnalyzer(symbol))
		{
			analyzer->AnalysisAllMarketDepthDiff(syncedData->m_allDiffDepthData);
			return true;
		}
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

void MarketMonitorStrategy::InitializeParameters(const std::string& strategyCfgPath)
{
	m_strategyCfgXml = std::make_unique<XMLDocument>();
	const auto errLoadFileXml = m_strategyCfgXml->LoadFile(strategyCfgPath.c_str());
	if (errLoadFileXml != XML_SUCCESS)
	{
		throw std::runtime_error("MarketMonitorStrategy: Load file Xml error="
			+ std::string(XMLDocument::ErrorIDToName(errLoadFileXml)) + ", error path:" + strategyCfgPath);
	}
	SetupStrategyLifeTime(m_strategyCfgXml.get());
}

void MarketMonitorStrategy::StartTrade()
{
	try
	{
		// Change Strategy state to live
		m_strategyRunStatus.store(StrategyRunStatus::LIVE, std::memory_order_release);
		// Prepare target symbols list
		m_logger->Info("Prepare target symbols list.");
		PrepareTargetMonitorSymbols();
		// Create Market Data Analyzer
		m_logger->Info("Create market data analyzer.");
		InitializeMarketDataAnalyzer();
		// Subscribe target symbols to receive real time market data
		m_logger->Info("Subscribe target symbols.");
		SubscribeTargetSymbols();
	}
	catch (const std::exception& e)
	{
		m_logger->Exception(std::string(e.what()));
	}
	catch (...)
	{
		m_logger->Exception("Unknown exception occurred.");
	}
}

void MarketMonitorStrategy::StopTrade()
{
	m_strategyRunStatus.store(StrategyRunStatus::STOP, std::memory_order_release);
	// Unsubscribe target symbols to stop receiving real time market data
	m_logger->Info("Unsubscribe target symbols.");
	UnsubscribeTargetSymbols();
}

void MarketMonitorStrategy::PrepareTargetMonitorSymbols()
{
	const auto* targetSymbolXml = m_strategyCfgXml->FirstChildElement("TargetSymbol");
	assert(targetSymbolXml);
	const XMLElement* symbolsXml = targetSymbolXml->FirstChildElement("AllSymbols");
	assert(symbolsXml);
	const bool useRemoteExchangeList = symbolsXml->BoolAttribute("UseRemoteExchangeList");
	if (useRemoteExchangeList)
	{
		m_logger->Info("Querying remote binance exchange listing symbols info...");
		m_targetMonitorSymbols = StaticDataMgr->GetAllRemoteListingSymbols(true);
#ifdef SAVE_BINANCE_LISTINGS // remove this macro to saving binance listings
		FileUtils::FromVectorStringToFile(m_targetMonitorSymbols, 
			(std::filesystem::path(PathUtils::GetApplicationFolderPath()) / "Configurations" / "Common" / "BinanceListings.txt").string());
#endif // DEBUG
	}
	else
	{
		std::string localListingFile(symbolsXml->Attribute("LocalListingFile"));
		PathUtils::ReplaceSubString(localListingFile, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());
		m_targetMonitorSymbols = FileUtils::ReadFileContentToLines(localListingFile, true);
	}
}

void MarketMonitorStrategy::SubscribeTargetSymbols()
{
	if (m_targetMonitorSymbols.empty())
	{
		throw std::runtime_error("No target symbols to subscribe market data.");
	}
	// register this class with market data to receive real time data
	m_marketData->RegisterDataListener(this);
	// subscibe all target symbols
	for (const auto& symbol : m_targetMonitorSymbols)
	{
		m_marketData->SubscribeSymbol(symbol);
	}
}

void MarketMonitorStrategy::UnsubscribeTargetSymbols()
{
	for (const auto& symbol : m_targetMonitorSymbols)
	{
		m_marketData->UnsubscribeSymbol(symbol);
	}
}
