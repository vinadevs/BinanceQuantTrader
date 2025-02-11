/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "../SettingNConfig/tinyxml2.h"

#include "HistoricalDataManager.h"

using namespace HistoricalData;
using namespace tinyxml2;

HistoricalDataManager::~HistoricalDataManager() {}

HistoricalDataManager* HistoricalDataManager::GetInstance()
{
    static HistoricalDataManager instance;
    return &instance;
}

void HistoricalDataManager::LoadHistoricalDatabase(const XMLElement* staticDataConfigXml)
{
}
