/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"
#include "../KernelTrading/double_type.h"

#include "SingleMarketDataFeed.h"
#include "TableMarketDataFeed.h"
#include "ArrayMarketDtaFeed.h"

#include <memory>
#include <string>

#pragma warning(disable : 4251)
#pragma warning(disable : 4267)
#pragma warning(disable : 4715)

namespace MarketData {

	//////////////////////////////////////////////////////////////////////////////////////
	// This is a wrapper layer for different market data types from kernel trading
	// The pupose is to have a common interface for different market data types
	// So the raw market data from Binanace Exchange will be used as SingleMarketDataFeed
	// and SynchronousMarketData will contain all of SingleMarketDataFeeds
	// TODO: Create another low latency version without wrapper layer
	//////////////////////////////////////////////////////////////////////////////////////

	// Class base for Binance Market Data
	class BinanceMarketData
	{
		virtual std::string ToString() = 0;
	protected:
		std::string m_dataName;
	};

	// Level 2 market data: Bid Ask price, quantity
	class DLL_CLASS_MARKETDATA_EXPORTS IndividualBookTickerData : public BinanceMarketData
	{
	public:
		// Rule of 5, for class containing unique pointers
		IndividualBookTickerData();
		IndividualBookTickerData(const IndividualBookTickerData& other);
		IndividualBookTickerData& operator=(const IndividualBookTickerData& other);
		IndividualBookTickerData(IndividualBookTickerData&& other) noexcept = default;
		IndividualBookTickerData& operator=(IndividualBookTickerData&& other) noexcept = default;
		~IndividualBookTickerData() = default;

		std::string ToString() override;

