/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include <string>

namespace TimeUtils {
    enum class TimeUnit : unsigned
    {
        Seconds,
        Milliseconds,
        Nanoseconds
    };

    std::string DLL_CLASS_LIBRARYUTILS_EXPORTS GetCurrentTimestampString();
    std::size_t DLL_CLASS_LIBRARYUTILS_EXPORTS GetCurrentTimeChrono(const TimeUnit unit);
    std::string DLL_CLASS_LIBRARYUTILS_EXPORTS GetTimestampString(const std::size_t ms);
    std::string DLL_CLASS_LIBRARYUTILS_EXPORTS ConvertEpochTickToTimeString(std::size_t epochTick);
};

