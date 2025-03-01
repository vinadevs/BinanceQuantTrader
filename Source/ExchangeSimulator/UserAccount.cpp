/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../SettingNConfig/tinyxml2.h"
#include "UserAccount.h"

#include <algorithm>
#include <chrono>
#include <cassert>

using namespace ExchangeSimulator;
using namespace tinyxml2;

UserAccount::UserAccount(const std::string& userConfigPath)
{
    auto userAccountCfgPathXml = std::make_unique<XMLDocument>();
    const auto errorLoadFileXml = userAccountCfgPathXml->LoadFile(userConfigPath.c_str());
    if (errorLoadFileXml != XML_SUCCESS)
    {
        throw std::runtime_error("UserAccount: Load file Xml error="
            + std::string(XMLDocument::ErrorIDToName(errorLoadFileXml)) + ", error path:" + userConfigPath);
    }
    const auto* stableCoinUSDTBalanceXml = userAccountCfgPathXml->FirstChildElement("StableCoinUSDTBalance");
    assert(stableCoinUSDTBalanceXml);
    const auto* usdtAmountXml = stableCoinUSDTBalanceXml->FirstChildElement("UsdtAmount");
    assert(usdtAmountXml);
    m_usdtBalance.m_usdtAmount = usdtAmountXml->DoubleAttribute("Amount");
    const auto* balanceAssetsXml = userAccountCfgPathXml->FirstChildElement("BalanceAssets");
    assert(balanceAssetsXml);
    for (const auto* child = balanceAssetsXml->FirstChildElement();
        child; child = child->NextSiblingElement()) 
    {
        m_assetBalances.try_emplace(
            child->Attribute("Symbol"),
            AssetBalance(child->Attribute("Symbol"), 
                child->DoubleAttribute("FreeAmount"),
                child->DoubleAttribute("LockAmount")));
    }
    m_updateTime = static_cast<std::size_t>(std::chrono::system_clock::now().time_since_epoch().count());
    EnableUserAccountControls();
}

bool UserAccount::IsAccountEligibleToWithdraw() const
{
    return m_canWithdraw;
}

bool UserAccount::IsAccountEligibleToDeposit() const
{
    return m_canDeposit;
}

bool UserAccount::IsAccountEligibleToTrade() const
{
    return m_canTrade;
}

bool UserAccount::IsAccountHavingAssets() const
{
    return std::any_of(m_assetBalances.begin(), m_assetBalances.end(),
        [](const auto& asset) { return asset.second.m_free > 0; });
}

void UserAccount::EnableUserAccountControls()
{
    m_updateTime = static_cast<std::size_t>(std::chrono::system_clock::now().time_since_epoch().count());
    m_canTrade = m_canDeposit = m_canWithdraw = true;
}

void UserAccount::DisableUserAccountControls()
{
    m_updateTime = static_cast<std::size_t>(std::chrono::system_clock::now().time_since_epoch().count());
    m_canTrade = m_canDeposit = m_canWithdraw = false;
}

