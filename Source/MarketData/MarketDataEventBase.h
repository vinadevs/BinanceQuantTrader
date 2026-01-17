/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include <memory>
#include <string>
#include <unordered_set>

#include "../LibraryUtils/Logger.h"

namespace tinyxml2 {
	class XMLElement;
};

// Base class for market data events, providing an interface for subscribing,
// unsubscribing, and managing market data subscriptions.

namespace MarketData {
	class MarketDataSubject;
	class MarketDataEventBase
	{
	public:
		MarketDataEventBase(const tinyxml2::XMLElement* marketDataConfigXml, MarketDataSubject* marketDataSubject)
			: m_marketDataConfigXml(marketDataConfigXml)
			, m_marketDataSubject(marketDataSubject) {}

		virtual ~MarketDataEventBase() = default;

		// Subscribe to market data for a specific symbol
		virtual bool Subscribe(const std::string& symbol) = 0;
		// Unsubscribe from market data for a specific symbol
		virtual bool Unsubscribe(const std::string& symbol) = 0;
		// Check if subscribed to market data for a specific symbol
		virtual bool IsSubscribed(const std::string& symbol) = 0;
		// Wait for market data events (blocking call), this call has to be called after subscribing symbols
		virtual void Wait() = 0;
		// Operate IO context for asynchronous operations, not apply for historical data
		virtual void StartIOContext() = 0;

		const std::unordered_set<std::string>& GetSubscribingSymbols() const
		{
			return m_subscribedSymbols;
		}

		MarketDataSubject* GetMarketDataSubject() const
		{
			return m_marketDataSubject;
		}

	protected:
		std::unordered_set<std::string> m_subscribedSymbols;
		std::unique_ptr<LibraryUtils::Logger> m_logger;
		const tinyxml2::XMLElement* m_marketDataConfigXml{ nullptr };
		MarketDataSubject* m_marketDataSubject{ nullptr };
	};
};