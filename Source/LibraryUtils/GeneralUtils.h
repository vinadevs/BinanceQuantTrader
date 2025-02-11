/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include <windows.h>
#include <string>

namespace GeneralUtils {
    BOOL DLL_CLASS_LIBRARYUTILS_EXPORTS WINAPI ConsoleCtrlHandler(DWORD ctrlType);
    std::string DLL_CLASS_LIBRARYUTILS_EXPORTS GenerateUniqueID(const std::string& instanceName);
};
