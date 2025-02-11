/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "BinanceAPI.h"

#pragma warning(disable : 4005)

// This is really a hack to use the class RestAPI within this file lol!
#define DLL_CLASS_RESTAPI_EXPORTS _declspec(dllimport)

#include "RestAPI.h"

using namespace RestAPI;

BinanceAPI::~BinanceAPI() {}

BinanceAPI* BinanceAPI::GetInstance()
{
    static BinanceAPI instance;
    return &instance;
}

void BinanceAPI::InitiateAPI(
    const std::string& host,
    const std::string& port,
    const std::string& pk,
    const std::string& sk,
    const std::string& timeout)
{
    m_binAPI = std::make_unique<binapi::rest::api>(m_ioContext, 
        host, port, pk, sk, static_cast<std::size_t>(std::stoul(timeout)));
}

binapi::rest::api* BinanceAPI::AccessAPI()
{
    return m_binAPI.get();
}
