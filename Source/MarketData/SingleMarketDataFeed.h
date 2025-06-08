/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include "../KernelTrading/double_type.h"
#include "FeedIdentification.h"

#include <variant>
#include <string>
#include <memory>

#pragma warning(disable : 4244)
#pragma warning(disable : 4251)

namespace MarketData {

// Add any data type we want to use as market data
// std::variant is a clearer way than template declaration
using SingleMarketData = std::variant<std::size_t, bool, binapi::double_type, std::string>;

// In trading, Single Market Data Feed refers to a data stream 
// that provides real-time or near-real-time information from 
// a specific data field of an asset class

class DLL_CLASS_MARKETDATA_EXPORTS SingleMarketDataFeed
{
public:
    void SetData(const SingleMarketData& newData);

    std::size_t GetUnsignedIntData() const;
    bool GetBooleanData() const;
    binapi::double_type GetDoubleMultiprecisionData() const;
    double GetDoubleData() const;
    std::string GetStringData() const;
    MarketDataFeedStatus GetDataStatus() const;

	inline friend std::ostream& operator<<(std::ostream& os, const SingleMarketDataFeed& o)
	{
        std::visit([&os](const auto& value) {
            os << value;
            }, o.m_data);
        return os;
	}

	static std::string MarketDataFeedStatusToString(const MarketDataFeedStatus status)
	{
		switch (status)
		{
			case MarketDataFeedStatus::UNDEF: return "UNDEF";
			case MarketDataFeedStatus::INIT: return "INIT";
			case MarketDataFeedStatus::STALED: return "STALED";
			case MarketDataFeedStatus::UPDATED: return "UPDATED";
			default: return "UNKNOWN";
		}
	}
private:
    SingleMarketData m_data;
    MarketDataFeedStatus m_mkdataFeedStatus{ MarketDataFeedStatus::INIT };
};
};

