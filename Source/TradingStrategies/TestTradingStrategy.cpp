/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "TestTradingStrategy.h"
#include "../MarketData/RealTimeMarketData.h"

#include <iostream>

using namespace TradingStrategies;
using namespace MarketData;
using namespace ComplianceNRegulatory;
using namespace RiskManagement;

TestTradingStrategy::TestTradingStrategy(
	const std::string& strategyCfgPath,
	MarketData::RealTimeMarketData* marketData,
	UserAccount::BinanceTrader* trader,
	BinanceTradingRules* tradingRules)
	: TradingStrategyBase("TestTradingStrategy", "test market data feed only...",
		strategyCfgPath, marketData, trader, tradingRules)
{
	InitializeParameters(strategyCfgPath);
	marketData->RegisterDataListener(this); // I want receive market data to get fun!
}

TestTradingStrategy::~TestTradingStrategy()
{
	m_marketData->UnRegisterDataListener(this); // I earn enough money, leave the market now!
}

bool TestTradingStrategy::OnIndividualBookTickerChange(MarketDataSubject* marketData, const std::string& symbol)
{
	auto data = marketData->GetSynchronousMarketData(symbol);
	LOG_INFO_STREAM(m_logger, "IndividualBookTickerData: " << data->m_individualBookTickerData);
	return true;
}

