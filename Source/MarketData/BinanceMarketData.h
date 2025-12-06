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

	// Class base for Binance Synchronous Market Data
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

		std::unique_ptr <TableMarketDataFeed<IndividualMarketTickerData>> m_allIndividualMarketTicker;
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

		std::unique_ptr <TableMarketDataFeed<IndividualMiniTickerData>> m_tableMarketTicker;
		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs;

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
		std::unique_ptr <ArrayMarketDataFeed<DepthData>> m_bids;
		std::unique_ptr <ArrayMarketDataFeed<DepthData>> m_asks;
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
		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const UserDataOrder& o);
	};
};