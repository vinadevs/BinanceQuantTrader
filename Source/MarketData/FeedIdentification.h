#pragma once

#include "dlldefine.h"

#include <iostream>
#include <string>
#include <type_traits>
#include <variant>
#include <array>

namespace MarketData
{

	// new enum types
	enum class IndividualBookTickerID : unsigned
	{
		BEST_BID_PRICE,
		BEST_BID_QUANTITY,
		BEST_ASK_PRICE,
		BEST_ASK_QUANTITY,
		EVENT_TIME_MS
	};

	enum class TradeID : unsigned
	{
		TRADE_ID,
		PRICE,
		QUANTITY,
		BUYER_ORDER_ID,
		SELLER_ORDER_ID,
		TRADE_TIME,
		IS_BUYER_MARKET_MAKER
	};

	enum class IndividualMarketTickerID : unsigned
	{
		PRICE_CHANGE,
		PRICE_CHANGE_PERCENT,
		WEIGHTED_AVERAGE_PRICE,
		FIRST_TRADE,
		LAST_PRICE,
		LAST_QUANTITY,
		BEST_BID_PRICE,
		BEST_BID_QUANTITY,
		BEST_ASK_PRICE,
		BEST_ASK_QUANTITY,
		OPEN_PRICE,
		HIGH_PRICE,
		LOW_PRICE,
		TOTAL_TRADED_BASE_ASSET_VOLUME,
		TOTAL_TRADED_QUOTE_ASSET_VOLUME,
		STATISTICS_OPEN_TIME,
		STATISTICS_CLOSE_TIME,
		FIRST_TRADE_ID,
		LAST_TRADE_ID,
		TOTAL_NUMBER_OF_TRADES,
		EVENT_TIME_MS
	};

	enum class AllMarketTickersID : unsigned
	{
		TICKERS,
		EVENT_TIME_MS
	};

	enum class IndividualMiniTickerID : unsigned
	{
		CLOSE_PRICE,
		OPEN_PRICE,
		HIGH_PRICE,
		LOW_PRICE,
		TOTAL_TRADED_BASE_ASSET_VOLUME,
		TOTAL_TRADED_QUOTE_ASSET_VOLUME,
		EVENT_TIME_MS
	};

	enum class AllMiniTickersID : unsigned
	{
		TICKERS,
		EVENT_TIME_MS
	};

	enum class AggregateTradeID : unsigned
	{
		AGGREGATE_TRADE_ID,
		PRICE,
		QUANTITY,
		FIRST_TRADE_ID,
		LAST_TRADE_ID,
		TRADE_TIME,
		IS_BUYER_MARKET_MAKER,
		EVENT_TIME_MS
	};

	enum class KlineCandleStickID : unsigned
	{
		KLINE_START_TIME,
		KLINE_CLOSE_TIME,
		INTERVAL,
		FIRST_TRADE_ID,
		LAST_TRADE_ID,
		OPEN_PRICE,
		CLOSE_PRICE,
		HIGH_PRICE,
		LOW_PRICE,
		BASE_ASSET_VOLUME,
		NUMBER_OF_TRADES,
		IS_THIS_KLINE_CLOSED,
		QUOTE_ASSET_VOLUME,
		TAKER_BUY_BASE_ASSET_VOLUME,
		TAKER_BUY_QUOTE_ASSET_VOLUME,
		EVENT_TIME_MS
	};

	enum class PartDepthID : unsigned
	{
		PRICE,
		AMOUNT,
		EVENT_TIME_MS
	};

	enum class DiffDepthID : unsigned
	{
		PRICE,
		AMOUNT,
		EVENT_TIME_MS
	};

	inline constexpr std::ostream &operator<<(std::ostream &os, const IndividualBookTickerID id)
	{
		switch (id)
		{
			case IndividualBookTickerID::BEST_BID_PRICE: os << "BEST_BID_PRICE"; break;
			case IndividualBookTickerID::BEST_BID_QUANTITY: os << "BEST_BID_QUANTITY"; break;
			case IndividualBookTickerID::BEST_ASK_PRICE: os << "BEST_ASK_PRICE"; break;
			case IndividualBookTickerID::BEST_ASK_QUANTITY: os << "BEST_ASK_QUANTITY"; break;
			case IndividualBookTickerID::EVENT_TIME_MS: os << "EVENT_TIME_MS"; break;
			default: os << "UNKNOWN_ID"; break;
		}
		return os;
	}

