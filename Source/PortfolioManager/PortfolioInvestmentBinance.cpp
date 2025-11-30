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
using namespace KernelTrading;

static constexpr double ZERO_DOUBLE_VALUE = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool BinanceTradingPairManager::CreateNewTradingPair(
    const std::string& tradingPair,
    RealTimeMarketData* marketData,
    const BinanceBalance& balance)
{
    std::unique_lock<std::mutex> lock(m_threadSafeMutex);
    return m_assets.try_emplace(tradingPair,
        std::make_unique<BinanceTradingPair>(tradingPair, marketData, balance)).second;
}

bool BinanceTradingPairManager::RemoveTradingPair(const std::string& tradingPair)
{
    std::unique_lock<std::mutex> lock(m_threadSafeMutex);
    if (const auto it = m_assets.find(tradingPair); it != m_assets.end())
    {
        m_assets.erase(it);
        return true;
    }
    return false;
}

BinanceTradingPair* BinanceTradingPairManager::GetTradingPair(const std::string& tradingPair)
{
    std::unique_lock<std::mutex> lock(m_threadSafeMutex);
    if (const auto it = m_assets.find(tradingPair); it != m_assets.end())
    {
        return it->second.get();
    }
}

const BinanceTradingPairMap& BinanceTradingPairManager::GetTradingPairs() const
{
    return m_assets;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PortfolioInvestmentBinance::PortfolioInvestmentBinance(
    const XMLElement* portfolioCfg, RealTimeMarketData* marketData)
	: PortfolioInvestment(PortfolioType::BINANCE_ASSET) , m_marketData(marketData)
{
    m_logger = std::make_unique<LibraryUtils::Logger>("PortfolioInvestmentBinance");
    assert(portfolioCfg);
}

PortfolioInvestmentBinance::~PortfolioInvestmentBinance() {}

void PortfolioInvestmentBinance::SetUserSpotAccountInfo(binapi::rest::account_info_t* account)
{
    assert(account);
    m_binanceSpotAccountInfo = account;
}

void PortfolioInvestmentBinance::SetUserFutureAccountInfo(KernelTrading::UserFutureAccount* account)
{
	assert(account);
	m_binanceFutureAccountInfo = account;
}

bool PortfolioInvestmentBinance::IsCryptoAssetHasMarketData(const std::string& asset)
{
    assert(m_marketData);
    const auto tradingPair = CreateTradingPairSymbol(asset);
    return m_marketData->IsSubscribedSymbol(tradingPair);
}

void PortfolioInvestmentBinance::UpdateBinanceAccountInfo()
{
    if (BinanceAccountUtils::QueryBinanceSpotAccount(m_binanceSpotAccountInfo, m_logger.get()))
    {
        m_logger->Info("updated Binance spot account information to portfolio manager.");
        UpdateBinanceTradingPairs();
    }
    else
    {
        // If you are seeing the error:
        // APIError(code=-2015): Invalid API-key, IP, or permissions for action from the python-binance module
        // This maybe because your API-key has been expired
        // Please try to renew your API-key at https://www.binance.com/en-JP/my/settings/api-management 
        // OR: Way too much request weight used; IP banned until specific time

		// OR if you are using simulator then you need to check if the simulator is running
		// to retrieve the simulation account information
		m_logger->Warning("failed to update Binance account information.");
    }
}

void PortfolioInvestmentBinance::UpdateBinanceFutureAccountInfo()
{
    if (BinanceAccountUtils::QueryBinanceFutureAccount(m_binanceFutureAccountInfo, m_logger.get()))
    {
        m_logger->Info("updated Binance future account information to portfolio manager.");
    }
    else
    {
        // If you are seeing the error:
        // APIError(code=-2015): Invalid API-key, IP, or permissions for action from the python-binance module
        // This maybe because your API-key has been expired
        // Please try to renew your API-key at https://www.binance.com/en-JP/my/settings/api-management 
        // OR: Way too much request weight used; IP banned until specific time

        // OR if you are using simulator then you need to check if the simulator is running
        // to retrieve the simulation account information
		m_logger->Warning("failed to update Binance future account information.");
    }
}

void PortfolioInvestmentBinance::UpdateBinanceTradingPairs()
{
    for (const auto& balance : m_binanceSpotAccountInfo->balances)
    {
        const auto binanceSymbol = CreateTradingPairSymbol(balance.first);
        if (IsCryptoAssetAbleToTrade(balance.second))
        {
            if (BinanceTradingPair* tradingPair 
                = m_binanceTradingPairMgr.GetTradingPair(binanceSymbol))
            {
                tradingPair->UpdateTradingPair(balance.second);
            }
            else
            {
                m_logger->Info("Could not update binance asset=" 
                    + binanceSymbol + ", we do NOT manage this asset now.");
            }
        }
        else
        {
            m_logger->Warning("Could not update binance asset=" 
                + binanceSymbol + ", there is no asset balance or martket data available.");
        }
    }
}

void PortfolioInvestmentBinance::AddNewAssetToManage(const std::string& asset)
{
    if (m_binanceTradingPairMgr.CreateNewTradingPair(asset, m_marketData, BinanceBalance()))
    {
        m_logger->Info("Added new binance trading pair=" + asset);
    }
    else
    {
        m_logger->Info("Could not add new binance asset=" + asset + ".");
    }
}

const binapi::rest::account_info_t* PortfolioInvestmentBinance::GetBinanceAccountInfo() const
{
    return m_binanceSpotAccountInfo;
}

BinanceTradingPairManager& PortfolioInvestmentBinance::GetBinanceTradingPairManager(
    bool updateNewData /*= false*/)
{
    if (updateNewData)
    {
        UpdateBinanceAccountInfo();
    }
    return m_binanceTradingPairMgr;
}

BinanceTradingPair* PortfolioInvestmentBinance::GetBinanceTradingPair(
    const std::string& asset,
    bool updateNewData /*= false*/)
{
    if (updateNewData)
    {
        UpdateBinanceAccountInfo();
    }
    return m_binanceTradingPairMgr.GetTradingPair(asset);
}

const PositionInfo& PortfolioInvestmentBinance::GetBinanceFuturePositionInfo(
    const std::string& asset,
    bool updateNewData  /*= false*/)
{
	if (updateNewData)
	{
		UpdateBinanceFutureAccountInfo();
	}
	if (m_binanceFutureAccountInfo)
	{
		const auto& positions = m_binanceFutureAccountInfo->GetPositions();
		for (const auto& position : positions)
		{
			if (position.symbol == asset)
			{
				return position;
			}
		}
	}
	throw std::runtime_error("PortfolioInvestmentBinance: we are not holding this asset/symbol=" + asset);
}

bool PortfolioInvestmentBinance::IsCryptoAssetAbleToTrade(const BinanceBalance& balance)
{
    return !balance.asset.empty() && IsCryptoAssetHasMarketData(balance.asset);
}

bool PortfolioInvestmentBinance::HasCryptoAssetBalance(const BinanceBalance& balance)
{
    return !balance.asset.empty() &&
        balance.free > ZERO_DOUBLE_VALUE &&
        IsCryptoAssetHasMarketData(balance.asset);
}

const BinanceBalances& PortfolioInvestmentBinance::GetAllBinanceBalances(bool updateNewData /*= false*/)
{
    if (updateNewData)
    {
        UpdateBinanceAccountInfo();
    }
    return m_binanceSpotAccountInfo->balances;
}

BinanceBalances PortfolioInvestmentBinance::GetTradableBinanceBalances(bool updateNewData /*= false*/)
{
    if (updateNewData)
    {
        UpdateBinanceAccountInfo();
    }
    BinanceBalances tradableBalances;
    for (const auto& balance : m_binanceSpotAccountInfo->balances)
    {
        if (IsCryptoAssetAbleToTrade(balance.second))
        {
            tradableBalances.try_emplace(balance.first, balance.second);
        }
        else
        {
            m_logger->Info("Could not trade binance asset=" 
                + balance.first + ", there is no asset balance or martket data available.");
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
    for (const auto& balance : m_binanceSpotAccountInfo->balances)
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