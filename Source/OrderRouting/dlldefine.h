/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#ifdef _MSC_VER
#ifdef ORDERROUTING_EXPORTS
#ifndef DLL_CLASS_ORDER_ROUTING_EXPORTS
#define DLL_CLASS_ORDER_ROUTING_EXPORTS _declspec(dllexport)
#endif
#else
#ifndef DLL_CLASS_ORDER_ROUTING_EXPORTS
#define DLL_CLASS_ORDER_ROUTING_EXPORTS _declspec(dllimport)
//#		define DLL_CLASS_ORDER_ROUTING_EXPORTS
#endif
#endif  // ORDERROUTING_EXPORTS
#else
#define DLL_CLASS_ORDER_ROUTING_EXPORTS
#endif  // _MSC_VER
