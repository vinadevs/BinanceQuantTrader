/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#ifdef _MSC_VER
#ifdef MIDDLEWAREMQ_EXPORTS
#ifndef DLL_CLASS_MIDDLEWAREMQ_EXPORTS
#define DLL_CLASS_MIDDLEWAREMQ_EXPORTS _declspec(dllexport)
#endif
#else
#ifndef DLL_CLASS_MIDDLEWAREMQ_EXPORTS
#define DLL_CLASS_MIDDLEWAREMQ_EXPORTS _declspec(dllimport)
//#		define DLL_CLASS_MIDDLEWAREMQ_EXPORTS
#endif
#endif  // MIDDLEWAREMQ_EXPORTS
#else
#define DLL_CLASS_MIDDLEWAREMQ_EXPORTS
#endif  // _MSC_VER
