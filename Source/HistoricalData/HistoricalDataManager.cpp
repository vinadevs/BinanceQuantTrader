/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "../SettingNConfig/tinyxml2.h"

#include "HistoricalDataManager.h"

#include <cassert>

using namespace HistoricalData;
using namespace tinyxml2;

HistoricalDataManager::~HistoricalDataManager() {}

HistoricalDataManager* HistoricalDataManager::GetInstance()
{
    static HistoricalDataManager instance;
    return &instance;
}

void HistoricalDataManager::LoadHistoricalDatabase(const XMLElement* historicalDataConfigXml)
{
    assert(historicalDataConfigXml);
}

MarketDataFileWriter* HistoricalDataManager::GetHistoricalDataWriter(
	const std::filesystem::path& filePath,
	const MarketDataFileWriter::DataSourceType sourceType)
{
	auto it = m_historicalDataWriters.find(filePath.string());
	if (it != m_historicalDataWriters.end()) {
		return it->second.get();
	}
	auto writer = std::make_unique<MarketDataFileWriter>(
		filePath.string(),
		sourceType);
	MarketDataFileWriter* writerPtr = writer.get();
	m_historicalDataWriters[filePath.string()] = std::move(writer);
	return writerPtr;
}
