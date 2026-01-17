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

std::string TimeUtils::GetCurrentTimestampString()
{
    const auto now = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
#pragma warning(disable : 4996)
    ss << std::put_time(std::localtime(&time), "[%Y-%m-%d %H:%M:%S]");
    return ss.str();
}

std::string TimeUtils::GetCurrentTimestampStringPath()
{
    const auto now = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
#pragma warning(disable : 4996)
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d_%H-%M-%S");
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

std::string TimeUtils::GetTimestampString(const std::size_t ms)
{
    // Convert milliseconds to seconds (since std::time_t uses seconds)
    std::time_t timeInSeconds = ms / 1000;
    // Convert to local time
    std::tm localTime{};
#ifdef _WIN32
    localtime_s(&localTime, &timeInSeconds);  // Windows
#else
    localtime_r(&timeInSeconds, &localTime);  // Linux/macOS
#endif
    // Format the time into a string
    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");  // Format: YYYY-MM-DD HH:MM:SS
    return oss.str();
}

std::string TimeUtils::ConvertEpochTickToTimeString(std::size_t epochTick)
{
	if (epochTick <= 0) {
		return "Undefined";
	}

    using clock = std::chrono::system_clock;
    using duration = clock::duration;

    duration d(epochTick);                 // Rebuild duration from count
    auto tp = clock::time_point(d);        // Create time_point from duration
    std::time_t t = clock::to_time_t(tp);  // Convert to time_t

    std::tm* tmPtr = std::localtime(&t);   // Or std::gmtime for UTC

    std::ostringstream oss;
    oss << std::put_time(tmPtr, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}

std::chrono::system_clock::time_point TimeUtils::EpochToTimePoint(
    const long long epochTickValue, const TimeUnit unit)
{
    using clock = std::chrono::system_clock;
    using dur = clock::duration;

    switch (unit) {
    case TimeUnit::Seconds:
        return clock::time_point{
            std::chrono::duration_cast<dur>(std::chrono::seconds{epochTickValue})
        };
    case TimeUnit::Milliseconds:
        return clock::time_point{
            std::chrono::duration_cast<dur>(std::chrono::milliseconds{epochTickValue})
        };
    case TimeUnit::Microseconds:
        return clock::time_point{
            std::chrono::duration_cast<dur>(std::chrono::microseconds{epochTickValue})
        };
    case TimeUnit::Nanoseconds:
        return clock::time_point{
            std::chrono::duration_cast<dur>(std::chrono::nanoseconds{epochTickValue})
        };
    default:
        throw std::invalid_argument("Unsupported TimeUnit");
    }
}

std::size_t TimeUtils::StringDateTimeToEpochSeconds(const std::string& datetime)
{
    std::tm tm{};
    std::istringstream ss(datetime);

    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    if (ss.fail())
        throw std::runtime_error("Invalid datetime format");

    // Interpret as LOCAL time
    std::time_t epoch = std::mktime(&tm);
    if (epoch == -1)
        throw std::runtime_error("mktime failed");

    return static_cast<std::size_t>(epoch);
}