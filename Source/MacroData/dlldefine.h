/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#ifdef _MSC_VER
#ifdef MACRODATA_EXPORTS
#ifndef DLL_CLASS_MACRODATA_EXPORTS
#define DLL_CLASS_MACRODATA_EXPORTS _declspec(dllexport)
#endif
#else
#ifndef DLL_CLASS_MACRODATA_EXPORTS
#define DLL_CLASS_MACRODATA_EXPORTS _declspec(dllimport)
//#		define DLL_CLASS
#endif
#endif  // MACRODATA_EXPORTS
#else
#define DLL_CLASS_MACRODATA_EXPORTS
#endif  // _MSC_VER
