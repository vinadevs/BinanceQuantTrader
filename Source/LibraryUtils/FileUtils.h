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

namespace FileUtils {

    std::string DLL_CLASS_LIBRARYUTILS_EXPORTS 
        ReadFileContent(const std::string& file);
};
