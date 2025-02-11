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

#include "../KernelTrading/types.h"

#include "MarketDataSubject.h"
#include "SynchronousMarketDataFeed.h"

// A feed handler is a critical component in financial trading systems,
// responsible for receiving, processing, and distributing real - time market data 
// from exchanges or data providers to various systems, traders, and applications.
// This data includes updates on prices, trades, orders, and other relevant market 
// information.

namespace MarketData {
	class SynchronousMarketDataFeed;
	class MarketDataFeedHandler : public MarketDataSubject
	{
	public:
		MarketDataFeedHandler();

		// register data listeners
		void RegisterObserver(MarketDataObserver* observer);
		void UnregisterObserver(MarketDataObserver* observer);
		// market data feed access
		bool CreateNewMarketDataFeed(const std::string& symbol);
		void RemoveMarketDataFeed(const std::string& symbol);
		SynchronousMarketData* GetSynchronousMarketData(const std::string& symbol) override;

		// - All handling feed functions are ansynchronous calls, it means each
		// handling function will be called back from a seperated worker thread.
		// - After we processed market data feed, we will notify changes to subscribed listeners
		bool HandleIndividualBookTickerData(const char* fl,
			int ec, std::string emsg, binapi::ws::book_ticker_t book);
		bool HandleTradeData(const char* fl,
			int ec, std::string emsg, binapi::ws::trade_t trade);
	private:
		std::unique_ptr<MarketDataFeedManager> m_synchronousFeedMgr;
	};
};