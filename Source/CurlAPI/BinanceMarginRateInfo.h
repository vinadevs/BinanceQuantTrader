/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include <string>
#include <vector>

namespace CurlAPI {
    /**
     * @brief Represents a single leverage bracket tier for a symbol
     */
    struct LeverageBracket {
        int m_Tier;
        double m_NotionalCap;           ///< Max notional value for this tier
        int m_InitialLeverage;          ///< Max leverage allowed in this tier
        double m_InitialMarginRate;     ///< Initial margin rate (IMR)
        double m_MaintMarginRate;       ///< Maintenance margin rate (MMR)
        // Constructor
        LeverageBracket(int tier, double notionalCap, int initialLeverage, double imr, double mmr)
			: m_Tier(tier),
            m_NotionalCap(notionalCap),
            m_InitialLeverage(initialLeverage),
            m_InitialMarginRate(imr),
            m_MaintMarginRate(mmr) {
        }
		// Default constructor
		LeverageBracket() : m_Tier(0), m_NotionalCap(0.0), m_InitialLeverage(0), m_InitialMarginRate(0.0), m_MaintMarginRate(0.0) {}
    };

    /**
     * @brief Represents all leverage bracket tiers for a given trading symbol
     */
    struct SymbolMarginRateInfo {
        std::string m_Symbol;                      ///< Trading pair symbol (e.g., "BTCUSDT")
        std::vector<LeverageBracket> m_Brackets;   ///< All leverage tiers for this symbol
        // Constructor
        SymbolMarginRateInfo(const std::string& symbol) : m_Symbol(symbol) {}
		// Default constructor
		SymbolMarginRateInfo() : m_Symbol("") {}
    };
}; // namespace CurlAPI