/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/PathUtils.h"
#include "../LibraryUtils/StringUtils.h"

#include "BqtXmlUtils.h"

#include <cassert>
#include <filesystem>

using namespace SettingNConfig;
using namespace tinyxml2;

static std::pair<std::string, std::unique_ptr<tinyxml2::XMLDocument>> GetConfigAsType(const std::string& marketDataName, const std::string& marketDataCfgFile)
{
    auto marketDataCfgXml = std::make_unique<XMLDocument>();
    const auto errLoadFileXml = marketDataCfgXml->LoadFile(marketDataCfgFile.c_str());
    if (errLoadFileXml != XML_SUCCESS)
    {
        throw std::runtime_error("MarketDataFactory: Load file Xml error="
            + std::string(XMLDocument::ErrorIDToName(errLoadFileXml)) + ", error path:" + marketDataCfgFile);
    }
    return { marketDataName, std::move(marketDataCfgXml) }; // Move ownership
}

std::pair<std::string, std::unique_ptr<tinyxml2::XMLDocument>> BqtXmlUtils::GetMarketDataConfig(const tinyxml2::XMLElement* realTimeMarketDataCfg)
{
    assert(realTimeMarketDataCfg);
    const auto* usingMarketDataXml = realTimeMarketDataCfg->FirstChildElement("UsingMarketData");
    assert(usingMarketDataXml);
    std::string marketDataCfgFile(usingMarketDataXml->Attribute("File"));
    PathUtils::ReplaceSubString(marketDataCfgFile, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());
    if (std::filesystem::exists(marketDataCfgFile))
    {
        if (StringUtils::IsConfigAttributeMatched(usingMarketDataXml->Attribute("Name"), "RealTimeMarketData"))
        {
			return GetConfigAsType("RealTimeMarketData", marketDataCfgFile);
        }
        if (StringUtils::IsConfigAttributeMatched(usingMarketDataXml->Attribute("Name"), "HistoricalMarketData"))
        {
            return GetConfigAsType("HistoricalMarketData", marketDataCfgFile);
		}
        else
        {
            throw std::runtime_error("MarketDataFactory: unsupported MarketData config");
        }
    }
    else
    {
        throw std::runtime_error("MarketDataFactory: File does not exist=" + marketDataCfgFile);
    }
}
