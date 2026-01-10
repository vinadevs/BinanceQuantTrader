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
	const XMLElement* mkDataConfigXml)
{
	m_binanceFeedHandler = std::make_unique<BinanceMarketDataFeedHandler>();
	m_binanceDataEvents = std::make_unique<BinanceMarketDataEvents>(mkDataConfigXml, m_binanceFeedHandler.get());
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