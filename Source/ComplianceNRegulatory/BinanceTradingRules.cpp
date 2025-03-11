/********************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "HardTradingLimits.h"
#include "BinanceTradingRules.h"
#include "BinanceExchangeProfile.h"

#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/PathUtils.h"

#include <cassert>

using namespace ComplianceNRegulatory;
using namespace tinyxml2;

BinanceTradingRules::BinanceTradingRules(const XMLElement* tradingRuleConfigXml)
{
	assert(tradingRuleConfigXml);
    const auto* rulesXml = tradingRuleConfigXml->FirstChildElement("TradingLimits");
    assert(rulesXml);
    m_limits = std::make_unique<HardTradingLimits>(
              rulesXml->UnsignedAttribute("RequestWeightPerMinute"),
              rulesXml->UnsignedAttribute("OrdersPerTenSeconds"),
              rulesXml->UnsignedAttribute("OrdersPerTwentyFourHours"));
    const auto* exchangeProfileXml = tradingRuleConfigXml->FirstChildElement("StaticExchangeProfile");
    assert(exchangeProfileXml);
    std::string exchangeProfileFile(exchangeProfileXml->Attribute("File"));
    PathUtils::ReplaceSubString(exchangeProfileFile, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());
    m_exchangeProfileMgr = std::make_unique<BinanceExchangeProfileMgr>(exchangeProfileFile);
}

BinanceTradingRules::~BinanceTradingRules() {}

void BinanceTradingRules::IncreaseRequestWeightPerMinute()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_requestWeightPerMinuteCounter++;
}

void BinanceTradingRules::IncreaseOrdersPerTenSeconds()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_ordersPerTenSecondsCounter++;
}

void BinanceTradingRules::IncreaseOrdersPerTwentyFourHours()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_ordersPerTwentyFourHoursCounter++;
}

void BinanceTradingRules::ResetRequestWeightPerMinuteCounter()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_requestWeightPerMinuteCounter >= m_limits->m_requestWeightPerMinute)
    {
        m_requestWeightPerMinuteCounter = 0; // reset counter when exceeded hard limit
    }
}

void BinanceTradingRules::ResetOrdersPerTenSecondsCounter()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_ordersPerTenSecondsCounter >= m_limits->m_ordersPerTenSeconds)
    {
        m_ordersPerTenSecondsCounter = 0; // reset counter when exceeded hard limit
    }
}

void BinanceTradingRules::ResetOrdersPerTwentyFourHoursCounter()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_ordersPerTwentyFourHoursCounter >= m_limits->m_ordersPerTwentyFourHours)
    {
        m_ordersPerTwentyFourHoursCounter = 0; // reset counter when exceeded hard limit
    }
}

bool BinanceTradingRules::IsNotExceededRequestWeightPerMinute()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_requestWeightPerMinuteCounter <= m_limits->m_requestWeightPerMinute;
}

bool BinanceTradingRules::IsNotExceededOrdersPerTenSeconds()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_ordersPerTenSecondsCounter <= m_limits->m_ordersPerTenSeconds;
}

bool BinanceTradingRules::IsNotExceededOrdersPerTwentyFourHours()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_ordersPerTwentyFourHoursCounter <= m_limits->m_ordersPerTwentyFourHours;
}
