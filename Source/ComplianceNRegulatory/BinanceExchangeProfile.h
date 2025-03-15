/********************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "../KernelTrading/types.h"

namespace LibraryUtils {
    class Logger;
};

namespace ComplianceNRegulatory {

struct DLL_CLASS_COMPLIANCENREGULATORY_EXPORTS 
    StaticBinanceExchangeProfile final {
    std::string m_tradingPair;
    double m_minTradeAmount{ 0 };
    double m_minAmountMovement{ 0 };
    double m_minPriceMovement{ 0 };
    double m_minOrderSize{ 0 };
    double m_maxMarketOrderAmount{ 0 };
    int m_maxOpenLimitOrders{ 0 };
    int m_maxOpenConditionalOrders{ 0 };
};

using RemoteBinanceExchangeProfile = binapi::rest::exchange_info_t;

class DLL_CLASS_COMPLIANCENREGULATORY_EXPORTS 
    BinanceExchangeProfileMgr final {
public:
    BinanceExchangeProfileMgr(const std::string& filePath);
    ~BinanceExchangeProfileMgr();

    // Should call only onne time at the starting of the trade.
    bool UpdateRemoteExchangeProfiles(const std::string& symbol, const bool logDataToFile = false);

    const StaticBinanceExchangeProfile* LookupStaticExchangeProfile(const std::string& symbol) const;
    const RemoteBinanceExchangeProfile* LookupRemoteExchangeProfile(const std::string& symbol) const;
private:
    void LoadStaticExchangeProfilesFromFile(const std::string& filePath);

    //////////////////////////////////
    std::unordered_map<std::string, StaticBinanceExchangeProfile> m_exchangeStaticProfiles;
    std::unordered_map<std::string, RemoteBinanceExchangeProfile> m_exchangeRemoteProfiles;
    std::unique_ptr<LibraryUtils::Logger> m_logger;
};
};
