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
	: m_mkDataTypeName(mkDataTypeName)
{
	if (m_mkDataTypeName == "RealTimeData")
	{
		m_marketDataFeedHandler = std::make_unique<BinanceMarketDataFeedHandler>();
		const auto* binanceRealTimeMarketDataCfg = mkDataConfigXml->FirstChildElement("RealTimeMarketData");
		assert(binanceRealTimeMarketDataCfg);
		m_marketDataEvents = std::make_unique<BinanceMarketDataEvents>(
			binanceRealTimeMarketDataCfg,
			m_marketDataFeedHandler.get());
	}
	else if (m_mkDataTypeName == "HistoricalMarketData")
	{
		m_marketDataFeedHandler = std::make_unique<HistoricalMarketDataFeedHandler>();
		const auto* historicalMarketDataCfg = mkDataConfigXml->FirstChildElement("HistoricalMarketData");
		assert(historicalMarketDataCfg);
		m_marketDataEvents = std::make_unique<HistoricalMarketDataEvents>(
			historicalMarketDataCfg,
			m_marketDataFeedHandler.get());
	}
	else
	{
		throw std::runtime_error("RealTimeMarketData: unsupported MarketData type");
	}
}

RealTimeMarketData::~RealTimeMarketData() {}

void RealTimeMarketData::RegisterDataListener(MarketDataObserver* observer)
{
	m_marketDataFeedHandler->AttachMarketDataObserver(observer);
}

void RealTimeMarketData::UnRegisterDataListener(MarketDataObserver* observer)
{
	m_marketDataFeedHandler->DettachMarketDataObserver(observer);
}

void RealTimeMarketData::StartStreamingData()
{
	m_marketDataEvents->Wait();
}

void RealTimeMarketData::StartIOContext()
{
	m_marketDataEvents->StartIOContext();
}

bool RealTimeMarketData::SubscribeSymbol(const std::string& symbol)
{
	return m_marketDataEvents->Subscribe(symbol);
}

bool RealTimeMarketData::UnsubscribeSymbol(const std::string& symbol)
{
	return m_marketDataEvents->Unsubscribe(symbol);
}

bool RealTimeMarketData::IsSubscribedSymbol(const std::string& symbol)
{
	return m_marketDataEvents->IsSubscribed(symbol);
}

const std::unordered_set<std::string>& RealTimeMarketData::GetSubscribingSymbols() const
{
	return m_marketDataEvents->GetSubscribingSymbols();
}

MarketDataFeedHanlder* RealTimeMarketData::GetFeedHandler() const
{
	return m_marketDataFeedHandler.get();
}