	inline constexpr std::ostream &operator<<(std::ostream &os, const TradeID id)
	{
		switch (id)
		{
			case TradeID::TRADE_ID: os << "TRADE_ID"; break;
			case TradeID::PRICE: os << "PRICE"; break;
			case TradeID::QUANTITY: os << "QUANTITY"; break;
			case TradeID::BUYER_ORDER_ID: os << "BUYER_ORDER_ID"; break;
			case TradeID::SELLER_ORDER_ID: os << "SELLER_ORDER_ID"; break;
			case TradeID::TRADE_TIME: os << "TRADE_TIME"; break;
			case TradeID::IS_BUYER_MARKET_MAKER: os << "IS_BUYER_MARKET_MAKER"; break;
			default: os << "UNKNOWN_ID"; break;
		}
		return os;
	}

	inline constexpr std::ostream &operator<<(std::ostream &os, const IndividualMarketTickerID id)
	{
		switch (id)
		{
			case IndividualMarketTickerID::PRICE_CHANGE: os << "PRICE_CHANGE"; break;
			case IndividualMarketTickerID::PRICE_CHANGE_PERCENT: os << "PRICE_CHANGE_PERCENT"; break;
			case IndividualMarketTickerID::WEIGHTED_AVERAGE_PRICE: os << "WEIGHTED_AVERAGE_PRICE"; break;
			case IndividualMarketTickerID::FIRST_TRADE: os << "FIRST_TRADE"; break;
			case IndividualMarketTickerID::LAST_PRICE: os << "LAST_PRICE"; break;
			case IndividualMarketTickerID::LAST_QUANTITY: os << "LAST_QUANTITY"; break;
			case IndividualMarketTickerID::BEST_BID_PRICE: os << "BEST_BID_PRICE"; break;
			case IndividualMarketTickerID::BEST_BID_QUANTITY: os << "BEST_BID_QUANTITY"; break;
			case IndividualMarketTickerID::BEST_ASK_PRICE: os << "BEST_ASK_PRICE"; break;
			case IndividualMarketTickerID::BEST_ASK_QUANTITY: os << "BEST_ASK_QUANTITY"; break;
			case IndividualMarketTickerID::OPEN_PRICE: os << "OPEN_PRICE"; break;
			case IndividualMarketTickerID::HIGH_PRICE: os << "HIGH_PRICE"; break;
			case IndividualMarketTickerID::LOW_PRICE: os << "LOW_PRICE"; break;
			case IndividualMarketTickerID::TOTAL_TRADED_BASE_ASSET_VOLUME: os << "TOTAL_TRADED_BASE_ASSET_VOLUME"; break;
			case IndividualMarketTickerID::TOTAL_TRADED_QUOTE_ASSET_VOLUME: os << "TOTAL_TRADED_QUOTE_ASSET_VOLUME"; break;
			case IndividualMarketTickerID::STATISTICS_OPEN_TIME: os << "STATISTICS_OPEN_TIME"; break;
			case IndividualMarketTickerID::STATISTICS_CLOSE_TIME: os << "STATISTICS_CLOSE_TIME"; break;
			case IndividualMarketTickerID::FIRST_TRADE_ID: os << "FIRST_TRADE_ID"; break;
			case IndividualMarketTickerID::LAST_TRADE_ID: os << "LAST_TRADE_ID"; break;
			case IndividualMarketTickerID::TOTAL_NUMBER_OF_TRADES: os << "TOTAL_NUMBER_OF_TRADES"; break;
			default: os << "UNKNOWN_ID"; break;
		}
		return os;
	}
	inline constexpr std::ostream &operator<<(std::ostream &os, const AllMarketTickersID id)
	{
		switch (id)
		{
			case AllMarketTickersID::TICKERS: os << "TICKERS"; break;
			case AllMarketTickersID::EVENT_TIME_MS: os << "EVENT_TIME_MS"; break;
			default: os << "UNKNOWN_ID"; break;
		}
		return os;
	}
	inline constexpr std::ostream &operator<<(std::ostream &os, const IndividualMiniTickerID id)
	{
		switch (id)
		{
			case IndividualMiniTickerID::CLOSE_PRICE: os << "CLOSE_PRICE"; break;
			case IndividualMiniTickerID::OPEN_PRICE: os << "OPEN_PRICE"; break;
			case IndividualMiniTickerID::HIGH_PRICE: os << "HIGH_PRICE"; break;
			case IndividualMiniTickerID::LOW_PRICE: os << "LOW_PRICE"; break;
			case IndividualMiniTickerID::TOTAL_TRADED_BASE_ASSET_VOLUME: os << "TOTAL_TRADED_BASE_ASSET_VOLUME"; break;
			case IndividualMiniTickerID::TOTAL_TRADED_QUOTE_ASSET_VOLUME: os << "TOTAL_TRADED_QUOTE_ASSET_VOLUME"; break;
			default: os << "UNKNOWN_ID"; break;
		}
		return os;
	}
	inline constexpr std::ostream &operator<<(std::ostream &os, const AllMiniTickersID id)
	{
		switch (id)
		{
			case AllMiniTickersID::TICKERS: os << "TICKERS"; break;
			case AllMiniTickersID::EVENT_TIME_MS: os << "EVENT_TIME_MS"; break;
			default: os << "UNKNOWN_ID"; break;
		}
		return os;
	}
	inline constexpr std::ostream &operator<<(std::ostream &os, const AggregateTradeID id)
	{
		switch (id)
		{
			case AggregateTradeID::AGGREGATE_TRADE_ID: os << "AGGREGATE_TRADE_ID"; break;
			case AggregateTradeID::PRICE: os << "PRICE"; break;
			case AggregateTradeID::QUANTITY: os << "QUANTITY"; break;
			case AggregateTradeID::FIRST_TRADE_ID: os << "FIRST_TRADE_ID"; break;
			case AggregateTradeID::LAST_TRADE_ID: os << "LAST_TRADE_ID"; break;
			case AggregateTradeID::TRADE_TIME: os << "TRADE_TIME"; break;
			case AggregateTradeID::IS_BUYER_MARKET_MAKER: os << "IS_BUYER_MARKET_MAKER"; break;
			default: os << "UNKNOWN_ID"; break;
		}
		return os;
	}
	inline constexpr std::ostream &operator<<(std::ostream &os, const KlineCandleStickID id)
	{
		switch (id)
		{
			case KlineCandleStickID::KLINE_START_TIME: os << "KLINE_START_TIME"; break;
			case KlineCandleStickID::KLINE_CLOSE_TIME: os << "KLINE_CLOSE_TIME"; break;
			case KlineCandleStickID::INTERVAL: os << "INTERVAL"; break;
			case KlineCandleStickID::FIRST_TRADE_ID: os << "FIRST_TRADE_ID"; break;
			case KlineCandleStickID::LAST_TRADE_ID: os << "LAST_TRADE_ID"; break;
			case KlineCandleStickID::OPEN_PRICE: os << "OPEN_PRICE"; break;
			case KlineCandleStickID::CLOSE_PRICE: os << "CLOSE_PRICE"; break;
			case KlineCandleStickID::HIGH_PRICE: os << "HIGH_PRICE"; break;
			case KlineCandleStickID::LOW_PRICE: os << "LOW_PRICE"; break;
			case KlineCandleStickID::BASE_ASSET_VOLUME: os << "BASE_ASSET_VOLUME"; break;
			case KlineCandleStickID::NUMBER_OF_TRADES: os << "NUMBER_OF_TRADES"; break;
			case KlineCandleStickID::IS_THIS_KLINE_CLOSED: os << "IS_THIS_KLINE_CLOSED"; break;
			case KlineCandleStickID::QUOTE_ASSET_VOLUME: os << "QUOTE_ASSET_VOLUME"; break;
			case KlineCandleStickID::TAKER_BUY_BASE_ASSET_VOLUME: os << "TAKER_BUY_BASE_ASSET_VOLUME"; break;
			case KlineCandleStickID::TAKER_BUY_QUOTE_ASSET_VOLUME: os << "TAKER_BUY_QUOTE_ASSET_VOLUME"; break;
			default: os << "UNKNOWN_ID"; break;
		}
		return os;
	}

	inline constexpr std::ostream &operator<<(std::ostream &os, const PartDepthID id)
	{
		switch (id)
		{
			case PartDepthID::PRICE: os << "PRICE"; break;
			case PartDepthID::AMOUNT: os << "AMOUNT"; break;
			default: os << "UNKNOWN_ID"; break;
		}
		return os;
	}

	inline constexpr std::ostream &operator<<(std::ostream &os, const DiffDepthID id)
	{
		switch (id)
		{
			case DiffDepthID::PRICE: os << "PRICE"; break;
			case DiffDepthID::AMOUNT: os << "AMOUNT"; break;
			default: os << "UNKNOWN_ID"; break;
		}
		return os;
	}
};