		std::unique_ptr<SingleMarketDataFeed> m_updateId;
		std::unique_ptr<SingleMarketDataFeed> m_bestBidPrice;
		std::unique_ptr<SingleMarketDataFeed> m_bestBidQty;
		std::unique_ptr<SingleMarketDataFeed> m_bestAskPrice;
		std::unique_ptr<SingleMarketDataFeed> m_bestAskQty;
		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs; // There is no event time from Binance API, this is internal update time

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const IndividualBookTickerData& o);
	};

	// Level 1 market data: other trading data: Kline, VWAP, Volume,...
	class DLL_CLASS_MARKETDATA_EXPORTS TradeData : public BinanceMarketData // trade_t
	{
	public:
		// Rule of 5, for class containing unique pointers
		TradeData();
		TradeData(const TradeData& other);
		TradeData& operator=(const TradeData& other);
		TradeData(TradeData&& other) noexcept = default;
		TradeData& operator=(TradeData&& other) noexcept = default;
		~TradeData() = default;

		std::string ToString() override;

		std::unique_ptr<SingleMarketDataFeed> m_tradeId;
		std::unique_ptr<SingleMarketDataFeed> m_price;
		std::unique_ptr<SingleMarketDataFeed> m_quantity;
		std::unique_ptr<SingleMarketDataFeed> m_buyerOrderID;
		std::unique_ptr<SingleMarketDataFeed> m_sellerOrderID;
		std::unique_ptr<SingleMarketDataFeed> m_tradeTime;
		std::unique_ptr<SingleMarketDataFeed> m_isBuyerTheMarketMaker;
		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs;

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const TradeData& o);
	};

	class DLL_CLASS_MARKETDATA_EXPORTS IndividualMarketTickerData : public BinanceMarketData // market_ticker_t
	{
	public:
		// Rule of 5, for class containing unique pointers
		IndividualMarketTickerData();
		IndividualMarketTickerData(const IndividualMarketTickerData& other);
		IndividualMarketTickerData& operator=(const IndividualMarketTickerData& other);
		IndividualMarketTickerData(IndividualMarketTickerData&& other) noexcept = default;
		IndividualMarketTickerData& operator=(IndividualMarketTickerData&& other) noexcept = default;
		~IndividualMarketTickerData() = default;

		std::string ToString() override;

		std::unique_ptr<SingleMarketDataFeed> m_priceChange;
		std::unique_ptr<SingleMarketDataFeed> m_priceChangePercent;
		std::unique_ptr<SingleMarketDataFeed> m_weightedAvgPrice;
		std::unique_ptr<SingleMarketDataFeed> m_firstTradePrice;
		std::unique_ptr<SingleMarketDataFeed> m_lastPrice;
		std::unique_ptr<SingleMarketDataFeed> m_lastQuantity;
		std::unique_ptr<SingleMarketDataFeed> m_bestBidPrice;
		std::unique_ptr<SingleMarketDataFeed> m_bestBidQty;
		std::unique_ptr<SingleMarketDataFeed> m_bestAskPrice;
		std::unique_ptr<SingleMarketDataFeed> m_bestAskQty;
		std::unique_ptr<SingleMarketDataFeed> m_openPrice;
		std::unique_ptr<SingleMarketDataFeed> m_highPrice;
		std::unique_ptr<SingleMarketDataFeed> m_lowPrice;
		std::unique_ptr<SingleMarketDataFeed> m_totalTradedBaseAssetVolume;
		std::unique_ptr<SingleMarketDataFeed> m_totalTradedQuoteAssetVolume;
		std::unique_ptr<SingleMarketDataFeed> m_statisticsOpenTimeMs;
		std::unique_ptr<SingleMarketDataFeed> m_statisticsCloseTimeMs;
		std::unique_ptr<SingleMarketDataFeed> m_firstTradeId;
		std::unique_ptr<SingleMarketDataFeed> m_lastTradeId;
		std::unique_ptr<SingleMarketDataFeed> m_totalNumberOfTrades;
		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs;

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const IndividualMarketTickerData& o);
	};

	class DLL_CLASS_MARKETDATA_EXPORTS AllMarketTickerData : public BinanceMarketData // market_ticker_all_t
	{
	public:
		// Rule of 0
		AllMarketTickerData();
		AllMarketTickerData(const AllMarketTickerData& other);
		AllMarketTickerData& operator=(const AllMarketTickerData& other);
		AllMarketTickerData(AllMarketTickerData&& other) noexcept = default;
		AllMarketTickerData& operator=(AllMarketTickerData&& other) noexcept = default;
		~AllMarketTickerData() = default;

		std::string ToString() override;

		std::unique_ptr<TableMarketDataFeed<IndividualMarketTickerData>> m_allIndividualMarketTicker;
		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs;

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const AllMarketTickerData& o);
	};

	class IndividualMiniTickerData : public BinanceMarketData // mini_ticker_t
	{
	public:
		// Rule of 5, for class containing unique pointers
		IndividualMiniTickerData();
		IndividualMiniTickerData(const IndividualMiniTickerData& other);
		IndividualMiniTickerData& operator=(const IndividualMiniTickerData& other);
		IndividualMiniTickerData(IndividualMiniTickerData&& other) noexcept = default;
		IndividualMiniTickerData& operator=(IndividualMiniTickerData&& other) noexcept = default;
		~IndividualMiniTickerData() = default;

		std::string ToString() override;

		std::unique_ptr<SingleMarketDataFeed> m_closePrice;
		std::unique_ptr<SingleMarketDataFeed> m_openPrice;
		std::unique_ptr<SingleMarketDataFeed> m_highPrice;
		std::unique_ptr<SingleMarketDataFeed> m_lowPrice;
		std::unique_ptr<SingleMarketDataFeed> m_totalTradedBaseAssetVolume;
		std::unique_ptr<SingleMarketDataFeed> m_totalTradedQuoteAssetVolume;
		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs;

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const IndividualMiniTickerData& o);
	};

	class DLL_CLASS_MARKETDATA_EXPORTS AllMiniTickerData : public BinanceMarketData // mini_ticker_all_t
	{
	public:
		// Rule of 5, for class containing unique pointers
		AllMiniTickerData();
		AllMiniTickerData(const AllMiniTickerData& other);
		AllMiniTickerData& operator=(const AllMiniTickerData& other);
		AllMiniTickerData(AllMiniTickerData&& other) noexcept = default;
		AllMiniTickerData& operator=(AllMiniTickerData&& other) noexcept = default;
		~AllMiniTickerData() = default;

		std::string ToString() override;

		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs;
		std::unique_ptr<TableMarketDataFeed<IndividualMiniTickerData>> m_tableMarketTicker;

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const AllMiniTickerData& o);
	};

	class DLL_CLASS_MARKETDATA_EXPORTS AggregateTradeData : public BinanceMarketData // aggregate_trade_t
	{
	public:
		// Rule of 5, for class containing unique pointers
		AggregateTradeData();
		AggregateTradeData(const AggregateTradeData& other);
		AggregateTradeData& operator=(const AggregateTradeData& other);
		AggregateTradeData(AggregateTradeData&& other) noexcept = default;
		AggregateTradeData& operator=(AggregateTradeData&& other) noexcept = default;
		~AggregateTradeData() = default;

		std::string ToString() override;

		std::unique_ptr<SingleMarketDataFeed> m_aggregateTradeId;
		std::unique_ptr<SingleMarketDataFeed> m_price;
		std::unique_ptr<SingleMarketDataFeed> m_quantity;
		std::unique_ptr<SingleMarketDataFeed> m_firstTradeId;
		std::unique_ptr<SingleMarketDataFeed> m_lastTradeId;
		std::unique_ptr<SingleMarketDataFeed> m_totalNumberOfTrades;
		std::unique_ptr<SingleMarketDataFeed> m_tradeTimeMs;
		std::unique_ptr<SingleMarketDataFeed> m_isBuyerMarketMaker;
		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs;

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const AggregateTradeData& o);
	};

	class DLL_CLASS_MARKETDATA_EXPORTS KlineCandleStickData : public BinanceMarketData // kline_candlestick_t
	{
	public:
		// Rule of 5, for class containing unique pointers
		KlineCandleStickData();
		KlineCandleStickData(const KlineCandleStickData& other);
		KlineCandleStickData& operator=(const KlineCandleStickData& other);
		KlineCandleStickData(KlineCandleStickData&& other) noexcept = default;
		KlineCandleStickData& operator=(KlineCandleStickData&& other) noexcept = default;
		~KlineCandleStickData() = default;

		std::string ToString() override;

		std::unique_ptr<SingleMarketDataFeed> m_klineStartTime;
		std::unique_ptr<SingleMarketDataFeed> m_klineCloseTime;
		std::unique_ptr<SingleMarketDataFeed> m_interval;
		std::unique_ptr<SingleMarketDataFeed> m_firstTradeId;
		std::unique_ptr<SingleMarketDataFeed> m_lastTradeId;
		std::unique_ptr<SingleMarketDataFeed> m_openPrice;
		std::unique_ptr<SingleMarketDataFeed> m_closePrice;
		std::unique_ptr<SingleMarketDataFeed> m_highPrice;
		std::unique_ptr<SingleMarketDataFeed> m_lowPrice;
		std::unique_ptr<SingleMarketDataFeed> m_baseAssetVolume;
		std::unique_ptr<SingleMarketDataFeed> m_numberOfTrades;
		std::unique_ptr<SingleMarketDataFeed> m_isThisKlineClosed;
		std::unique_ptr<SingleMarketDataFeed> m_quoteAssetVolume;
		std::unique_ptr<SingleMarketDataFeed> m_takerBuyBaseAssetVolume;
		std::unique_ptr<SingleMarketDataFeed> m_takerBuyQuoteAssetVolume;
		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs;

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const KlineCandleStickData& o);
	};

	class DLL_CLASS_MARKETDATA_EXPORTS DepthData : public BinanceMarketData // depth_t
	{
	public:
		// Rule of 5, for class containing unique pointers
		DepthData();
		DepthData(const DepthData& other);
		DepthData& operator=(const DepthData& other);
		DepthData(DepthData&& other) noexcept = default;
		DepthData& operator=(DepthData&& other) noexcept = default;
		~DepthData() = default;

		std::string ToString() override;

		std::unique_ptr<SingleMarketDataFeed> m_price;
		std::unique_ptr<SingleMarketDataFeed> m_amount;
		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs;

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const DepthData& o);
	};

	class DLL_CLASS_MARKETDATA_EXPORTS AllPartDepthData : public BinanceMarketData // depth_all_t
	{
	public:
		// Rule of 5, for class containing unique pointers
		AllPartDepthData();
		AllPartDepthData(const AllPartDepthData& other);
		AllPartDepthData& operator=(const AllPartDepthData& other);
		AllPartDepthData(AllPartDepthData&& other) noexcept = default;
		AllPartDepthData& operator=(AllPartDepthData&& other) noexcept = default;
		~AllPartDepthData() = default;

		std::string ToString() override;

		std::unique_ptr<ArrayMarketDataFeed<DepthData>> m_bids;
		std::unique_ptr<ArrayMarketDataFeed<DepthData>> m_asks;
		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs;

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const AllPartDepthData& o);
	};

	class DLL_CLASS_MARKETDATA_EXPORTS AllDiffDepthData : public BinanceMarketData // diff_depth_t
	{
	public:
		// Rule of 5, for class containing unique pointers
		AllDiffDepthData();
		AllDiffDepthData(const AllDiffDepthData& other);
		AllDiffDepthData& operator=(const AllDiffDepthData& other);
		AllDiffDepthData(AllDiffDepthData&& other) noexcept = default;
		AllDiffDepthData& operator=(AllDiffDepthData&& other) noexcept = default;
		~AllDiffDepthData() = default;

		std::string ToString() override;

		std::unique_ptr<SingleMarketDataFeed> m_firstUpdateID;
		std::unique_ptr<SingleMarketDataFeed> m_finalUpdateID;
		std::unique_ptr<ArrayMarketDataFeed<DepthData>> m_bids;
		std::unique_ptr<ArrayMarketDataFeed<DepthData>> m_asks;
		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs;

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const AllDiffDepthData& o);
	};

	class DLL_CLASS_MARKETDATA_EXPORTS UserDataAccount : public BinanceMarketData // account_t
	{
		// Rule of 5, for class containing unique pointers
	public:
		UserDataAccount();
		UserDataAccount(const UserDataAccount& other);
		UserDataAccount& operator=(const UserDataAccount& other);
		UserDataAccount(UserDataAccount&& other) noexcept = default;
		UserDataAccount& operator=(UserDataAccount&& other) noexcept = default;
		~UserDataAccount() = default;
		std::string ToString() override;

		//{
		//	"subscriptionId": 0,
		//		"event" : {
		//		"e": "outboundAccountPosition", // Event type
		//			"E" : 1564034571105,        // Event Time
		//			"u" : 1564034571073,        // Time of last account update
		//			"B" :                       // Balances Array
		//			[
				//		{
				//			"a": "ETH",             // Asset
				//			"f" : "10000.000000",   // Free
				//			"l" : "0.000000"        // Locked
				//		}
		//			]
		//	}
		//}

		std::unique_ptr<SingleMarketDataFeed> m_asset;
		std::unique_ptr<SingleMarketDataFeed> m_free;
		std::unique_ptr<SingleMarketDataFeed> m_locked;
		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs;
		std::unique_ptr<SingleMarketDataFeed> m_eventType;
		std::unique_ptr<SingleMarketDataFeed> m_timeOfLastAccountUpdateMs;

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const UserDataAccount& o);
	};

	class DLL_CLASS_MARKETDATA_EXPORTS UserDataBalance : public BinanceMarketData // balance_t
	{
		// Rule of 5, for class containing unique pointers
	public:
		UserDataBalance();
		UserDataBalance(const UserDataBalance& other);
		UserDataBalance& operator=(const UserDataBalance& other);
		UserDataBalance(UserDataBalance&& other) noexcept = default;
		UserDataBalance& operator=(UserDataBalance&& other) noexcept = default;
		~UserDataBalance() = default;
		std::string ToString() override;

		//{
		//	"subscriptionId": 0,
		//		"event" : {
		//		"e": "balanceUpdate",         // Event Type
		//			"E" : 1573200697110,      // Event Time
		//			"a" : "BTC",              // Asset
		//			"d" : "100.00000000",     // Balance Delta
		//			"T" : 1573200697068       // Clear Time
		//	}
		//}

		std::unique_ptr<SingleMarketDataFeed> m_asset;
		std::unique_ptr<SingleMarketDataFeed> m_balanceDelta;
		std::unique_ptr<SingleMarketDataFeed> m_clearTimeMs;
		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs;
		std::unique_ptr<SingleMarketDataFeed> m_eventType;

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const UserDataBalance& o);
	};

	class DLL_CLASS_MARKETDATA_EXPORTS UserDataOrder : public BinanceMarketData // order_t
	{
		// Rule of 5, for class containing unique pointers
	public:
		UserDataOrder();
		UserDataOrder(const UserDataOrder& other);
		UserDataOrder& operator=(const UserDataOrder& other);
		UserDataOrder(UserDataOrder&& other) noexcept = default;
		UserDataOrder& operator=(UserDataOrder&& other) noexcept = default;
		~UserDataOrder() = default;
		std::string ToString() override;

		//{
		//	"subscriptionId": 0,
		//		"event" : {
		//		"e": "executionReport",         // Event type
		//			"E" : 1499405658658,             // Event time
		//			"s" : "ETHBTC",                  // Symbol
		//			"c" : "mUvoqJxFIILMdfAW5iGSOW",  // Client order ID
		//			"S" : "BUY",                     // Side
		//			"o" : "LIMIT",                   // Order type
		//			"f" : "GTC",                     // Time in force
		//			"q" : "1.00000000",              // Order quantity
		//			"p" : "0.10264410",              // Order price
		//			"P" : "0.00000000",              // Stop price
		//			"F" : "0.00000000",              // Iceberg quantity
		//			"g" : -1,                        // OrderListId
		//			"C" : "",                        // Original client order ID; This is the ID of the order being canceled
		//			"x" : "NEW",                     // Current execution type
		//			"X" : "NEW",                     // Current order status
		//			"r" : "NONE",                    // Order reject reason; Please see Order Reject Reason (below) for more information.
		//			"i" : 4293153,                   // Order ID
		//			"l" : "0.00000000",              // Last executed quantity
		//			"z" : "0.00000000",              // Cumulative filled quantity
		//			"L" : "0.00000000",              // Last executed price
		//			"n" : "0",                       // Commission amount
		//			"N" : null,                      // Commission asset
		//			"T" : 1499405658657,             // Transaction time
		//			"t" : -1,                        // Trade ID
		//			"v" : 3,                         // Prevented Match Id; This is only visible if the order expired due to STP
		//			"I" : 8641984,                   // Execution Id
		//			"w" : true,                      // Is the order on the book?
		//			"m" : false,                     // Is this trade the maker side?
		//			"M" : false,                     // Ignore
		//			"O" : 1499405658657,             // Order creation time
		//			"Z" : "0.00000000",              // Cumulative quote asset transacted quantity
		//			"Y" : "0.00000000",              // Last quote asset transacted quantity (i.e. lastPrice * lastQty)
		//			"Q" : "0.00000000",              // Quote Order Quantity
		//			"W" : 1499405658657,             // Working Time; This is only visible if the order has been placed on the book.
		//			"V" : "NONE"                     // SelfTradePreventionMode
		//	}

        std::unique_ptr<SingleMarketDataFeed> m_eventType;                // "e": Event type
        std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs;              // "E": Event time
        std::unique_ptr<SingleMarketDataFeed> m_symbol;                   // "s": Symbol
        std::unique_ptr<SingleMarketDataFeed> m_clientOrderId;            // "c": Client order ID
        std::unique_ptr<SingleMarketDataFeed> m_side;                     // "S": Side
        std::unique_ptr<SingleMarketDataFeed> m_orderType;                // "o": Order type
        std::unique_ptr<SingleMarketDataFeed> m_timeInForce;              // "f": Time in force
        std::unique_ptr<SingleMarketDataFeed> m_orderQuantity;            // "q": Order quantity
        std::unique_ptr<SingleMarketDataFeed> m_orderPrice;               // "p": Order price
        std::unique_ptr<SingleMarketDataFeed> m_stopPrice;                // "P": Stop price
        std::unique_ptr<SingleMarketDataFeed> m_icebergQuantity;          // "F": Iceberg quantity
        std::unique_ptr<SingleMarketDataFeed> m_orderListId;              // "g": OrderListId
        std::unique_ptr<SingleMarketDataFeed> m_originalClientOrderId;    // "C": Original client order ID
        std::unique_ptr<SingleMarketDataFeed> m_currentExecutionType;     // "x": Current execution type
        std::unique_ptr<SingleMarketDataFeed> m_currentOrderStatus;       // "X": Current order status
        std::unique_ptr<SingleMarketDataFeed> m_orderRejectReason;        // "r": Order reject reason
        std::unique_ptr<SingleMarketDataFeed> m_orderId;                  // "i": Order ID
        std::unique_ptr<SingleMarketDataFeed> m_lastExecutedQuantity;     // "l": Last executed quantity
        std::unique_ptr<SingleMarketDataFeed> m_cumulativeFilledQuantity; // "z": Cumulative filled quantity
        std::unique_ptr<SingleMarketDataFeed> m_lastExecutedPrice;        // "L": Last executed price
        std::unique_ptr<SingleMarketDataFeed> m_commissionAmount;         // "n": Commission amount
        std::unique_ptr<SingleMarketDataFeed> m_commissionAsset;          // "N": Commission asset
        std::unique_ptr<SingleMarketDataFeed> m_transactionTimeMs;        // "T": Transaction time
        std::unique_ptr<SingleMarketDataFeed> m_tradeId;                  // "t": Trade ID
        std::unique_ptr<SingleMarketDataFeed> m_preventedMatchId;         // "v": Prevented Match Id
        std::unique_ptr<SingleMarketDataFeed> m_executionId;              // "I": Execution Id
        std::unique_ptr<SingleMarketDataFeed> m_isOrderOnBook;            // "w": Is the order on the book?
        std::unique_ptr<SingleMarketDataFeed> m_isMakerSide;              // "m": Is this trade the maker side?
        std::unique_ptr<SingleMarketDataFeed> m_ignore;                   // "M": Ignore
        std::unique_ptr<SingleMarketDataFeed> m_orderCreationTimeMs;      // "O": Order creation time
        std::unique_ptr<SingleMarketDataFeed> m_cumulativeQuoteAssetQty;  // "Z": Cumulative quote asset transacted quantity
        std::unique_ptr<SingleMarketDataFeed> m_lastQuoteAssetQty;        // "Y": Last quote asset transacted quantity
        std::unique_ptr<SingleMarketDataFeed> m_quoteOrderQuantity;       // "Q": Quote Order Quantity
        std::unique_ptr<SingleMarketDataFeed> m_workingTimeMs;            // "W": Working Time
        std::unique_ptr<SingleMarketDataFeed> m_selfTradePreventionMode;  // "V": SelfTradePreventionMode

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const UserDataOrder& o);
	};

	class DLL_CLASS_MARKETDATA_EXPORTS FutureTradeData : public BinanceMarketData // future_trade_t
	{
	public:
		// Rule of 5, for class containing unique pointers
		FutureTradeData();
		FutureTradeData(const FutureTradeData& other);
		FutureTradeData& operator=(const FutureTradeData& other);
		FutureTradeData(FutureTradeData&& other) noexcept = default;
		FutureTradeData& operator=(FutureTradeData&& other) noexcept = default;
		~FutureTradeData() = default;
		std::string ToString() override;

		std::unique_ptr<SingleMarketDataFeed> m_aggregatedTradeId;
		std::unique_ptr<SingleMarketDataFeed> m_price;
		std::unique_ptr<SingleMarketDataFeed> m_quantity;
		std::unique_ptr<SingleMarketDataFeed> m_firstTradeId;
		std::unique_ptr<SingleMarketDataFeed> m_lastTradeId;
		std::unique_ptr<SingleMarketDataFeed> m_tradeTimeMs;
		std::unique_ptr<SingleMarketDataFeed> m_isBuyerTheMarketMaker;
		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs;

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const FutureTradeData& o);
	};

	class DLL_CLASS_MARKETDATA_EXPORTS FutureBookData : public BinanceMarketData // future_book_t
	{
	public:
		// Rule of 5, for class containing unique pointers
		FutureBookData();
		FutureBookData(const FutureBookData& other);
		FutureBookData& operator=(const FutureBookData& other);
		FutureBookData(FutureBookData&& other) noexcept = default;
		FutureBookData& operator=(FutureBookData&& other) noexcept = default;
		~FutureBookData() = default;
		std::string ToString() override;

		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs;
		std::unique_ptr<SingleMarketDataFeed> m_transactionTimeMs;
		std::unique_ptr<SingleMarketDataFeed> m_firstUpdateId;
		std::unique_ptr<SingleMarketDataFeed> m_finalUpdateId;
		std::unique_ptr<ArrayMarketDataFeed<DepthData>> m_bids;
		std::unique_ptr<ArrayMarketDataFeed<DepthData>> m_asks;

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const FutureBookData& o);
	};
};