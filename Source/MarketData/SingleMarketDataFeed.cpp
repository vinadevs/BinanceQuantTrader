/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "SingleMarketDataFeed.h"
#include "../LibraryUtils/TimeUtils.h"

#include <type_traits>

using namespace MarketData;

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

std::string SingleMarketDataFeed::GetStringDataFromStdDouble() const
{
    return std::get<std::string>(m_data);
}

std::string SingleMarketDataFeed::GetStringDataFromBoostDouble() const
{
    return std::get<binapi::double_type>(m_data).convert_to<std::string>();
}

std::string SingleMarketDataFeed::GetStringDataFromStdUnsignedIntData() const
{
    return std::to_string(std::get<std::size_t>(m_data));
}

std::string SingleMarketDataFeed::GetStringDataFromStdBoolData() const
{
	return std::get<bool>(m_data) ? "true" : "false";
}

std::string SingleMarketDataFeed::GetStringData()
{
    return std::get<std::string>(m_data);
}

std::string SingleMarketDataFeed::GetStringDataVariant()
{
    return std::visit([](const auto& value) -> std::string {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, std::size_t>) {
            return std::to_string(value);
        }
        else if constexpr (std::is_same_v<T, bool>) {
            return value ? "true" : "false";
        }
        else if constexpr (std::is_same_v<T, binapi::double_type>) {
            return value.template convert_to<std::string>();
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            return value;
        }
        else {
            return "";
        }
        }, m_data);
}

std::string SingleMarketDataFeed::GetStringDataFromEventTimeMs() const
{
	return TimeUtils::GetTimestampString(std::get<std::size_t>(m_data));
}
