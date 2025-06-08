/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include "../KernelTrading/double_defaults.h"

#include "QuantMarketDataAnalyzer.h"
#include "MarketDataAnalyzer.h"

#include <cmath>

using namespace QuantitativeModel;

MarketDataAnalyzer::MarketDataAnalyzer(
	const std::vector<std::string>& symbolList,
	LibraryUtils::Logger* logger)
	: m_logger(logger)
{
	for (const auto& symbol : symbolList)
	{
		m_quantMarketDataAnalyzers[symbol] = std::make_shared<QuantMarketDataAnalyzer>(symbol);
	}
}

MarketDataAnalyzer::~MarketDataAnalyzer()
{
}

QuantMarketDataAnalyzer* MarketDataAnalyzer::GetQuantMarketDataAnalyzer(const std::string& symbol)
{
	if (const auto it = m_quantMarketDataAnalyzers.find(symbol); it != m_quantMarketDataAnalyzers.end())
	{
		return it->second.get();
	}
	return nullptr;
}
