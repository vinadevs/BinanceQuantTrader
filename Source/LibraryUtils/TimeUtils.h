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

    std::string DLL_CLASS_LIBRARYUTILS_EXPORTS GetCurrentTimestamp();
    std::size_t DLL_CLASS_LIBRARYUTILS_EXPORTS GetCurrentTimeChrono(const TimeUnit unit);
};

