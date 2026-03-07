/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"
#include "RiskDefinitions.h"

namespace tinyxml2 {
    class XMLElement;
};

namespace RiskManagement {

    // GreeksCalculator provides basic calculation methods for risk greeks (Delta, Gamma, Theta) 
    // in the context of quantitative trading. It supports:
    // - Spot: Calculates greeks for spot positions, where Delta equals the position size, and Gamma/Theta are zero.
    // - Futures: Calculates greeks for futures positions, where Delta is negative (short exposure), 
    //   Gamma is zero, and Theta is proportional to the carry cost and position size.
    // - Combine: Aggregates two RiskTradingGreeks objects, allowing for portfolio-level risk aggregation.
    // This class is stateless and intended for use in risk management modules to quickly assess 
    // and combine position-level greeks for portfolio risk analysis.
    class DLL_CLASS_RISKMANAGEMENT_EXPORTS GreeksCalculator final
    {
    public:
        static inline RiskTradingGreeks Spot(const double qty) noexcept
        {
            return { qty, 0.0, 0.0 };
        }

        static inline RiskTradingGreeks Futures(const double qty, const double carry) noexcept
        {
            return { -qty, 0.0, carry * qty };
        }

        static inline RiskTradingGreeks Combine(const RiskTradingGreeks& a, const RiskTradingGreeks& b) noexcept
        {
            return
            {
                a.m_delta + b.m_delta,
                a.m_gamma + b.m_gamma,
                a.m_theta + b.m_theta
            };
        }
    };

    // DerivativesRiskModel encapsulates risk management logic for derivatives trading.
    // Responsibilities:
    // - Initializes risk trading limits from an XML configuration (riskManagementConfigXml).
    // - Provides CanTradeNow method to evaluate if a trade can be executed based on:
    //   - RiskTradingGreeks (g): Position greeks (delta, gamma, theta).
    //   - pos: Current position size.
    //   - edge: Expected trading edge (alpha).
    //   - pnl: Current profit and loss.
    //   - vol: Market volatility.
    // - Maintains internal risk limits (m_riskLimits) for decision-making.
    // Usage:
    // - Constructed with XML config for flexible risk parameterization.
    // - Used in risk management modules to enforce trading constraints and prevent excessive risk.
    class DLL_CLASS_RISKMANAGEMENT_EXPORTS DerivativesRiskModel final
    {
    public:
        explicit DerivativesRiskModel(const tinyxml2::XMLElement* riskManagementConfigXml);

        bool CanTradeNow(
            const RiskTradingGreeks& geeks,
            const double pos,
            const double edge,
            const double pnl,
            const double vol) const noexcept;

    private:
    RiskTradingLimits m_riskLimits;
    };
};

