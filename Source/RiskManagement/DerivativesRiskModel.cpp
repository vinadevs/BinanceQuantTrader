/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include "../SettingNConfig/tinyxml2.h"

#include "DerivativesRiskModel.h"
#include <cassert>

using namespace RiskManagement;

DerivativesRiskModel::DerivativesRiskModel(const tinyxml2::XMLElement* riskManagementConfigXml)
{
	assert(riskManagementConfigXml);
	// Load risk limits from XML configuration
	m_riskLimits.m_maxDelta = riskManagementConfigXml->DoubleAttribute("MaxDelta");
	m_riskLimits.m_maxGamma = riskManagementConfigXml->DoubleAttribute("MaxGamma");
	m_riskLimits.m_maxTheta = riskManagementConfigXml->DoubleAttribute("MaxTheta");
	m_riskLimits.m_maxLoss = riskManagementConfigXml->DoubleAttribute("MaxLoss");
	m_riskLimits.m_maxPositionSize = riskManagementConfigXml->DoubleAttribute("MaxPositionSize");
	m_riskLimits.m_minEdge = riskManagementConfigXml->DoubleAttribute("MaxEdge");
	m_riskLimits.m_maxVolume = riskManagementConfigXml->DoubleAttribute("MaxVolume");
}

bool DerivativesRiskModel::CanTradeNow(
	const RiskTradingGreeks& geeks,
	const double pos, 
	const double edge,
	const double pnl,
	const double vol) const noexcept
{
	if (std::abs(pos) > m_riskLimits.m_maxPositionSize) return false;
	if (std::abs(geeks.m_delta) > m_riskLimits.m_maxDelta) return false;
	if (pnl < -m_riskLimits.m_maxLoss) return false;
	if (edge < m_riskLimits.m_minEdge) return false;
	if (vol > m_riskLimits.m_maxVolume) return false;

	return true;
}
