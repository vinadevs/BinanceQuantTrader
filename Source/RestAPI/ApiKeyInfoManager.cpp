/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "ApiKeyInfoManager.h"

using namespace RestAPI;

ApiKeyInfoManager::~ApiKeyInfoManager() {}

ApiKeyInfoManager* ApiKeyInfoManager::GetInstance()
{
    static ApiKeyInfoManager instance;
    return &instance;
}

void ApiKeyInfoManager::InitApiKeyInfo(
    const std::string& userID,
    const std::string& sk,
    const std::string& pk)
{
    m_ApiKeyInfo = ApiKeyInfo(userID, sk, pk);
}

const ApiKeyInfo&
ApiKeyInfoManager::GetApiKeyInfo() const
{
    return m_ApiKeyInfo;
}

