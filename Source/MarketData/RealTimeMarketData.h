/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

// CAREFULLY WITH HEADER ORDER INCLUDING AND 
// DLL_CLASS STUFFS, THEY ARE PAIN IN ASS BUILD ISSUE
// DONT USE SAME NAME FOR MACRO DEFINE DLL_CLASS

#pragma once

#include "dlldefine.h"

#include <memory>
#include <unordered_set>

#include "MarketDataFeedHandler.h"
#include "MarketDataEvents.h"

namespace tinyxml2 {
	class XMLElement;
};

namespace MarketData {

class MarketDataObserver;

/*
 * @file RealTimeMarketData.h
 * @brief High-Frequency Trading (HFT) Real-Time Market Data Module
 *
 * This module is responsible for handling real-time market data streams
 * in a high-frequency trading (HFT) environment. It provides functionalities
 * for subscribing to market data feeds, parsing incoming data, and notifying
 * trading strategies with actionable insights in real-time.
 *
 * Key Features:
 * - **Market Data Subscription**:
 *   Connects to multiple market data providers and subscribes to relevant
 *   instruments and market events.
 *
 * - **Data Parsing**:
 *   Efficiently parses raw data from the provider-specific format into
 *   a unified internal structure.
 *
 * - **Low-Latency Notifications**:
 *   Delivers processed data to trading algorithms with minimal latency (nanoseconds)
 *   to ensure timely decision-making.
 *
 * - **Error Handling and Recovery**:
 *   Detects and handles network disruptions or data feed errors to maintain
 *   robust operations.
 *
 * Typical Usage:
 * 1. Initialize the module with configuration parameters such as provider
 *    endpoints and instruments of interest.
 * 2. Subscribe to desired instruments and register callbacks for data updates.
 * 3. Start the market data feed and process incoming data.
 *
 */

// -A RealTimeMarketData class simulates or integrates real - time data 
// from a source such as an exchange, broker, or feed.In a trading system,
// it would handle the continuous flow of real - time data and notify
// registered strategies or a TradingModel to react accordingly.

class DLL_CLASS_MARKETDATA_EXPORTS RealTimeMarketData
{
public:
	RealTimeMarketData(const tinyxml2::XMLElement* mkDataConfigXml);
	~RealTimeMarketData();

	// These 2 functions must be called at the class
	// wants to use real time market data
	void RegisterDataListener(MarketDataObserver* observer);
	void UnRegisterDataListener(MarketDataObserver* observer);

	void StartStreamingData();
	// This function must be called after subscribing symbols
	// usually we called it in strategy StartLive() function to create an ansync wait
	void StartIOContext();
	bool SubscribeSymbol(const std::string& symbol);
	bool UnsubscribeSymbol(const std::string& symbol);
	bool IsSubscribedSymbol(const std::string& symbol);
	const std::unordered_set<std::string>& GetSubscribingSymbols() const;
	MarketDataFeedHandler* GetFeedHandler() const;
private:
	std::unique_ptr<MarketDataFeedHandler> m_feedHandler;
	std::unique_ptr<MarketDataEvents> m_dataEvents;
};
};