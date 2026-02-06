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
#include <utility>
#include <cstddef>

#include "../LibraryUtils/TimeUtils.h"

namespace TradingStrategies 
{

// VWAPVolumeProfile class calculates the Volume Weighted Average Price (VWAP) profile
// m_bucketVolumes can be added with new volume data for specific time buckets
// or config from external file
class VWAPVolumeProfile 
{
public:
    VWAPVolumeProfile(int bucketMs)
        : m_bucketMs(bucketMs) {}

    void AddNewBucketVolume(const double volume, const long long epochTickValue) 
    {
        size_t bucketId = GetBucketId(epochTickValue);
        m_bucketVolumes[bucketId] += volume;
        m_totalVolume += volume;
    }

    std::vector<std::pair<size_t, double>> GetVolumeProfiles() const 
    {
        std::vector<std::pair<size_t, double>> profile;
        profile.reserve(m_bucketVolumes.size());
        for (const auto& [id, vol] : m_bucketVolumes)
        {
            const double pct = (m_totalVolume > 0.0) ? (vol / m_totalVolume) : 0.0;
            profile.emplace_back(id, pct);
        }
        return profile;
    }

    bool HasBucketVolumnes() const 
    {
        return !m_bucketVolumes.empty();
    }
private:
    size_t GetBucketId(const long long epochTickValue) const 
    {
        auto epochSec = TimeUtils::EpochToTimePoint(epochTickValue,
            TimeUtils::TimeUnit::Milliseconds).time_since_epoch().count();
        return epochSec / m_bucketMs;
    }

    int m_bucketMs{ 0 };
    double m_totalVolume{ 0.0 };
    std::unordered_map<size_t, double> m_bucketVolumes;
};

} // namespace TradingStrategies