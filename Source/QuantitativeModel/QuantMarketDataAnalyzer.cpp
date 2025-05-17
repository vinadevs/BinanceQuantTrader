/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "../LibraryUtils/Logger.h"
#include "QuantMarketDataAnalyzer.h"

using namespace QuantitativeModel;
using namespace MarketData;
using namespace LibraryUtils;

QuantMarketDataAnalyzer::QuantMarketDataAnalyzer(const std::string& symbol)
	: m_logger(std::make_unique<LibraryUtils::Logger>("QuantMarketDataAnalyzer")),
	  m_symbol(symbol)
{
}

QuantMarketDataAnalyzer::~QuantMarketDataAnalyzer()
{
}

void QuantMarketDataAnalyzer::AnalysisIndividualBookTicker(const MarketData::IndividualBookTickerData& data)
{
	if (m_bestBidPrices.size() >= m_windowSize)
	{
		m_bestBidPrices.pop_front();
	}
	m_bestBidPrices.emplace_back(data.m_bestBidPrice->GetDoubleMultiprecisionData());

	if (m_bestBidPrices.size() == m_windowSize)
	{
		binapi::double_type sum = 0.0;
		for (const auto& price : m_bestBidPrices)
		{
			sum += price;
		}
		m_smaPrice = sum / m_bestBidPrices.size();
		const auto& latestBestBidPrice = m_bestBidPrices.back();
		if (latestBestBidPrice > m_smaPrice)
		{
			m_tickbyTickUpCounter++;
			if (m_tickbyTickUpCounter >= m_triggerMaxiumUpTick)
			{
				m_periodUpCounter++;
				m_tickbyTickUpCounter = 0; // reset counter
				m_lastBestBidPrice = latestBestBidPrice;
				m_logger->Info("Symbol[" + m_symbol + "] reported UP_TREND, last best bid price : " + latestBestBidPrice.str() + ",SMA : " + m_smaPrice.str());
			}
		}
		else if (latestBestBidPrice < m_smaPrice)
		{
			m_tickbyTickDownCounter++;
			if (m_tickbyTickDownCounter >= m_triggerMaxiumDownTick)
			{
				m_periodDownCounter++;
				m_tickbyTickDownCounter = 0; // reset counter
				m_lastBestBidPrice = latestBestBidPrice;
				m_logger->Info("Symbol[" + m_symbol + "] reported DOWN_TREND, last best bid price: " + latestBestBidPrice.str() + ",SMA: " + m_smaPrice.str());
			}
		}
	}
	if (m_periodUpCounter >= m_triggerMaxiumUpPeriod)
	{
		m_periodUpCounter = 0; // reset counter
		m_logger->Info("Symbol[" + m_symbol + "] reported UP_PERIOD, last best bid price : " + m_lastBestBidPrice.str() + ",SMA : " + m_smaPrice.str());
	}
	else if (m_periodDownCounter >= m_triggerMaxiumDownPeriod)
	{
		m_periodDownCounter = 0; // reset counter
		m_logger->Info("Symbol[" + m_symbol + "] reported DOWN_PERIOD, last best bid price: " + m_lastBestBidPrice.str() + ",SMA: " + m_smaPrice.str());
	}

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
