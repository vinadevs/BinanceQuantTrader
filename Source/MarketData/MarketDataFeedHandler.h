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
		// - We are using "pass by value" in param for std::move later
		bool HandleIndividualBookTickerData(const char* fl, int ec, std::string emsg, binapi::ws::book_ticker_t book);
		bool HandleTradeData(const char* fl, int ec, std::string emsg, binapi::ws::trade_t trade);
		bool HandleIndividualMarketTickerData(const char* fl, int ec, std::string emsg, binapi::ws::market_ticker_t market);
		bool HandleMiniTickerData(const char* fl, int ec, std::string emsg, binapi::ws::mini_ticker_t mini);
		bool HandleAggregateTradeData(const char* fl, int ec, std::string emsg, binapi::ws::agg_trade_t aggregate);
		bool HandleKlineCandleStickData(const char* fl, int ec, std::string emsg, binapi::ws::kline_t kline);
		bool HandleAllMiniTickerData(const char* fl, int ec, std::string emsg, binapi::ws::mini_tickers_t mini);
		bool HandleAllMarketTickersData(const char* fl, int ec, std::string emsg, binapi::ws::markets_tickers_t market);
		bool HandlePartDepthData(const char* fl, int ec, std::string emsg, binapi::ws::part_depths_t depth);
		bool HandleDiffDepthData(const char* fl, int ec, std::string emsg, binapi::ws::diff_depths_t depth);
		bool HandleUserDataAccountUpdate(const char* fl, int ec, std::string emsg, binapi::userdata::account_update_t accountUpdate);
		bool HandleUserDataBalanceUpdate(const char* fl, int ec, std::string emsg, binapi::userdata::balance_update_t balanceUpdate);
		bool HandleUserDataOrderUpdate(const char* fl, int ec, std::string emsg, binapi::userdata::order_update_t orderUpdate);

		// Set and get part diff symbol, this is used to store the symbol of part diff depth data
		void SetPartDiffSymbol(const std::string& symbol) { m_partDiffSymbol = symbol; }
		const std::string& GetPartDiffSymbol() const { return m_partDiffSymbol; }
	private:
		std::unique_ptr<MarketDataFeedManager> m_synchronousFeedMgr;
		std::string m_partDiffSymbol; // this is used to store the symbol of part diff depth data
	};
};