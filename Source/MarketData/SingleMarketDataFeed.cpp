/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "SingleMarketDataFeed.h"

using namespace MarketData;

SingleMarketDataFeed::SingleMarketDataFeed() {}

void SingleMarketDataFeed::SetData(const SingleMarketData& newData)
{
    m_data = newData;
    m_mkdataFeedStatus = MarketDataFeedStatus::UPDATED;
}

std::size_t SingleMarketDataFeed::GetUnsignedIntData() const
{
    return std::get<std::size_t>(m_data);
}

bool SingleMarketDataFeed::GetBooleanData() const
{
    return std::get<bool>(m_data);
}

binapi::double_type SingleMarketDataFeed::GetDoubleMultiprecisionData() const
{
    return std::get<binapi::double_type>(m_data);
}

double SingleMarketDataFeed::GetDoubleData() const
{
    return std::get<binapi::double_type>(m_data).convert_to<double>();
}

MarketDataFeedStatus SingleMarketDataFeed::GetDataStatus() const
{
    return m_mkdataFeedStatus;
}