/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "RiskManager.h"

#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/Logger.h"

#include <cassert>
#include <cmath>

using namespace RiskManagement;
using namespace LibraryUtils;
using namespace tinyxml2;

RiskManager::RiskManager(const XMLElement* riskManagementConfigXml)
    : m_logger{ std::make_unique<Logger>("RiskManager") }
{
	assert(riskManagementConfigXml);
    const auto* stopLossManagementXml = riskManagementConfigXml->FirstChildElement("StopLossManagement");
    assert(stopLossManagementXml);
    m_riskPerTradePercent = stopLossManagementXml->DoubleAttribute("RiskPerTradePercent");
}

RiskManager::~RiskManager()
{
}

void RiskManager::SetAccountBalance(const double accountBalance)
{
    m_accountBalance = accountBalance;
}

void RiskManager::SetRiskPerTradePercent(const double riskPerTradePercent)
{
    m_riskPerTradePercent = riskPerTradePercent;
}

double RiskManager::GetAccountBalance() const 
{
    return m_accountBalance;
}

double RiskManager::GetRiskPerTradePercent() const 
{
    return m_riskPerTradePercent;
}

double RiskManager::GetMaxPositionSize() const 
{
    return m_maxPositionSize;
}

double RiskManager::CalculatePositionSize(double entryPrice, double stopLoss) const 
{
    if (stopLoss <= 0 || entryPrice <= stopLoss) 
    {
        m_logger->Error("Invalid stop loss value! Must be positive and less than entry price");
        return 0.0;
    }
    const double riskAmount = m_accountBalance * m_riskPerTradePercent;  // Dollar amount we are willing to risk
    const double riskPerUnit = entryPrice - stopLoss;             // Risk per unit of the asset
    double positionSize = riskAmount / riskPerUnit;   
#undef max
#undef min
    // Calculate position size
    positionSize = std::min(positionSize, m_maxPositionSize); // Ensure it does not exceed max position size
    return std::floor(positionSize);  // Return position size rounded down
}
