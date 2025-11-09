/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#ifdef _MSC_VER
#ifdef ORDERMANAGEMENT_EXPORTS
#ifndef DLL_CLASS_ORDERMANAGEMENT_EXPORTS
#define DLL_CLASS_ORDERMANAGEMENT_EXPORTS _declspec(dllexport)
#endif
#else
#ifndef DLL_CLASS_ORDERMANAGEMENT_EXPORTS
#define DLL_CLASS_ORDERMANAGEMENT_EXPORTS _declspec(dllimport)
//#		define DLL_CLASS_ORDERMANAGEMENT_EXPORTS
#endif
#endif  // ORDERMANAGEMENT_EXPORTS
#else
#define DLL_CLASS_ORDERMANAGEMENT_EXPORTS
#endif  // _MSC_VER