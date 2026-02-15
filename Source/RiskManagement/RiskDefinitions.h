/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"
#include <string>

namespace RiskManagement {

	enum class RISK_ACTION
	{
		NONE = 0,
		REDUCE_EXPOSURE,
		INCREASE_EXPOSURE,
		HALT_TRADING,
	};

	enum class RISK_LEVEL
	{
		LOW = 0,
		MEDIUM,
		HIGH,
		CRITICAL,
	};

	enum class RISK_TYPE
	{
		MARKET_RISK = 0,
		CREDIT_RISK,
		LIQUIDITY_RISK,
		OPERATIONAL_RISK,
	};

	// Structure to hold risk report details
	// This structure encapsulates information about identified risks,
	// including the message, recommended action, risk level, and risk type
	struct DLL_CLASS_RISKMANAGEMENT_EXPORTS RiskReport final
	{
		std::string m_message;
		RISK_ACTION m_recommendedAction{ RISK_ACTION::NONE };
		RISK_LEVEL m_riskLevel{ RISK_LEVEL::LOW };
		RISK_TYPE m_riskType{ RISK_TYPE::MARKET_RISK };
	};

    // Structure to hold the primary option Greeks relevant for risk management in quantitative trading.
    // These Greeks quantify the sensitivity of a portfolio or position to various market factors:
    // - Delta: Measures the sensitivity of the portfolio's value to changes in the price of the underlying asset.
    // - Gamma: Measures the rate of change of delta with respect to changes in the underlying asset's price, indicating convexity.
    // - Theta: Measures the sensitivity of the portfolio's value to the passage of time (time decay).
    // These metrics are essential for assessing and managing risk exposure in options and derivatives trading.
	struct DLL_CLASS_RISKMANAGEMENT_EXPORTS RiskTradingGreeks final
    {
		double m_delta{ 0.0 }; // Sensitivity to underlying price changes (Δ)
		double m_gamma{ 0.0 }; // Sensitivity of delta to underlying price changes (Γ)
		double m_theta{ 0.0 }; // Sensitivity to time decay (Θ)
    };

    // Structure to define trading risk limits for quantitative trading strategies.
    // This class encapsulates key quantitative risk controls:
    // - m_maxPositionSize: The maximum allowable position size (absolute value) to limit exposure to a single asset or strategy.
    // - m_maxDelta: The maximum permitted portfolio delta, restricting directional risk to underlying price movements.
	// - m_maxTheta: The maximum allowed portfolio theta, controlling time decay risk in options strategies.
	// - m_maxGamma: The maximum allowed portfolio gamma, managing convexity risk in options strategies.	
    // - m_maxLoss: The maximum tolerable loss (drawdown) before risk actions are triggered, enforcing capital preservation.
    // - m_minEdge: The minimum required statistical edge (expected value or profit factor) for trade entry, ensuring only favorable trades are taken.
    // - m_maxVolume: The maximum trading volume allowed per period or order, controlling market impact and liquidity risk.
    // These parameters are essential for systematic risk management, helping to prevent excessive losses, concentration, and operational risk in automated trading systems.
    struct DLL_CLASS_RISKMANAGEMENT_EXPORTS RiskTradingLimits final
    {
		double m_maxPositionSize{ 0.0 };
		double m_maxDelta{ 0.0 };
		double m_maxTheta{ 0.0 };
		double m_maxGamma{ 0.0 };
		double m_maxLoss{ 0.0 };
		double m_minEdge{ 0.0 };
		double m_maxVolume{ 0.0 };
    };
};

