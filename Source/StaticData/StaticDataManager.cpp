/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../SettingNConfig/tinyxml2.h"
#include "../SettingNConfig/BqtGlobalSettings.h"
#include "../CurlAPI/CurlAPIGateWay.h"
#include "../LibraryUtils/SourceBuildFlags.h"

#include "StaticDataManager.h"

#include <cassert>

using namespace StaticData;
using namespace tinyxml2;

StaticDataManager::~StaticDataManager() {}

StaticDataManager* StaticDataManager::GetInstance()
{
    static StaticDataManager instance;
    return &instance;
}

void StaticDataManager::LoadStaticDatabase(const XMLElement* staticDataConfigXml)
{
    assert(staticDataConfigXml);
    m_stablecoinUSDT = "USDT";
}

std::vector<std::string> StaticDataManager::GetAllRemoteListingSymbols(const bool logDataToFile)
{
    std::vector<std::string> symbols;
#ifdef USE_BINANCE_TEST_TRADING
    symbols = CurlAPI::GetBinanceListingSymbols("TRADING");
#endif // USE_BINANCE_TEST_TRADING
    return symbols;
}

const std::string& StaticDataManager::GetStableCoinUSDTSymbol() const
{
    return m_stablecoinUSDT;
}
