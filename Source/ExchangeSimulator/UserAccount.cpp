/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/FileUtils.h"
#include "../LibraryUtils/PathUtils.h"
#include "../LibraryUtils/TimeUtils.h"
#include "../KernelTrading/flatjson.h"
#include "../StaticData/StaticDataManager.h"

#include "UserAccount.h"

#include <algorithm>
#include <cassert>

using namespace ExchangeSimulator;
using namespace tinyxml2;

UserAccount::UserAccount(
    const std::string& userConfigPath,
    const std::string& accountInfoJsonFile)
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
    m_updateTime = TimeUtils::GetEpochTimeTickNow();
    EnableUserAccountControls();
	// init account info from json file
    const std::string accountInfoJsonStr = FileUtils::ReadFileContent(accountInfoJsonFile);
    const flatjson::fjson accountInfoJson{ accountInfoJsonStr.c_str(), accountInfoJsonStr.size() };
    m_accountInfo = binapi::rest::account_info_t::construct(accountInfoJson);
}

std::size_t UserAccount::GetUpdateTime() const
{
    return TimeUtils::GetEpochTimeTickNow();
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

// Order symsol from upstream side is trading pair format
// BTCUSDT not BTC only so we have to parse symbol before lookup
static std::string GetSymbolFromTradingPair(const std::string& input) {
    size_t pos = input.find(StaticDataMgr->GetStableCoinUSDTSymbol()); // Find "USDT" in the string
    return (pos != std::string::npos) ? input.substr(0, pos) : input;
}

AssetBalance& UserAccount::LookupAssetBalance(const AssetSymbol& symbol)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_assetBalances.at(GetSymbolFromTradingPair(symbol)); // Throws std::out_of_range if symbol not found
}

void UserAccount::EnableUserAccountControls()
{
    m_updateTime = TimeUtils::GetEpochTimeTickNow();
    m_canTrade = m_canDeposit = m_canWithdraw = true;
}

void UserAccount::DisableUserAccountControls()
{
    m_updateTime = TimeUtils::GetEpochTimeTickNow();
    m_canTrade = m_canDeposit = m_canWithdraw = false;
}

