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
	m_feedHandler = std::make_unique<MarketDataFeedHandler>();
	m_dataEvents = std::make_unique<MarketDataEvents>(mkDataConfigXml, m_feedHandler.get());
}

RealTimeMarketData::~RealTimeMarketData() {}

void RealTimeMarketData::RegisterDataStream(MarketDataObserver* observer)
{
	m_feedHandler->RegisterObserver(observer);
}

void RealTimeMarketData::UnregisterDataStream(MarketDataObserver* observer)
{
	m_feedHandler->UnregisterObserver(observer);
}

void RealTimeMarketData::StartStreamingData()
{
	m_dataEvents->StartSubscriptionEvents();
	m_dataEvents->Wait();
}

const std::unordered_set<std::string>& RealTimeMarketData::GetSubscribingSymbols() const
{
	return m_dataEvents->GetSubscribingSymbols();
}

MarketDataFeedHandler* RealTimeMarketData::GetFeedHandler() const
{
	return m_feedHandler.get();
}