/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../MarketData/MarketDataSubject.h"
#include "../MarketData/SynchronousMarketData.h"
#include "../PortfolioManager/PortfolioInvestmentBinance.h"
#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/Logger.h"

#include "TrendDetector.h"
#include "TradingSignalService.h"

using namespace IndicatorNSignals;
using namespace PortfolioManager;
using namespace MarketData;
using namespace tinyxml2;

////////////////////////////////////////////////////////////////////////

IndicatorAndSignalManager::IndicatorAndSignalManager() {}

IndicatorAndSignalManager::~IndicatorAndSignalManager() {}

void IndicatorAndSignalManager::RegisterTradingHintsListener(TradingHintsListener* listener)
{
    for (auto& signal : m_trendingSignalAnalyzers)
    {
        signal.second->RegisterTradingHintsListener(listener);
    }
}

void IndicatorAndSignalManager::UnregisterTradingHintsListener(TradingHintsListener* listener)
{
    for (auto& signal : m_trendingSignalAnalyzers)
    {
        signal.second->UnregisterTradingHintsListener(listener);
    }
}

bool IndicatorAndSignalManager::CreateNewTrendingAnalyzer(const std::string& symbol, const XMLElement* parametersXml)
{
    return m_trendingSignalAnalyzers.try_emplace(symbol, std::make_unique<TradingTrendDetector>(symbol, parametersXml)).second;
}

bool IndicatorAndSignalManager::RemoveTrendingAnalyzer(const std::string& symbol)
{
    if (const auto it = m_trendingSignalAnalyzers.find(symbol); it != m_trendingSignalAnalyzers.end())
    {
        m_trendingSignalAnalyzers.erase(it);
        return true;
    }
    return false;
}

TradingTrendDetector* IndicatorAndSignalManager::GetTrendindAnalyzer(const std::string& symbol)
{
    if (const auto it = m_trendingSignalAnalyzers.find(symbol); it != m_trendingSignalAnalyzers.end())
    {
        return it->second.get();
    }
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////

TradingSignalService::TradingSignalService(PortfolioInvestmentBinance* portfolio, const tinyxml2::XMLElement* signalXml)
    : m_signalMgr{ std::make_unique<IndicatorAndSignalManager>() },
      m_logger{ std::make_unique<LibraryUtils::Logger>("TradingSignalService") }
{
    const XMLElement* trendDetectorParametersXml = signalXml->FirstChildElement("TrendDetectorParameters");
    assert(trendDetectorParametersXml);
    m_logger->Info("Creating trending services.");
    for (const auto& pairs : portfolio->GetBinanceTradingPairManager().GetTradingPairs())
    {
        m_signalMgr->CreateNewTrendingAnalyzer(pairs.first, trendDetectorParametersXml);
        m_logger->Info("Created trending analyzer for symbol=" + pairs.first);
    }
}

bool TradingSignalService::OnIndividualBookTickerChange(
    MarketDataSubject* marketData, const std::string& symbol)
{
    if (const auto* data = marketData->GetSynchronousMarketData(symbol))
    {
        if (auto* analyzer = m_signalMgr->GetTrendindAnalyzer(symbol))
        {
            analyzer->AnalysisIndividualBookTicker(data);
            return true;
        }
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}

void TradingSignalService::RegisterTradingHintsListener(TradingHintsListener* listener)
{
    m_signalMgr->RegisterTradingHintsListener(listener);
}

void TradingSignalService::UnregisterTradingHintsListener(TradingHintsListener* listener)
{
    m_signalMgr->UnregisterTradingHintsListener(listener);
}