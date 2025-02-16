/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include "../SettingNConfig/tinyxml2.h"

#include "ExchangeRuleAndCompliance.h"

#include <cassert>
#include <stdexcept>

using namespace ExchangeSimulator;
using namespace tinyxml2;

ExchangeRuleAndCompliance::~ExchangeRuleAndCompliance() {}

ExchangeRuleAndCompliance* ExchangeRuleAndCompliance::GetInstance()
{
    static ExchangeRuleAndCompliance instance;
    return &instance;
}

void ExchangeRuleAndCompliance::SetRuleAndCompliance(const tinyxml2::XMLElement* exchangeRuleConfigXml)
{
    assert(exchangeRuleConfigXml);
    const auto* rulesXml = exchangeRuleConfigXml->FirstChildElement("TradeCommissions");
    assert(rulesXml);
    m_makerCommission = rulesXml->DoubleAttribute("MakerCommission");
    m_takerCommission = rulesXml->DoubleAttribute("TakerCommission");
    m_buyerCommission = rulesXml->DoubleAttribute("BuyerCommission");
    m_sellerCommission = rulesXml->DoubleAttribute("SellerCommission");
    if (m_makerCommission <= 0 || m_takerCommission <= 0)
    {
        throw std::runtime_error("MakerCommission/TakerCommission are invalid.");
    }
}

RateCommission ExchangeRuleAndCompliance::GetMakerCommission() const
{
    return m_makerCommission;
}

RateCommission ExchangeRuleAndCompliance::GetTakerCommission() const
{
    return m_takerCommission;
}

RateCommission ExchangeRuleAndCompliance::GetBuyerCommission() const
{
    return m_buyerCommission;
}

RateCommission ExchangeRuleAndCompliance::GetSellerCommission() const
{
    return m_sellerCommission;
}
