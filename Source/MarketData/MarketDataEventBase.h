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

namespace MarketData {
	class MarketDataEventBase
	{
	public:
		MarketDataEventBase(const tinyxml2::XMLElement* marketDataConfigXml) 
			: m_marketDataConfigXml(marketDataConfigXml) { }

		virtual bool Subscribe(const std::string& symbol) = 0;
		virtual bool Unsubscribe(const std::string& symbol) = 0;
		virtual bool IsSubscribed(const std::string& symbol) = 0;

		const std::unordered_set<std::string>& GetSubscribingSymbols() const
		{
			return m_subscribedSymbols;
		}

	protected:
		std::unordered_set<std::string> m_subscribedSymbols;
		std::unique_ptr<LibraryUtils::Logger> m_logger;
		const tinyxml2::XMLElement* m_marketDataConfigXml{ nullptr };
	};
};