/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

// Macro flag to enable or disable multi-threading trading mode
#define USE_MULTITHREADING 1  // Set to 0 to disable multi-threading
#if USE_MULTITHREADING
#define USE_MULTITHREADING_MESSAGE "Using Multiple threads trading mode."
#else
#define USE_MULTITHREADING_MESSAGE "Using singe thread trading mode."
#endif

#define ENABLE_FEATURE 1
#define DISABLE_FEATURE 0

// Macro flag to enable or disable test trading mode
#define USE_BACK_TEST_TRADING DISABLE_FEATURE  // Set to 0 to disable test mode, CAREFULLY, lose monney!!!!
#define USE_BINANCE_TEST_TRADING ENABLE_FEATURE  // Set to 0 to disable test mode, CAREFULLY, lose monney!!!!
#if USE_BACK_TEST_TRADING
#define USE_BACK_TEST_TRADING_MESSAGE "Using back test trading mode."
#elif USE_BINANCE_TEST_TRADING
#define USE_BINANCE_TEST_TRADING_MESSAGE "Using binance test trading mode."
#else
#define USE_REAL_TRADING_MESSAGE "Using real trading mode."
#endif
