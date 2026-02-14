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

#include "BinanceMarketData.h"

#include <iostream>
#include <memory>
#include <string>
#include <map>

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

	class DLL_CLASS_MARKETDATA_EXPORTS SynchronousMarketData final
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
		UserDataAccount m_userDataAccount;
		UserDataBalance m_userDataBalance;
		UserDataOrder m_userDataOrder;
		FutureTradeData m_futureTradeData;
		FutureBookData m_futureBookData;

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