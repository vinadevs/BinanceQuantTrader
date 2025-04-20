/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "SynchronousMarketData.h"

#include <list>
#include <string>

namespace MarketData {

class MarketDataObserver;

/**
 * @class MarketDataSubject
 * @brief Represents the subject in the Observer design pattern.
 *
 * The Subject class maintains a list of observers and notifies them of any changes
 * to its state. Observers register themselves with the subject to receive updates
 * whenever a specific event occurs.
 *
 * Key Responsibilities:
 * - Maintain a collection of observers.
 * - Provide methods to add, remove, and notify observers.
 * - Notify all registered observers when a state change occurs.
 */

class DLL_CLASS_MARKETDATA_EXPORTS MarketDataSubject
{
public:
	MarketDataSubject() = default;
	virtual ~MarketDataSubject();

	virtual void AttachMarketDataObserver(MarketDataObserver* observer);
	virtual void DettachMarketDataObserver(MarketDataObserver* observer);
	virtual void ClearMarketDataObservers();

	// This function is a sycnchronous call, only one notify message can
	// be delivered to listeners each time...
	// We should call AddMessage() before call this function
	// Return int as we need to count number of succeeded updates
	// Input symbol param specifics for symbol has data change
	virtual int NotifyIndividualBookTickerChange(const std::string& symbol);
	virtual int NotifyTradeChange(const std::string& symbol);
	virtual int NotifyIndividualMarketTickerChange(const std::string& symbol);
	virtual int NotifyMiniTickerChange(const std::string& symbol);
	virtual int NotifyKlineCandleStickChange(const std::string& symbol);
	virtual int NotifyAggregateTradeChange(const std::string& symbol);
	virtual int NotifyAllMarketTickersChange(const std::string& symbol);
	virtual int NotifyAllMiniTickersChange(const std::string& symbol);
	virtual int NotifyAllBookTickersChange(const std::string& symbol);
	virtual int NotifyAllPartDepthChange(const std::string& symbol);
	virtual int NotifyAllDiffDepthChange(const std::string& symbol);

	// get synchronous market data
	virtual SynchronousMarketData* GetSynchronousMarketData(const std::string& symbol) { return nullptr; }
protected:
	// collection of observers
	using ListMarketDataObserver = std::list<MarketDataObserver*>;
	ListMarketDataObserver m_marketDataObservers;
};

};