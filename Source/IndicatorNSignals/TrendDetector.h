/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include "../KernelTrading/double_type.h"

#include "IndicatorAndSignals.h"

#include <deque>
#include <string>

namespace MarketData {
    class SynchronousMarketData;
    class MarketDataSubject;
}

namespace tinyxml2 {
    class XMLElement;
};

namespace IndicatorNSignals {

enum class TradingTrendType : unsigned
{
	UP_TREND,
    DOWN_TREND,
	SETTLE_TREND,
};

// This indicator will analyze Individual Book Ticker market data
class DLL_CLASS_INDICATORNSIGNALS_EXPORTS
    TradingTrendDetector : public IndicatorAndSignals
{
public:
    TradingTrendDetector(
        const std::string& targetSymbol,
        const tinyxml2::XMLElement* parametersXml);

    void AnalysisIndividualBookTicker(const MarketData::SynchronousMarketData* data);

    // Check input params
    void InitParameters(const tinyxml2::XMLElement* parametersXml) override;
    void VerifyParameters() override;
private:
    std::deque<binapi::double_type> m_bestBidPrices;
    std::deque<binapi::double_type> m_bestAskPrices;
    std::deque<binapi::double_type> m_bestBidQty;
    std::deque<binapi::double_type> m_bestAskQty;
    size_t m_windowSize{ 0 };
    binapi::double_type m_smaPrice{ 0.0 };
    bool m_isInvertedTrend {false};
    TradingTrendType m_tradingTrendType {TradingTrendType::SETTLE_TREND };
    size_t m_triggerMaxiumUpTick{ 0 };
    size_t m_triggerMaxiumDownTick{ 0 };
    size_t m_tickbyTickUpCounter{ 0 };
    size_t m_tickbyTickDownCounter{ 0 };

    // Simple Moving Average (SMA) 
    void CalculateSMA();
    // Detect curent price trending
    void DetectTradingTrend();

    binapi::double_type GetWindowSMAPrice() const;
    binapi::double_type GetWindowBestBidPrice() const;
    binapi::double_type GetWindowBestAskPrice() const;

    void UpdateBestBidPrice(const binapi::double_type bidPrice);
    void UpdateBestAskPrice(const binapi::double_type askPrice);
    void UpdateBestBidQuality(const binapi::double_type bidQuality);
    void UpdateBestAskQuality(const binapi::double_type askQuality);

    bool IsInvertedTrend() const;
    bool IsUpTrend() const;
    bool IsDownTrend() const;
};
};