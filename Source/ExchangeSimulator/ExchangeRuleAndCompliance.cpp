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
    m_makerCommission = rulesXml->UnsignedAttribute("MakerCommission");
    m_takerCommission = rulesXml->UnsignedAttribute("TakerCommission");
    m_buyerCommission = rulesXml->UnsignedAttribute("BuyerCommission");
    m_sellerCommission = rulesXml->UnsignedAttribute("SellerCommission");
    if (m_makerCommission <= 0 || m_takerCommission <= 0
        || m_buyerCommission <= 0 || m_sellerCommission <= 0)
    {
        throw std::runtime_error("TradeCommissions are invalid.");
    }
}

std::size_t ExchangeRuleAndCompliance::GetMakerCommission() const
{
    return m_makerCommission;
}

std::size_t ExchangeRuleAndCompliance::GetTakerCommission() const
{
    return m_takerCommission;
}

std::size_t ExchangeRuleAndCompliance::GetBuyerCommission() const
{
    return m_buyerCommission;
}

std::size_t ExchangeRuleAndCompliance::GetSellerCommission() const
{
    return m_sellerCommission;
}
