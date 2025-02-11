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

// Macro flag to enable or disable test trading mode
#define USE_TEST_TRADING 1  // Set to 0 to disable test mode, CAREFULLY, lose monney!!!!
#if USE_TEST_TRADING
#define USE_TEST_TRADING_MESSAGE "Using test trading mode."
#else
#define USE_TEST_TRADING_MESSAGE "Using real trading mode."
#endif
