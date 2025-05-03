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
#include "../RestAPI/RestAPI.h"
#include "../RestAPI/BinanceAPI.h"

#include "StaticDataManager.h"

#include <cassert>

using namespace StaticData;
using namespace tinyxml2;
using namespace RestAPI;

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
    const auto exchangeInfoResult = BinanceApiGateWay->exchange_info();
    if (exchangeInfoResult)
	{
        for (const auto& asset : exchangeInfoResult.v.symbols)
        {
            symbols.emplace_back(asset.second.symbol);
        }
        LOG_INFO_STREAM(m_logger, "Updated exchange_info for all symbols");
        if (logDataToFile)
        {
            const auto exchangeInfoPath = BqtGlobalSettingsMgr->GetdDataAppPath() + "//exchange_info_all.txt";
            binapi::rest::exchange_info_t::write_exchange_info_to_file(exchangeInfoPath, exchangeInfoResult.v);
        }
    }
    else
    {
        LOG_ERROR_STREAM(m_logger, "exchange_info error=" << exchangeInfoResult.errmsg);
    }
    return symbols;
}

const std::string& StaticDataManager::GetStableCoinUSDTSymbol() const
{
    return m_stablecoinUSDT;
}
