/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "ApiKeyInfoManager.h"

#include <stdexcept>

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
    if (userID.empty() || sk.empty() || pk.empty())
    {
        throw std::runtime_error("ApiKeyInfoManager: ApiKeyInfo(userID, sk, pk) from config is invalid.");
    }
    m_ApiKeyInfo = ApiKeyInfo(userID, sk, pk);
}

const ApiKeyInfo&
ApiKeyInfoManager::GetApiKeyInfo() const
{
    return m_ApiKeyInfo;
}

