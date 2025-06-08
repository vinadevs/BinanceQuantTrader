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

namespace PathUtils {
    static const std::string RootBQTPath = "[ROOT_BQT_PATH]";

    enum class Path_Type : unsigned
    {
        ROOT,
        COMMON,
        STRATEGY,
        BQT,
        SIMULATOR,
        MESSAGE_SERVER,
        MARKET_DATA_CAPTURE,
    };

    std::string DLL_CLASS_LIBRARYUTILS_EXPORTS GetApplicationFolderPath();
    std::string DLL_CLASS_LIBRARYUTILS_EXPORTS GetConfigFolderPath(const Path_Type type);
    void DLL_CLASS_LIBRARYUTILS_EXPORTS ReplaceSubString(
        std::string& originalString,
        const std::string& targetString,
        const std::string& replaceBylString);
};
