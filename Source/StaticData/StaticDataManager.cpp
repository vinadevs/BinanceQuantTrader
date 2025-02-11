/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "../SettingNConfig/tinyxml2.h"

#include "StaticDataManager.h"

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
    m_stablecoinUSDT = "USDT";
}

const std::string& StaticDataManager::GetStableCoinUSDTSymbol() const
{
    return m_stablecoinUSDT;
}
