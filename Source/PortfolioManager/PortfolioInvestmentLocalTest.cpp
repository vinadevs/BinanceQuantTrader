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

#include "PortfolioInvestmentLocalTest.h"

#include <fstream>
#include <filesystem>
#include <exception>

using namespace PortfolioManager;
using namespace LibraryUtils;
using namespace tinyxml2;

PortfolioInvestmentLocalTest::PortfolioInvestmentLocalTest(const tinyxml2::XMLElement* portfolioCfg)
    : PortfolioInvestment(PortfolioType::LOCAL_TEST)
{
    assert(portfolioCfg);
    const auto* investmentListXml = portfolioCfg->FirstChildElement("InvestmentList");
    assert(investmentListXml);
    const auto* backTestFile = investmentListXml->Attribute("SubscriptionAssetFile");
    assert(backTestFile);
    m_logger = std::make_unique<Logger>("PortfolioInvestmentLocalTest");
    m_logger->Info("Loading test investment data.");
    LoadInvestmentAssets(backTestFile);
}

PortfolioInvestmentLocalTest::~PortfolioInvestmentLocalTest() {}

void PortfolioInvestmentLocalTest::LoadInvestmentAssets(const char* filePath)
{
    std::string assetFilePath(filePath);
    PathUtils::ReplaceSubString(assetFilePath, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());
    if (std::filesystem::exists(assetFilePath))
    {
        std::ifstream file(assetFilePath);
        if (file.is_open()) 
        {
            std::string line;
            while (std::getline(file, line))
            {
                if (line.empty() || line[0] == '#') // ignore empty/comment line
                {
                    continue;
                }
                std::istringstream ss(line);
                std::string tradingPair;
                std::string quantityStr;
                std::string cashBalanceStr;
                std::string originalAssetCashBalanceStr;
                if (std::getline(ss, tradingPair, ',') &&
                    std::getline(ss, quantityStr, ',') &&
                    std::getline(ss, cashBalanceStr, ',') &&
                    std::getline(ss, originalAssetCashBalanceStr))
                {
                    AddInvestmentAsset(tradingPair, 
                        std::stod(quantityStr),
                        std::stod(cashBalanceStr),
                        std::stod(originalAssetCashBalanceStr));

                    m_logger->Info("Added new asset= " + tradingPair);
                }
                else
                {
                    m_logger->Warning("Invalid line, reading failed.");
                }
            }
            file.close();
        }
        else 
        {
            throw std::runtime_error("PortfolioInvestmentLocalTest: error opening file=" + assetFilePath);
        }
    }
    else
    {
        throw std::runtime_error("PortfolioInvestmentLocalTest: file does not exist=" + assetFilePath);
    }
}

bool PortfolioInvestmentLocalTest::IsAlreadyHold(const std::string& symbol)
{
    for (const auto& asset : m_cryptoAssets) 
    {
        if (asset.GetSymbol() == symbol)
        {
            return true;
        }
    }
    return false;
}

const std::vector<CryptoAssetLocalTest>& PortfolioInvestmentLocalTest::GetHoldingAssetList() const
{
    return m_cryptoAssets;
}

bool PortfolioInvestmentLocalTest::UpdateBinanceAssets(const binapi::rest::account_info_t& accountInfo)
{
    return false;
}

const CryptoAssetLocalTest& PortfolioInvestmentLocalTest:: GetCryptoAsset(const std::string& symbol)
{
    const auto it = std::find_if(m_cryptoAssets.begin(),
    m_cryptoAssets.end(), [symbol](const CryptoAssetLocalTest& asset) 
    {
        return asset.GetSymbol() == symbol;
    });
    if (it != m_cryptoAssets.end()) 
    {
        return *it;
    }
    throw std::runtime_error("PortfolioInvestmentLocalTest: we does not hold this asset.");
}

void PortfolioInvestmentLocalTest::AddInvestmentAsset(const std::string& symbol,
											 const binapi::double_type assetQty,
											 const binapi::double_type assetCashBalance,
                                             const binapi::double_type originalAssetCashBalance)
{
    if (!IsAlreadyHold(symbol))
    {
        auto asset = CryptoAssetLocalTest(symbol, assetQty, assetCashBalance);
        asset.SetOriginalAssetCashBalance(originalAssetCashBalance);
        m_cryptoAssets.emplace_back(asset);
    }
    else
    {
        m_logger->Warning("Already hold this asset.");
    }
}

void PortfolioInvestmentLocalTest::UpdateInvestmentAsset(const std::string& symbol,
											    const binapi::double_type newAssetQty,
											    const binapi::double_type newAssetCashBalance)
{
    const auto it = std::find_if(m_cryptoAssets.begin(),
    m_cryptoAssets.end(), [symbol](const CryptoAssetLocalTest& asset)
    {
        return asset.GetSymbol() == symbol;
    });
    if (it != m_cryptoAssets.end())
    {
        it->UpdateAssetQty(newAssetQty);
        it->UpdateAssetCashBalance(newAssetCashBalance);
    }
    else
    {
        throw std::runtime_error("PortfolioInvestmentLocalTest: we does not hold this asset.");
    }
}

void PortfolioInvestmentLocalTest::RemoveInvestmentAsset(const std::string& symbol)
{
    const auto it = std::remove_if(m_cryptoAssets.begin(), m_cryptoAssets.end(), [&symbol](const CryptoAssetLocalTest& asset)
    {
        return asset.GetSymbol() == symbol;
    });
    if (it != m_cryptoAssets.end()) 
    {
        m_cryptoAssets.erase(it, m_cryptoAssets.end());
    }
    else
    {
        throw std::runtime_error("PortfolioInvestmentLocalTest: we does not hold this asset.");
    }
}
