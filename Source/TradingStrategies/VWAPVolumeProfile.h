/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include <unordered_map>
#include <vector>
#include <chrono>
#include <utility> // for std::pair
#include <cstddef> // for size_t
#include "../LibraryUtils/TimeUtils.h"

namespace TradingStrategies {

// VWAPVolumeProfile class calculates the Volume Weighted Average Price (VWAP) profile
class VWAPVolumeProfile {
public:
    VWAPVolumeProfile(int bucketSeconds)
        : m_bucketSeconds(bucketSeconds) {
    }

    void AddNewBucketVolume(const double volume, const long long epochTickValue) {
        size_t bucketId = GetBucketId(epochTickValue);
        m_bucketVolumes[bucketId] += volume;
        m_totalVolume += volume;
    }

    std::vector<std::pair<size_t, double>> GetVolumeProfiles() const {
        std::vector<std::pair<size_t, double>> profile;
        for (auto& kv : m_bucketVolumes) {
            double pct = (m_totalVolume > 0.0) ? (kv.second / m_totalVolume) : 0.0;
            profile.push_back({ kv.first, pct });
        }
        return profile;
    }

private:
    size_t GetBucketId(const long long epochTickValue) const {
		auto epochSec = TimeUtils::EpochToTimePoint(epochTickValue,
            TimeUtils::TimeUnit::Seconds).time_since_epoch().count();
        return epochSec / m_bucketSeconds;
    }

    int m_bucketSeconds{ 0 };
    double m_totalVolume{ 0.0 };
    std::unordered_map<size_t, double> m_bucketVolumes;
};

} // namespace TradingStrategies