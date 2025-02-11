/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

// ----------------------------------------------------------------------------
// Feature updated,bug fixed and codebase ported to windows platform, tested on 
// Visual Studio 2022
// Visual Studio 2019
// Visual Studio 2017
// Developer: vinadevs
// ----------------------------------------------------------------------------

#pragma once

#ifdef _MSC_VER
#ifdef RESTAPI_EXPORTS
#ifndef DLL_CLASS_RESTAPI_EXPORTS
#define DLL_CLASS_RESTAPI_EXPORTS _declspec(dllexport)
#endif
#else
#ifndef DLL_CLASS_RESTAPI_EXPORTS
#define DLL_CLASS_RESTAPI_EXPORTS _declspec(dllimport)
//#		define DLL_CLASS_RESTAPI_EXPORTS
#endif
#endif  // RESTAPI_EXPORTS
#else
#define DLL_CLASS
#endif  // _MSC_VER
