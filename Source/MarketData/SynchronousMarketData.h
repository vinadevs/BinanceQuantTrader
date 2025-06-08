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

#include <iostream>
#include <memory>
#include <string>
#include <map>

#pragma warning(disable : 4251)
#pragma warning(disable : 4267)
#pragma warning(disable : 4715)

namespace MarketData {
	// Level 2 market data: Bid Ask price, quantity
	class DLL_CLASS_MARKETDATA_EXPORTS IndividualBookTickerData
	{
	public:
		// Rule of 5, for class containing unique pointers
		IndividualBookTickerData();
		IndividualBookTickerData(const IndividualBookTickerData& other);
		IndividualBookTickerData& operator=(const IndividualBookTickerData& other);
		IndividualBookTickerData(IndividualBookTickerData&& other) noexcept = default;
		IndividualBookTickerData& operator=(IndividualBookTickerData&& other) noexcept = default;
		~IndividualBookTickerData() = default;

		std::unique_ptr<SingleMarketDataFeed> m_updateId;
		std::unique_ptr<SingleMarketDataFeed> m_bestBidPrice;
		std::unique_ptr<SingleMarketDataFeed> m_bestBidQty;
		std::unique_ptr<SingleMarketDataFeed> m_bestAskPrice;
		std::unique_ptr<SingleMarketDataFeed> m_bestAskQty;
		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs; // There is no event time from Binance API, this is internal update time

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const IndividualBookTickerData& o);
	};

	// Level 1 market data: other trading data: Kline, VWAP, Volume,...
	class DLL_CLASS_MARKETDATA_EXPORTS TradeData // trade_t
	{
	public:
		// Rule of 5, for class containing unique pointers
		TradeData();
		TradeData(const TradeData& other);
		TradeData& operator=(const TradeData& other);
		TradeData(TradeData&& other) noexcept = default;
		TradeData& operator=(TradeData&& other) noexcept = default;
		~TradeData() = default;

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

	class DLL_CLASS_MARKETDATA_EXPORTS IndividualMarketTickerData // market_ticker_t
	{
	public:
		// Rule of 5, for class containing unique pointers
		IndividualMarketTickerData();
		IndividualMarketTickerData(const IndividualMarketTickerData& other);
		IndividualMarketTickerData& operator=(const IndividualMarketTickerData& other);
		IndividualMarketTickerData(IndividualMarketTickerData&& other) noexcept = default;
		IndividualMarketTickerData& operator=(IndividualMarketTickerData&& other) noexcept = default;
		~IndividualMarketTickerData() = default;

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

	class DLL_CLASS_MARKETDATA_EXPORTS AllMarketTickerData
	{
	public:
		// Rule of 0
		AllMarketTickerData();
		AllMarketTickerData(const AllMarketTickerData& other);
		AllMarketTickerData& operator=(const AllMarketTickerData& other);
		AllMarketTickerData(AllMarketTickerData&& other) noexcept = default;
		AllMarketTickerData& operator=(AllMarketTickerData&& other) noexcept = default;
		~AllMarketTickerData() = default;

		std::unique_ptr <TableMarketDataFeed<IndividualMarketTickerData>> m_allIndividualMarketTicker;
		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs;

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const AllMarketTickerData& o);
	};

	class IndividualMiniTickerData
	{
	public:
		// Rule of 5, for class containing unique pointers
		IndividualMiniTickerData();
		IndividualMiniTickerData(const IndividualMiniTickerData& other);
		IndividualMiniTickerData& operator=(const IndividualMiniTickerData& other);
		IndividualMiniTickerData(IndividualMiniTickerData&& other) noexcept = default;
		IndividualMiniTickerData& operator=(IndividualMiniTickerData&& other) noexcept = default;
		~IndividualMiniTickerData() = default;

		std::unique_ptr<SingleMarketDataFeed> m_closePrice;
		std::unique_ptr<SingleMarketDataFeed> m_openPrice;
		std::unique_ptr<SingleMarketDataFeed> m_highPrice;
		std::unique_ptr<SingleMarketDataFeed> m_lowPrice;
		std::unique_ptr<SingleMarketDataFeed> m_totalTradedBaseAssetVolume;
		std::unique_ptr<SingleMarketDataFeed> m_totalTradedQuoteAssetVolume;
		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs;

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const IndividualMiniTickerData& o);
	};

	class DLL_CLASS_MARKETDATA_EXPORTS AllMiniTickerData
	{
	public:
		// Rule of 5, for class containing unique pointers
		AllMiniTickerData();
		AllMiniTickerData(const AllMiniTickerData& other);
		AllMiniTickerData& operator=(const AllMiniTickerData& other);
		AllMiniTickerData(AllMiniTickerData&& other) noexcept = default;
		AllMiniTickerData& operator=(AllMiniTickerData&& other) noexcept = default;
		~AllMiniTickerData() = default;

		std::unique_ptr <TableMarketDataFeed<IndividualMiniTickerData>> m_tableMarketTicker;
		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs;

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const AllMiniTickerData& o);
	};

	class DLL_CLASS_MARKETDATA_EXPORTS AggregateTradeData
	{
	public:
		// Rule of 5, for class containing unique pointers
		AggregateTradeData();
		AggregateTradeData(const AggregateTradeData& other);
		AggregateTradeData& operator=(const AggregateTradeData& other);
		AggregateTradeData(AggregateTradeData&& other) noexcept = default;
		AggregateTradeData& operator=(AggregateTradeData&& other) noexcept = default;
		~AggregateTradeData() = default;

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

	class DLL_CLASS_MARKETDATA_EXPORTS KlineCandleStickData
	{
	public:
		// Rule of 5, for class containing unique pointers
		KlineCandleStickData();
		KlineCandleStickData(const KlineCandleStickData& other);
		KlineCandleStickData& operator=(const KlineCandleStickData& other);
		KlineCandleStickData(KlineCandleStickData&& other) noexcept = default;
		KlineCandleStickData& operator=(KlineCandleStickData&& other) noexcept = default;
		~KlineCandleStickData() = default;

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

	class DLL_CLASS_MARKETDATA_EXPORTS DepthData
	{
	public:
		// Rule of 5, for class containing unique pointers
		DepthData();
		DepthData(const DepthData& other);
		DepthData& operator=(const DepthData& other);
		DepthData(DepthData&& other) noexcept = default;
		DepthData& operator=(DepthData&& other) noexcept = default;
		~DepthData() = default;

		std::unique_ptr<SingleMarketDataFeed> m_price;
		std::unique_ptr<SingleMarketDataFeed> m_amount;
		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs;

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const DepthData& o);
	};

	class DLL_CLASS_MARKETDATA_EXPORTS AllPartDepthData
	{
	public:
		// Rule of 5, for class containing unique pointers
		AllPartDepthData();
		AllPartDepthData(const AllPartDepthData& other);
		AllPartDepthData& operator=(const AllPartDepthData& other);
		AllPartDepthData(AllPartDepthData&& other) noexcept = default;
		AllPartDepthData& operator=(AllPartDepthData&& other) noexcept = default;
		~AllPartDepthData() = default;

		std::unique_ptr<ArrayMarketDataFeed<DepthData>> m_bids;
		std::unique_ptr<ArrayMarketDataFeed<DepthData>> m_asks;
		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs;

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const AllPartDepthData& o);
	};

	class DLL_CLASS_MARKETDATA_EXPORTS AllDiffDepthData
	{
	public:
		// Rule of 5, for class containing unique pointers
		AllDiffDepthData();
		AllDiffDepthData(const AllDiffDepthData& other);
		AllDiffDepthData& operator=(const AllDiffDepthData& other);
		AllDiffDepthData(AllDiffDepthData&& other) noexcept = default;
		AllDiffDepthData& operator=(AllDiffDepthData&& other) noexcept = default;
		~AllDiffDepthData() = default;

		std::unique_ptr<SingleMarketDataFeed> m_firstUpdateID;
		std::unique_ptr<SingleMarketDataFeed> m_finalUpdateID;
		std::unique_ptr <ArrayMarketDataFeed<DepthData>> m_bids;
		std::unique_ptr <ArrayMarketDataFeed<DepthData>> m_asks;
		std::unique_ptr<SingleMarketDataFeed> m_eventTimeMs;
		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const AllDiffDepthData& o);
	};

	///////////////////////////////////////////////////////////////

	class LevelOneMarketData
	{

	};

	class LevelTwoMarketData
	{

	};

	class LevelThreeMarketData
	{

	};

	///////////////////////////////////////////////////////////////
	// This class contains all of single feed data is available from the exchange
	class DLL_CLASS_MARKETDATA_EXPORTS SynchronousMarketData
	{
	public:
		SynchronousMarketData(const std::string& symbol);

		IndividualBookTickerData m_individualBookTickerData;
		TradeData m_tradeData;
		IndividualMarketTickerData m_individualMarketTickerData;
		AllMarketTickerData m_allMarketTickerData;
		IndividualMiniTickerData m_individualMiniTickerData;
		AllMiniTickerData m_allMiniTickerData;
		AggregateTradeData m_aggregateTradeData;
		KlineCandleStickData m_klineCandleStickData;
		AllPartDepthData m_allPartDepthData;
		AllDiffDepthData m_allDiffDepthData;

		template <typename FeedType>
		SingleMarketDataFeed* GetSingleFeed(const FeedType id) const;

		std::string GetSymbol() const { return m_symbol; }

		friend DLL_CLASS_MARKETDATA_EXPORTS std::ostream& operator<<(std::ostream& os, const SynchronousMarketData& o);
	protected:
		std::string m_symbol;
	};

	template <>
	inline SingleMarketDataFeed* SynchronousMarketData::GetSingleFeed(const IndividualBookTickerID id) const
	{
		switch (id)
		{
		case IndividualBookTickerID::BEST_BID_PRICE:
			return m_individualBookTickerData.m_bestBidPrice.get();
		case IndividualBookTickerID::BEST_BID_QUANTITY:
			return m_individualBookTickerData.m_bestBidQty.get();
		case IndividualBookTickerID::BEST_ASK_PRICE:
			return m_individualBookTickerData.m_bestAskPrice.get();
		case IndividualBookTickerID::BEST_ASK_QUANTITY:
			return m_individualBookTickerData.m_bestAskQty.get();
		case IndividualBookTickerID::EVENT_TIME_MS:
			return m_individualBookTickerData.m_eventTimeMs.get();
		default:
			break;
		}
		return nullptr;
	}

	template <>
	inline SingleMarketDataFeed* SynchronousMarketData::GetSingleFeed(const TradeID id) const
	{
		switch (id)
		{
		case TradeID::TRADE_ID:
			return m_tradeData.m_tradeId.get();
		case TradeID::PRICE:
			return m_tradeData.m_price.get();
		case TradeID::QUANTITY:
			return m_tradeData.m_quantity.get();
		case TradeID::BUYER_ORDER_ID:
			return m_tradeData.m_buyerOrderID.get();
		case TradeID::SELLER_ORDER_ID:
			return m_tradeData.m_sellerOrderID.get();
		case TradeID::TRADE_TIME:
			return m_tradeData.m_tradeTime.get();
		case TradeID::IS_BUYER_MARKET_MAKER:
			return m_tradeData.m_isBuyerTheMarketMaker.get();
		default:
			break;
		}
		return nullptr;
	}

	template <>
	inline SingleMarketDataFeed* SynchronousMarketData::GetSingleFeed(const IndividualMarketTickerID id) const
	{
		switch (id)
		{
		case IndividualMarketTickerID::PRICE_CHANGE:
			return m_individualMarketTickerData.m_priceChange.get();
		case IndividualMarketTickerID::PRICE_CHANGE_PERCENT:
			return m_individualMarketTickerData.m_priceChangePercent.get();
		case IndividualMarketTickerID::WEIGHTED_AVERAGE_PRICE:
			return m_individualMarketTickerData.m_weightedAvgPrice.get();
		case IndividualMarketTickerID::FIRST_TRADE:
			return m_individualMarketTickerData.m_firstTradePrice.get();
		case IndividualMarketTickerID::LAST_PRICE:
			return m_individualMarketTickerData.m_lastPrice.get();
		case IndividualMarketTickerID::LAST_QUANTITY:
			return m_individualMarketTickerData.m_lastQuantity.get();
		case IndividualMarketTickerID::BEST_BID_PRICE:
			return m_individualMarketTickerData.m_bestBidPrice.get();
		case IndividualMarketTickerID::BEST_BID_QUANTITY:
			return m_individualMarketTickerData.m_bestBidQty.get();
		case IndividualMarketTickerID::BEST_ASK_PRICE:
			return m_individualMarketTickerData.m_bestAskPrice.get();
		case IndividualMarketTickerID::BEST_ASK_QUANTITY:
			return m_individualMarketTickerData.m_bestAskQty.get();
		case IndividualMarketTickerID::OPEN_PRICE:
			return m_individualMarketTickerData.m_openPrice.get();
		case IndividualMarketTickerID::HIGH_PRICE:
			return m_individualMarketTickerData.m_highPrice.get();
		case IndividualMarketTickerID::LOW_PRICE:
			return m_individualMarketTickerData.m_lowPrice.get();
		case IndividualMarketTickerID::TOTAL_TRADED_BASE_ASSET_VOLUME:
			return m_individualMarketTickerData.m_totalTradedBaseAssetVolume.get();
		case IndividualMarketTickerID::TOTAL_TRADED_QUOTE_ASSET_VOLUME:
			return m_individualMarketTickerData.m_totalTradedQuoteAssetVolume.get();
		case IndividualMarketTickerID::STATISTICS_OPEN_TIME:
			return m_individualMarketTickerData.m_statisticsOpenTimeMs.get();
		case IndividualMarketTickerID::STATISTICS_CLOSE_TIME:
			return m_individualMarketTickerData.m_statisticsCloseTimeMs.get();
		case IndividualMarketTickerID::FIRST_TRADE_ID:
			return m_individualMarketTickerData.m_firstTradeId.get();
		case IndividualMarketTickerID::LAST_TRADE_ID:
			return m_individualMarketTickerData.m_lastTradeId.get();
		case IndividualMarketTickerID::TOTAL_NUMBER_OF_TRADES:
			return m_individualMarketTickerData.m_totalNumberOfTrades.get();
		case IndividualMarketTickerID::EVENT_TIME_MS:
			return m_individualMarketTickerData.m_eventTimeMs.get();
		default:
			break;
		}

		return nullptr;
	}

	template <>
	inline SingleMarketDataFeed* SynchronousMarketData::GetSingleFeed(const IndividualMiniTickerID id) const
	{
		switch (id)
		{
		case IndividualMiniTickerID::CLOSE_PRICE:
			return m_individualMiniTickerData.m_closePrice.get();
		case IndividualMiniTickerID::OPEN_PRICE:
			return m_individualMiniTickerData.m_openPrice.get();
		case IndividualMiniTickerID::HIGH_PRICE:
			return m_individualMiniTickerData.m_highPrice.get();
		case IndividualMiniTickerID::LOW_PRICE:
			return m_individualMiniTickerData.m_lowPrice.get();
		case IndividualMiniTickerID::TOTAL_TRADED_BASE_ASSET_VOLUME:
			return m_individualMiniTickerData.m_totalTradedBaseAssetVolume.get();
		case IndividualMiniTickerID::TOTAL_TRADED_QUOTE_ASSET_VOLUME:
			return m_individualMiniTickerData.m_totalTradedQuoteAssetVolume.get();
		default:
			break;
		}
		return nullptr;
	}

	template <>
	inline SingleMarketDataFeed* SynchronousMarketData::GetSingleFeed(const AggregateTradeID id) const
	{
		switch (id)
		{
		case AggregateTradeID::AGGREGATE_TRADE_ID:
			return m_aggregateTradeData.m_aggregateTradeId.get();
		case AggregateTradeID::PRICE:
			return m_aggregateTradeData.m_price.get();
		case AggregateTradeID::QUANTITY:
			return m_aggregateTradeData.m_quantity.get();
		case AggregateTradeID::FIRST_TRADE_ID:
			return m_aggregateTradeData.m_firstTradeId.get();
		case AggregateTradeID::LAST_TRADE_ID:
			return m_aggregateTradeData.m_lastTradeId.get();
		case AggregateTradeID::TRADE_TIME:
			return m_aggregateTradeData.m_tradeTimeMs.get();
		case AggregateTradeID::IS_BUYER_MARKET_MAKER:
			return m_aggregateTradeData.m_isBuyerMarketMaker.get();
		default:
			break;
		}
		return nullptr;
	}

	template <>
	inline SingleMarketDataFeed* SynchronousMarketData::GetSingleFeed(const KlineCandleStickID id) const
	{
		switch (id)
		{
		case KlineCandleStickID::KLINE_START_TIME:
			return m_klineCandleStickData.m_klineStartTime.get();
		case KlineCandleStickID::KLINE_CLOSE_TIME:
			return m_klineCandleStickData.m_klineCloseTime.get();
		case KlineCandleStickID::INTERVAL:
			return m_klineCandleStickData.m_interval.get();
		case KlineCandleStickID::FIRST_TRADE_ID:
			return m_klineCandleStickData.m_firstTradeId.get();
		case KlineCandleStickID::LAST_TRADE_ID:
			return m_klineCandleStickData.m_lastTradeId.get();
		case KlineCandleStickID::OPEN_PRICE:
			return m_klineCandleStickData.m_openPrice.get();
		case KlineCandleStickID::CLOSE_PRICE:
			return m_klineCandleStickData.m_closePrice.get();
		case KlineCandleStickID::HIGH_PRICE:
			return m_klineCandleStickData.m_highPrice.get();
		case KlineCandleStickID::LOW_PRICE:
			return m_klineCandleStickData.m_lowPrice.get();
		case KlineCandleStickID::BASE_ASSET_VOLUME:
			return m_klineCandleStickData.m_baseAssetVolume.get();
		case KlineCandleStickID::NUMBER_OF_TRADES:
			return m_klineCandleStickData.m_numberOfTrades.get();
		case KlineCandleStickID::IS_THIS_KLINE_CLOSED:
			return m_klineCandleStickData.m_isThisKlineClosed.get();
		case KlineCandleStickID::QUOTE_ASSET_VOLUME:
			return m_klineCandleStickData.m_quoteAssetVolume.get();
		case KlineCandleStickID::TAKER_BUY_BASE_ASSET_VOLUME:
			return m_klineCandleStickData.m_takerBuyBaseAssetVolume.get();
		case KlineCandleStickID::TAKER_BUY_QUOTE_ASSET_VOLUME:
			return m_klineCandleStickData.m_takerBuyQuoteAssetVolume.get();
		default:
			break;
		}
		return nullptr;
	}
};