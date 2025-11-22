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
};

