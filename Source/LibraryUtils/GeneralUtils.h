/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#ifdef _WIN32
#include <windows.h>
#endif
#include <string>

namespace GeneralUtils {
#ifdef _WIN32
    BOOL DLL_CLASS_LIBRARYUTILS_EXPORTS WINAPI ConsoleCtrlHandler(DWORD ctrlType);
#else
    // Non-Windows platforms can have their own signal handlers if needed
#endif
    std::string DLL_CLASS_LIBRARYUTILS_EXPORTS GenerateUniqueID(const std::string& instanceName);
};
