/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#ifdef _MSC_VER
#ifdef KERNELTRADING_EXPORTS
#ifndef DLL_CLASS
#define DLL_CLASS _declspec(dllexport)
#endif
#else
#ifndef DLL_CLASS
#define DLL_CLASS _declspec(dllimport)
//#		define DLL_CLASS
#endif
#endif  // KERNELTRADING_EXPORTS
#else
#define DLL_CLASS
#endif  // _MSC_VER
