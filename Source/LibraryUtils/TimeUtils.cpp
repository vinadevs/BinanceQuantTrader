/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "TimeUtils.h"

#include <chrono>
#include <sstream>
#include <iomanip>

std::string TimeUtils::GetCurrentTimestamp()
{
    const auto now = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
#pragma warning(disable : 4996)
    ss << std::put_time(std::localtime(&time), "[%Y-%m-%d %H:%M:%S]");
    return ss.str();
}

std::size_t TimeUtils::GetCurrentTimeChrono(const TimeUnit unit) {
    // Get the current time as a time point
    const auto now = std::chrono::system_clock::now();
    // Convert to a duration since the epoch
    const auto duration = now.time_since_epoch();
    // Return the requested time unit
    switch (unit) {
    case TimeUnit::Seconds:
        return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    case TimeUnit::Milliseconds:
        return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    case TimeUnit::Nanoseconds:
        return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    default:
        throw std::invalid_argument("Invalid TimeUnit");
    }
}