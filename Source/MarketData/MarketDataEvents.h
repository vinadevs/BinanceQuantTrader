/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

// CAREFULLY WITH HEADER ORDER INCLUDING AND 
// DLL_CLASS STUFFS, THEY ARE PAIN IN ASS BUILD ISSUE

#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>

#include "MarketDataSubscriptionManager.h"

#include <memory>
#include <vector>
#include <string>
#include <unordered_set>

namespace tinyxml2 {
	class XMLElement;
};

namespace LibraryUtils {
	class Logger;
};

// In trading, market data events refer to the dissemination of real - time
// or historical information about various financial instruments(e.g., stocks, bonds, 
// commodities, derivatives, etc.) to traders, investors, and other market
// participants.These events include price changes, order book updates, trade executions,
// and other significant information that impacts trading decisions.

namespace MarketData {
	class MarketDataFeedHandler;
	class MarketDataEvents
	{
	public:
		MarketDataEvents(
			const tinyxml2::XMLElement* mkDataConfigXml,
			MarketDataFeedHandler* feedHandler);
		~MarketDataEvents();

		void StartSubscriptionEvents();
		void Wait();
		const std::unordered_set<std::string>& GetSubscribingSymbols() const;
	private:
		// choose what we want to receive from exchange
		void SubscibeIndividualBookTicker(const std::string& symbol);
		void SubscibeTrade(const std::string& symbol);
		void SubscibeIndividualMarketTicker(const std::string& symbol);
		void SubscibeAllMarketTickers(const std::string& symbol);
		void SubscibeIndividualMiniTicker(const std::string& symbol);
		void SubscibeAllMiniTickers(const std::string& symbol);
		void SubscibeAggregateTrade(const std::string& symbol);
		void SubscibeKlineCandleStick(const std::string& symbol);
		void SubscibePartDepth(const std::string& symbol);
		void SubscibeDiffDepth(const std::string& symbol);
		void SubscibeUserData(const std::string& symbol);
		// remove what we dont want to receive from exchange
		void Unsubscribe(const binapi::ws::websockets::handle& h);
		void AsyncUnsubscribe(const binapi::ws::websockets::handle& h);
		void UnsubscribeAll();
		void AsyncUnsubscribeAll();
		// check what we have subcribed
		void VerifySubscriptionHandle(
			const std::string& symbol,
			const std::string& dataName,
			binapi::ws::websockets::handle h,
			const SubscriptionHandleType type);
		// load static symbols to subscribe
		void LoadInterestingDataSymbols(const char* filePath);

		std::unordered_set<std::string> m_subscribingSymbols;
		boost::asio::io_context m_ioContext;
		std::unique_ptr<binapi::ws::websockets> m_webSocketRealTime;
		std::unique_ptr<MarketDataSubscriptionManager> m_mdSubscriptionMgr;
		MarketDataFeedHandler* m_feedHandler{ nullptr };
		std::unique_ptr<LibraryUtils::Logger> m_logger;
		const tinyxml2::XMLElement* m_configXml{ nullptr };
	};
};