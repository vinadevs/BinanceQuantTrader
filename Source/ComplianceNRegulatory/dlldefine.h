/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#ifdef _MSC_VER
#ifdef COMPLIANCENREGULATORY_EXPORTS
#ifndef DLL_CLASS_COMPLIANCENREGULATORY_EXPORTS
#define DLL_CLASS_COMPLIANCENREGULATORY_EXPORTS _declspec(dllexport)
#endif
#else
#ifndef DLL_CLASS_COMPLIANCENREGULATORY_EXPORTS
#define DLL_CLASS_COMPLIANCENREGULATORY_EXPORTS _declspec(dllimport)
//#		define DLL_CLASS_COMPLIANCENREGULATORY_EXPORTS
#endif
#endif  // COMPLIANCENREGULATORY_EXPORTS
#else
#define DLL_CLASS
#endif  // _MSC_VER
