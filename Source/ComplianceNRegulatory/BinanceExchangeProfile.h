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

namespace ComplianceNRegulatory {

#pragma once

struct DLL_CLASS_COMPLIANCENREGULATORY_EXPORTS BinanceExchangeProfile final {
    std::string m_tradingPair;
    double m_minTradeAmount{ 0 };
    double m_minAmountMovement{ 0 };
    double m_minPriceMovement{ 0 };
    double m_minOrderSize{ 0 };
    double m_maxMarketOrderAmount{ 0 };
    int m_maxOpenLimitOrders{ 0 };
    int m_maxOpenConditionalOrders{ 0 };
};

class BinanceExchangeProfileMgr final {
public:
    BinanceExchangeProfileMgr(const std::string& filePath) {
        LoadFromFile(filePath);
    }

    const BinanceExchangeProfile* LookupExchangeProfile(const std::string& pair) const {
        auto it = m_exchangeProfiles.find(pair);
        return (it != m_exchangeProfiles.end()) ? &it->second : nullptr;
    }

    void PrintAllPairs() const {
        for (const auto& [pairName, pairInfo] : m_exchangeProfiles) {
            std::cout << "Pair: " << pairInfo.m_tradingPair
                << ", Min Trade: " << pairInfo.m_minTradeAmount
                << ", Min Movement: " << pairInfo.m_minAmountMovement
                << ", Min Price Move: " << pairInfo.m_minPriceMovement
                << ", Min Order Size: " << pairInfo.m_minOrderSize
                << ", Max Market Order: " << pairInfo.m_maxMarketOrderAmount
                << ", Max Open Limit Orders: " << pairInfo.m_maxOpenLimitOrders
                << ", Max Open Conditional Orders: " << pairInfo.m_maxOpenConditionalOrders
                << std::endl;
        }
    }
private:
    void LoadFromFile(const std::string& filePath) {
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
            BinanceExchangeProfile pair;
            std::string temp;

            std::getline(ss, pair.m_tradingPair, ',');
            std::getline(ss, temp, ','); pair.m_minTradeAmount = std::stod(temp);
            std::getline(ss, temp, ','); pair.m_minAmountMovement = std::stod(temp);
            std::getline(ss, temp, ','); pair.m_minPriceMovement = std::stod(temp);
            std::getline(ss, temp, ','); pair.m_minOrderSize = std::stod(temp);
            std::getline(ss, temp, ','); pair.m_maxMarketOrderAmount = std::stod(temp);
            std::getline(ss, temp, ','); pair.m_maxOpenLimitOrders = std::stoi(temp);
            std::getline(ss, temp, ','); pair.m_maxOpenConditionalOrders = std::stoi(temp);

            m_exchangeProfiles[pair.m_tradingPair] = pair; // Store in the map for fast lookup
        }
        file.close();
    }

    std::unordered_map<std::string, BinanceExchangeProfile> m_exchangeProfiles;
};
};
