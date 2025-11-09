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
#include <vector>

namespace FileUtils {

    std::string DLL_CLASS_LIBRARYUTILS_EXPORTS 
        ReadFileContent(const std::string& file);
    void DLL_CLASS_LIBRARYUTILS_EXPORTS 
        FromVectorStringToFile(const std::vector<std::string>& vectorString, const std::string& ouputFile);
    std::vector<std::string> DLL_CLASS_LIBRARYUTILS_EXPORTS 
        ReadFileContentToLines(
        const std::string& file,
        const bool trimLine);
};
