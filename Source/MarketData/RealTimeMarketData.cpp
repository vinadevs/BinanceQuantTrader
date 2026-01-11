/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../SettingNConfig/tinyxml2.h"

#include "RealTimeMarketData.h"

using namespace MarketData;
using namespace tinyxml2;

RealTimeMarketData::RealTimeMarketData(
	const XMLDocument* mkDataConfigXml,
	const std::string& mkDataTypeName)
{
	if (mkDataTypeName == "RealTimeData")
	{
		m_binanceFeedHandler = std::make_unique<BinanceMarketDataFeedHandler>();
		const auto* binanceRealTimeMarketDataCfg = mkDataConfigXml->FirstChildElement("RealTimeMarketData");
		assert(binanceRealTimeMarketDataCfg);
		m_binanceDataEvents = std::make_unique<BinanceMarketDataEvents>(
			binanceRealTimeMarketDataCfg,
			m_binanceFeedHandler.get());
	}
	else if (mkDataTypeName == "HistoricalMarketData")
	{
		m_historicalFeedHandler = std::make_unique<HistoricalMarketDataFeedHandler>();
		const auto* historicalMarketDataCfg = mkDataConfigXml->FirstChildElement("HistoricalMarketData");
		assert(historicalMarketDataCfg);
		m_historicalDataEvents = std::make_unique<HistoricalMarketDataEvents>(
			historicalMarketDataCfg,
			m_historicalFeedHandler.get());
	}
	else
	{
		throw std::runtime_error("RealTimeMarketData: unsupported MarketData type");
	}
}

RealTimeMarketData::~RealTimeMarketData() {}

void RealTimeMarketData::RegisterDataListener(MarketDataObserver* observer)
{
	m_binanceFeedHandler->RegisterObserver(observer);
}

void RealTimeMarketData::UnRegisterDataListener(MarketDataObserver* observer)
{
	m_binanceFeedHandler->UnregisterObserver(observer);
}

void RealTimeMarketData::StartStreamingData()
{
	m_binanceDataEvents->Wait();
}

void RealTimeMarketData::StartIOContext()
{
	m_binanceDataEvents->StartIOContext();
}

bool RealTimeMarketData::SubscribeSymbol(const std::string& symbol)
{
	return m_binanceDataEvents->Subscribe(symbol);
}

bool RealTimeMarketData::UnsubscribeSymbol(const std::string& symbol)
{
	return m_binanceDataEvents->Unsubscribe(symbol);
}

bool RealTimeMarketData::IsSubscribedSymbol(const std::string& symbol)
{
	return m_binanceDataEvents->IsSubscribed(symbol);
}

const std::unordered_set<std::string>& RealTimeMarketData::GetSubscribingSymbols() const
{
	return m_binanceDataEvents->GetSubscribingSymbols();
}

BinanceMarketDataFeedHandler* RealTimeMarketData::GetFeedHandler() const
{
	return m_binanceFeedHandler.get();
}