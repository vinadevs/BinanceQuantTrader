/********************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include "../RestAPI/RestAPI.h"
#include "../RestAPI/BinanceAPI.h"
#include "../LibraryUtils/Logger.h"
#include "../SettingNConfig/BqtGlobalSettings.h"

#include "BinanceExchangeProfile.h"

using namespace ComplianceNRegulatory;
using namespace RestAPI;

BinanceExchangeProfileMgr::BinanceExchangeProfileMgr(const std::string& filePath)
    : m_logger{ std::make_unique<LibraryUtils::Logger>("BinanceExchangeProfileMgr") }
{
    LoadStaticExchangeProfilesFromFile(filePath);
}

BinanceExchangeProfileMgr::~BinanceExchangeProfileMgr() {}

bool BinanceExchangeProfileMgr::UpdateRemoteExchangeProfiles(
    const std::string& symbol,
    const bool logDataToFile)
{
    const auto exchangeInfo = BinanceApiGateWay->exchange_info(symbol);
    if (!exchangeInfo)
    {
        LOG_ERROR_STREAM(m_logger, "exchange_info error: " << exchangeInfo.errmsg);
        return false;
    }
    if (logDataToFile) {
        const auto exchangeInfoPath = BqtGlobalSettingsMgr->GetdDataAppPath() + "//exchange_info_" + symbol + ".txt";
        RemoteBinanceExchangeProfile::write_exchange_info_to_file(exchangeInfoPath, exchangeInfo.v);
    }
    return m_exchangeRemoteProfiles.try_emplace(symbol, std::move(exchangeInfo.v)).second;
}

const StaticBinanceExchangeProfile* BinanceExchangeProfileMgr::LookupStaticExchangeProfile(const std::string& symbol) const {
    const auto it = m_exchangeStaticProfiles.find(symbol);
    return (it != m_exchangeStaticProfiles.end()) ? &it->second : nullptr;
}

const RemoteBinanceExchangeProfile* BinanceExchangeProfileMgr::LookupRemoteExchangeProfile(const std::string& symbol) const
{
    const auto it = m_exchangeRemoteProfiles.find(symbol);
    return (it != m_exchangeRemoteProfiles.end()) ? &it->second : nullptr;
}

void BinanceExchangeProfileMgr::LoadStaticExchangeProfilesFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open Binance Exchange Profile file=" + filePath);
    }

    std::string line;
    bool firstLine = true;
    while (std::getline(file, line)) {
        if (firstLine) { // Skip header row
            firstLine = false;
            continue;
        }

        std::stringstream ss(line);
        StaticBinanceExchangeProfile pair;
        std::string temp;

        std::getline(ss, pair.m_tradingPair, ',');
        std::getline(ss, temp, ','); pair.m_minTradeAmount = std::stod(temp);
        std::getline(ss, temp, ','); pair.m_minAmountMovement = std::stod(temp);
        std::getline(ss, temp, ','); pair.m_minPriceMovement = std::stod(temp);
        std::getline(ss, temp, ','); pair.m_minOrderSize = std::stod(temp);
        std::getline(ss, temp, ','); pair.m_maxMarketOrderAmount = std::stod(temp);
        std::getline(ss, temp, ','); pair.m_maxOpenLimitOrders = std::stoi(temp);
        std::getline(ss, temp, ','); pair.m_maxOpenConditionalOrders = std::stoi(temp);

        m_exchangeStaticProfiles[pair.m_tradingPair] = pair; // Store in the map for fast lookup
    }
    file.close();
}