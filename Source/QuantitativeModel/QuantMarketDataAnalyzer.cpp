/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "QuantMarketDataAnalyzer.h"

using namespace QuantitativeModel;
using namespace MarketData;
using namespace LibraryUtils;

QuantMarketDataAnalyzer::QuantMarketDataAnalyzer(const std::string& symbol)
	: m_symbol(symbol)
{
}

QuantMarketDataAnalyzer::~QuantMarketDataAnalyzer()
{
}

void QuantMarketDataAnalyzer::AnalysisIndividualBookTicker(const MarketData::IndividualBookTickerData& data)
{
}

void QuantMarketDataAnalyzer::AnalysisTrade(const MarketData::TradeData& data)
{
}

void QuantMarketDataAnalyzer::AnalysisIndividualMarketTicker(const MarketData::IndividualMarketTickerData& data)
{
}

void QuantMarketDataAnalyzer::AnalysisMiniTicker(const MarketData::IndividualMiniTickerData& data)
{
}

void QuantMarketDataAnalyzer::AnalysisAggregateTrade(const MarketData::AggregateTradeData& data)
{
}

void QuantMarketDataAnalyzer::AnalysisKlineCandleStick(const MarketData::KlineCandleStickData& data)
{
}

void QuantMarketDataAnalyzer::AnalysisAllMarketTickers(const MarketData::AllMarketTickerData& data)
{
}

void QuantMarketDataAnalyzer::AnalysisAllMiniTickers(const MarketData::AllMiniTickerData& data)
{
}

void QuantMarketDataAnalyzer::AnalysisAllBookTickers(const MarketData::AllMarketTickerData& data)
{
}

void QuantMarketDataAnalyzer::AnalysisAllMarketDepth(const MarketData::AllPartDepthData& data)
{
}

void QuantMarketDataAnalyzer::AnalysisAllMarketDepthDiff(const MarketData::AllDiffDepthData& data)
{
}
