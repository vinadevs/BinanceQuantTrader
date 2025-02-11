/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

// ----------------------------------------------------------------------------
// Developer: vinadevs
// ----------------------------------------------------------------------------

#pragma once

#ifdef _MSC_VER
#ifdef EXCHANGECONNECTIVITY_EXPORTS
#ifndef DLL_CLASS_EXCHANGECONNECTIVITY_EXPORTS
#define DLL_CLASS_EXCHANGECONNECTIVITY_EXPORTS _declspec(dllexport)
#endif
#else
#ifndef DLL_CLASS_EXCHANGECONNECTIVITY_EXPORTS
#define DLL_CLASS_EXCHANGECONNECTIVITY_EXPORTS _declspec(dllimport)
//#		define DLL_CLASS_EXCHANGECONNECTIVITY_EXPORTS
#endif
#endif  // EXCHANGECONNECTIVITY_EXPORTS
#else
#define DLL_CLASS_EXCHANGECONNECTIVITY_EXPORTS
#endif  // _MSC_VER
