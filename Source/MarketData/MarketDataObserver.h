/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include <string>

namespace MarketData {

class MarketDataSubject;

/**
 * @class MarketDataObserver
 * @brief Represents the observer in the Observer design pattern.
 *
 * The Observer class defines an interface for objects that should be notified
 * of changes in a subject's state. Concrete implementations of this class
 * implement the update logic to handle notifications from the subject.
 *
 * Key Responsibilities:
 * - Define an interface for receiving updates from the subject.
 * - Implement custom behavior in derived classes to respond to subject updates.
 */

class DLL_CLASS_MARKETDATA_EXPORTS MarketDataObserver
{
public:
	MarketDataObserver() = default;
	virtual ~MarketDataObserver() = default;
	// These are asychronously callbacks, and will be listened 
	// at child classes for market data change events
	virtual bool OnIndividualBookTickerChange(MarketDataSubject* marketData, const std::string& symbol);
	virtual bool OnTradeChange(MarketDataSubject* marketData, const std::string& symbol);
	virtual bool OnIndividualMarketTickerChange(MarketDataSubject* marketData, const std::string& symbol);
	virtual bool OnMiniTickerChange(MarketDataSubject* marketData, const std::string& symbol);
	virtual bool OnAggregateTradeChange(MarketDataSubject* marketData, const std::string& symbol);
	virtual bool OnKlineCandleStickChange(MarketDataSubject* marketData, const std::string& symbol);
	virtual bool OnAllMarketTickersChange(MarketDataSubject* marketData, const std::string& symbol);
	virtual bool OnAllMiniTickersChange(MarketDataSubject* marketData, const std::string& symbol);
	virtual bool OnAllPartDepthChange(MarketDataSubject* marketData, const std::string& symbol);
	virtual bool OnAllDiffDepthChange(MarketDataSubject* marketData, const std::string& symbol);
};

};