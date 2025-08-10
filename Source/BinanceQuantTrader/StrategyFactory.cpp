/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "StrategyFactory.h"

#include "../SettingNConfig/tinyxml2.h"
#include "../MarketData/RealTimeMarketData.h"

// list of strategy types supported
#include "../TradingStrategies/TestTradingStrategy.h"
#include "../TradingStrategies/TradingStrategyBase.h"
#include "../TradingStrategies/FomoTradingStrategy.h"
#include "../TradingStrategies/MarketMonitorStrategy.h"
#include "../TradingStrategies/StopLossStrategy.h"
#include "../TradingStrategies/SmartLongShortStrategy.h"
#include "../TradingStrategies/VWAPStrategy.h"

#include "../LibraryUtils/PathUtils.h"
#include "../LibraryUtils/StringUtils.h"

#include <filesystem>
#include <exception>

using namespace BinanceQuantTrader;
using namespace TradingStrategies;
using namespace MarketData;
using namespace UserAccount;
using namespace ComplianceNRegulatory;
using namespace RiskManagement;
using namespace tinyxml2;

std::unique_ptr<TradingStrategyBase>
StrategyFactory::CreateTargetStrategy(
	const XMLElement* strategyXmlCfg,
	RealTimeMarketData* marketData,
	Trader* trader,
	BinanceTradingRules* tradingRules)
{
	assert(strategyXmlCfg);
	const auto* usingStrategyXml = strategyXmlCfg->FirstChildElement("UsingStrategy");
	assert(usingStrategyXml);
	std::string strategyCfgFile(usingStrategyXml->Attribute("File"));
	PathUtils::ReplaceSubString(strategyCfgFile, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());
	if (std::filesystem::exists(strategyCfgFile))
	{
		if (StringUtils::IsConfigAttributeMatched(usingStrategyXml->Attribute("Name"), "FomoStrategy"))
		{
			return std::make_unique<FomoTradingStrategy>(strategyCfgFile, marketData, trader, tradingRules);
		}
		else if (StringUtils::IsConfigAttributeMatched(usingStrategyXml->Attribute("Name"), "MarketMonitorStrategy"))
		{
			return std::make_unique<MarketMonitorStrategy>(strategyCfgFile, marketData);
		}
		else if (StringUtils::IsConfigAttributeMatched(usingStrategyXml->Attribute("Name"), "StopLossStrategy"))
		{
			return std::make_unique<StopLossStrategy>(strategyCfgFile, marketData, trader, tradingRules);
		}
		else if (StringUtils::IsConfigAttributeMatched(usingStrategyXml->Attribute("Name"), "SmartLongShortStrategy"))
		{
			return std::make_unique<SmartLongShortStrategy>(strategyCfgFile, marketData, trader, tradingRules);
		}
		else if (StringUtils::IsConfigAttributeMatched(usingStrategyXml->Attribute("Name"), "VWAPStrategy"))
		{
			//return std::make_unique<VWAPStrategy>(strategyCfgFile, marketData, trader, tradingRules);
		}
		else if (StringUtils::IsConfigAttributeMatched(usingStrategyXml->Attribute("Name"), "TestStrategy"))
		{
			return std::make_unique<TestTradingStrategy>(strategyCfgFile, marketData, trader, tradingRules);
		}
		else
		{
			throw std::runtime_error("StrategyFactory: unsupported TradingStrategy config");
		}
	}
	else
	{
		throw std::runtime_error("StrategyFactory: File does not exist=" + strategyCfgFile);
	}
}
