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
#include "../SettingNConfig/tinyxml2.h"

#include "TrendDetector.h"

#include <iostream>
#include <exception>

using namespace IndicatorNSignals;
using namespace MarketData;
using namespace tinyxml2;

TradingTrendDetector::TradingTrendDetector(
    const std::string& targetSymbol,
    const tinyxml2::XMLElement* parametersXml)
    : IndicatorAndSignals(targetSymbol)
{
    InitParameters(parametersXml);
    VerifyParameters();
    m_logger = std::make_unique<LibraryUtils::Logger>("TradingTrendDetector");
}

void TradingTrendDetector::AnalysisIndividualBookTicker(const SynchronousMarketData* data)
{
    UpdateBestBidPrice(data->GetFeed(FeedID::BEST_BID_PRICE)->GetDoubleMultiprecisionData());
    UpdateBestAskPrice(data->GetFeed(FeedID::BEST_ASK_PRICE)->GetDoubleMultiprecisionData());
    UpdateBestBidQuality(data->GetFeed(FeedID::BEST_BID_QUANTY)->GetDoubleMultiprecisionData());
    UpdateBestAskQuality(data->GetFeed(FeedID::BEST_ASK_QUANTY)->GetDoubleMultiprecisionData());
    // update trading hints
    m_tradingHints->isInvertedTrend = IsInvertedTrend();
    m_tradingHints->isUpTrend = IsUpTrend();
    m_tradingHints->isDownTrend = IsDownTrend();
    m_tradingHints->longSuggestionQuantity = 0.0001;
    m_tradingHints->shortSuggestionQuantity = 0.0001;
    const auto qualitySantizied1 = m_tradingHints->longSuggestionQuantity.convert_to<double>();
    const auto qualitySantiziedd1 = std::to_string(qualitySantizied1);
    const auto qualitySantiziedd1s = qualitySantiziedd1.c_str();
    m_tradingHints->windowBestAskPrice = GetWindowBestAskPrice();
    m_tradingHints->windowBestBidPrice = GetWindowBestBidPrice();
    if (this->m_tradingTrendType != TradingTrendType::SETTLE_TREND)
    {
        IndicatorAndSignals::NotifyTradingHintsToListeners();
    }
}

void TradingTrendDetector::InitParameters(const tinyxml2::XMLElement* parametersXml)
{
    assert(parametersXml);
    m_windowSize = parametersXml->UnsignedAttribute("WindowSize");
    m_triggerMaxiumUpTick = parametersXml->UnsignedAttribute("TriggerMaxiumDownTick");
    m_triggerMaxiumDownTick = parametersXml->UnsignedAttribute("TriggerMaxiumUpTick");
}

void TradingTrendDetector::VerifyParameters()
{
    if (m_windowSize <= 0)
    {
        throw std::runtime_error("TradingTrendDetector: Invalid windowSize.");
    }
    else if (m_triggerMaxiumUpTick <= 0)
    {
        throw std::runtime_error("TradingTrendDetector: Invalid triggerMaxiumUpTick.");
    }
    else if (m_triggerMaxiumDownTick <= 0)
    {
        throw std::runtime_error("TradingTrendDetector: Invalid triggerMaxiumDownTick.");
    }
}

void TradingTrendDetector::DetectTradingTrend() 
{
    const auto& latestBesstBidPrice = m_bestBidPrices.back();
    if (latestBesstBidPrice > m_smaPrice)
    {
        m_isInvertedTrend = true;
        m_tickbyTickUpCounter++;
        m_tradingTrendType = TradingTrendType::UP_TREND;
        //m_logger->Info("Symbol[" + GetTargetSymbol() + "] reported uptrend, last best bid price : " + latestBesstBidPrice.str() + ",SMA : " + m_smaPrice.str());
    }
    else if (latestBesstBidPrice < m_smaPrice)
    {
        m_isInvertedTrend = true;
        m_tickbyTickDownCounter++;
        m_tradingTrendType = TradingTrendType::DOWN_TREND;
        //m_logger->Info("Symbol[" + GetTargetSymbol() + "] reported downtrend, last best bid price: " + latestBesstBidPrice.str() + ",SMA: " + m_smaPrice.str());
    }
    else
    {
        m_tradingTrendType = TradingTrendType::SETTLE_TREND;
        m_isInvertedTrend = false;
        m_tickbyTickUpCounter = 0; // reset counter
        m_tickbyTickDownCounter = 0; // reset counter
    }
}

void TradingTrendDetector::UpdateBestBidPrice(const binapi::double_type bidPrice)
{
    if (m_bestBidPrices.size() >= m_windowSize) 
    {
        m_bestBidPrices.pop_front(); // Remove the oldest price
    }
    m_bestBidPrices.emplace_back(bidPrice);
    if (m_bestBidPrices.size() == m_windowSize) 
    {
        CalculateSMA();
        DetectTradingTrend();
    }
}

void TradingTrendDetector::CalculateSMA() 
{
    binapi::double_type sum = 0.0;
    for (const auto& price : m_bestBidPrices) 
    {
        sum += price;
    }
    m_smaPrice = sum / m_bestBidPrices.size();
}

void TradingTrendDetector::UpdateBestAskPrice(const binapi::double_type askPrice)
{
    if (m_bestAskPrices.size() >= m_windowSize)
    {
        m_bestAskPrices.pop_front();  // Remove the oldest price
    }
    m_bestAskPrices.emplace_back(askPrice);
}

void TradingTrendDetector::UpdateBestBidQuality(const binapi::double_type bidQuality)
{
    if (m_bestBidQty.size() >= m_windowSize)
    {
        m_bestBidQty.pop_front();  // Remove the oldest quantity
    }
    m_bestBidQty.emplace_back(bidQuality);
}

void TradingTrendDetector::UpdateBestAskQuality(const binapi::double_type askQuality)
{
    if (m_bestAskQty.size() >= m_windowSize)
    {
        m_bestAskQty.pop_front();  // Remove the oldest quantity
    }
    m_bestAskQty.emplace_back(askQuality);
}

bool TradingTrendDetector::IsInvertedTrend() const
{
    return m_isInvertedTrend
       && m_tradingTrendType != TradingTrendType::SETTLE_TREND;
}

bool TradingTrendDetector::IsUpTrend() const
{
    return m_tickbyTickUpCounter >= m_triggerMaxiumUpTick
        && m_tradingTrendType == TradingTrendType::UP_TREND;
}

bool TradingTrendDetector::IsDownTrend() const
{
    return m_tickbyTickDownCounter >= m_triggerMaxiumDownTick
        && m_tradingTrendType == TradingTrendType::DOWN_TREND;
}

binapi::double_type TradingTrendDetector::GetWindowSMAPrice() const
{
    return m_smaPrice;
}

binapi::double_type TradingTrendDetector::GetWindowBestBidPrice() const
{
    return *std::max_element(m_bestBidPrices.begin(), m_bestBidPrices.end());
}

binapi::double_type TradingTrendDetector::GetWindowBestAskPrice() const
{
    return *std::min_element(m_bestAskPrices.begin(), m_bestAskPrices.end());
}
