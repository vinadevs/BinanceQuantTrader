/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/PathUtils.h"
#include "../MarketData/RealTimeMarketData.h"
#include "../StaticData/StaticDataManager.h"
#include "../LibraryUtils/SourceBuildFlags.h"

#include "PortfolioInvestmentBinance.h"
#include "BinanceAccountUtils.h"

#include <fstream>
#include <filesystem>
#include <exception>

using namespace PortfolioManager;
using namespace LibraryUtils;
using namespace tinyxml2;
using namespace MarketData;
using namespace StaticData;

static constexpr double ZERO_DOUBLE_VALUE = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool BinanceTradingPairManager::CreateNewTradingPair(const std::string& tradingPairPair, const RealTimeMarketData* marketData, const BinanceBalance& balance)
{
    std::unique_lock<std::mutex> lock(m_threadSafeMutex);
    return m_assets.try_emplace(tradingPairPair, std::make_unique<BinanceTradingPair>(tradingPairPair, marketData, balance)).second;
}

bool BinanceTradingPairManager::RemoveTradingPair(const std::string& tradingPairPair)
{
    std::unique_lock<std::mutex> lock(m_threadSafeMutex);
    if (const auto it = m_assets.find(tradingPairPair); it != m_assets.end())
    {
        m_assets.erase(it);
        return true;
    }
    return false;
}

BinanceTradingPair* BinanceTradingPairManager::GetTradingPair(const std::string& tradingPairPair)
{
    std::unique_lock<std::mutex> lock(m_threadSafeMutex);
    if (const auto it = m_assets.find(tradingPairPair); it != m_assets.end())
    {
        return it->second.get();
    }
}

const BinanceTradingPairMap& BinanceTradingPairManager::GetTradingPairs() const
{
    return m_assets;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PortfolioInvestmentBinance::PortfolioInvestmentBinance(const XMLElement* portfolioCfg, const RealTimeMarketData* marketData)
	: PortfolioInvestment(PortfolioType::BINANCE_ASSET) , m_marketData(marketData)
{
    m_logger = std::make_unique<LibraryUtils::Logger>("PortfolioInvestmentBinance");
    assert(portfolioCfg);
    const auto* investmentListXml = portfolioCfg->FirstChildElement("InvestmentManagement");
    assert(investmentListXml);
}

PortfolioInvestmentBinance::~PortfolioInvestmentBinance() {}

void PortfolioInvestmentBinance::SetUserAccountInfo(binapi::rest::account_info_t* account)
{
    assert(account);
    m_binanceAccountInfo = account;
}

bool PortfolioInvestmentBinance::IsCryptoAssetHasMarketData(const std::string& asset) const
{
    assert(m_marketData);
    const auto tradingPairPair = CreateTradingPairSymbol(asset);
    return m_marketData->GetSubscribingSymbols().find(tradingPairPair) 
        != m_marketData->GetSubscribingSymbols().end();
}

void PortfolioInvestmentBinance::UpdateBinanceAccountInfo()
{
    if (BinanceAccountUtils::QueryBinanceAccount(m_binanceAccountInfo, m_logger.get()))
    {
        m_logger->Info("updated Binance account information to portfolio manager.");
        UpdateBinanceTradingPairs();
    }
    else
    {
        // If you are seeing the error:
        // APIError(code=-2015): Invalid API-key, IP, or permissions for action from the python-binance module
        // This maybe because your API-key has been expired
        // Please try to renew your API-key at https://www.binance.com/en-JP/my/settings/api-management 
        throw std::runtime_error("PortfolioInvestmentBinance: failed to update Binance account information.");
    }
}

void PortfolioInvestmentBinance::UpdateBinanceTradingPairs()
{
    for (const auto& balance : m_binanceAccountInfo->balances)
    {
        if (IsCryptoAssetAbleToTrade(balance.second))
        {
            const auto tradingPairPair = CreateTradingPairSymbol(balance.first);
            BinanceTradingPair* tradingPair{ nullptr };
            tradingPair = m_binanceTradingPairMgr.GetTradingPair(tradingPairPair);
            if (tradingPair)
            {
                tradingPair->UpdateTradingPair(balance.second);
            }
            else if (m_binanceTradingPairMgr.CreateNewTradingPair(tradingPairPair, m_marketData, balance.second))
            {
                m_logger->Info("Created new binance trading pair=" + tradingPairPair);
            }
        }
        else
        {
            m_logger->Info("Could not trade binance asset=" + balance.first + ", there is no asset balance or martket data available.");
        }
    }
}

binapi::rest::account_info_t* PortfolioInvestmentBinance::GetBinanceAccountInfo()
{
    return m_binanceAccountInfo;
}

BinanceTradingPairManager& PortfolioInvestmentBinance::GetBinanceTradingPairManager(bool updateNewData /*= false*/)
{
    if (updateNewData)
    {
        UpdateBinanceAccountInfo();
    }
    return m_binanceTradingPairMgr;
}

BinanceTradingPair* PortfolioInvestmentBinance::GetBinanceTradingPair(const std::string& asset, bool updateNewData /*= false*/)
{
    if (updateNewData)
    {
        UpdateBinanceAccountInfo();
    }
    return m_binanceTradingPairMgr.GetTradingPair(asset);
}

bool PortfolioInvestmentBinance::IsCryptoAssetAbleToTrade(const BinanceBalance& balance) const
{
    return !balance.asset.empty() && IsCryptoAssetHasMarketData(balance.asset);
}

bool PortfolioInvestmentBinance::HasCryptoAssetBalance(const BinanceBalance& balance) const
{
    return !balance.asset.empty() && balance.free > ZERO_DOUBLE_VALUE && IsCryptoAssetHasMarketData(balance.asset);
}

const BinanceBalances& PortfolioInvestmentBinance::GetAllBinanceBalances(bool updateNewData /*= false*/)
{
    if (updateNewData)
    {
        UpdateBinanceAccountInfo();
    }
    return m_binanceAccountInfo->balances;
}

BinanceBalances PortfolioInvestmentBinance::GetTradableBinanceBalances(bool updateNewData /*= false*/)
{
    if (updateNewData)
    {
        UpdateBinanceAccountInfo();
    }
    BinanceBalances tradableBalances;
    for (const auto& balance : m_binanceAccountInfo->balances)
    {
        if (IsCryptoAssetAbleToTrade(balance.second))
        {
            tradableBalances.try_emplace(balance.first, balance.second);
        }
        else
        {
            m_logger->Info("Could not trade binance asset=" + balance.first + ", there is no asset balance or martket data available.");
        }
    }
    return tradableBalances;
}

const BinanceBalance& PortfolioInvestmentBinance::GetBinanceBalance(const std::string& asset, bool updateNewData /*= false*/)
{
    if (updateNewData)
    {
        UpdateBinanceAccountInfo();
    }
    for (const auto& balance : m_binanceAccountInfo->balances)
    {
        if (balance.first == asset)
        {
            return balance.second;
        }
    }
    throw std::runtime_error("PortfolioInvestmentBinance: we are not holding this asset/symbol=" + asset);
}

std::string PortfolioInvestmentBinance::CreateTradingPairSymbol(const std::string& tartgetSymbol)
{
    return tartgetSymbol + StaticDataMgr->GetStableCoinUSDTSymbol();
}