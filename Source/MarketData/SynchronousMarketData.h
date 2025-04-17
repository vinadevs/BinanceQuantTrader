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

#include <iostream>
#include <memory>
#include <string>
#include <map>

#pragma warning(disable : 4251)
#pragma warning(disable : 4267)
#pragma warning(disable : 4715)

namespace MarketData {
	inline constexpr std::string_view indentation = "    ";
	// Level 2 market data: Bid Ask price, quantity
	class IndividualBookTickerData
	{
	public:
		DECLARE_SINGLE_FEED_POINTER(m_updateId);
		DECLARE_SINGLE_FEED_POINTER(m_eventTimeMs); // There is no event time from Binance API, this is internal update time
		DECLARE_SINGLE_FEED_POINTER(m_bestBidPrice);
		DECLARE_SINGLE_FEED_POINTER(m_bestBidQty);
		DECLARE_SINGLE_FEED_POINTER(m_bestAskPrice);
		DECLARE_SINGLE_FEED_POINTER(m_bestAskQty);

		friend std::ostream& operator<<(std::ostream& os, const IndividualBookTickerData& o)
		{
			os
				<< "{"
				<< "\"updateId\":\"" << o.m_updateId->GetUnsignedIntData() << "\","
				<< "\"eventTime\":\"" << o.m_eventTimeMs->GetUnsignedIntData() << "\","
				<< "\"bestBidPrice\":\"" << o.m_bestBidPrice->GetDoubleMultiprecisionData() << "\","
				<< "\"bestBidQt\":\"" << o.m_bestBidQty->GetDoubleMultiprecisionData() << "\","
				<< "\"bestAskPrice\":\"" << o.m_bestAskPrice->GetDoubleMultiprecisionData() << "\","
				<< "\"bestAskQty\":\"" << o.m_bestAskQty->GetDoubleMultiprecisionData() << "\""
				<< "}";

			return os;
		}
};

	// Level 1 market data: other trading data: Kline, VWAP, Volume,...
	class TradeData // trade_t
	{
	public:
		DECLARE_SINGLE_FEED_POINTER(m_eventTimeMs);
		DECLARE_SINGLE_FEED_POINTER(m_tradeId);
		DECLARE_SINGLE_FEED_POINTER(m_price);
		DECLARE_SINGLE_FEED_POINTER(m_quantity);
		DECLARE_SINGLE_FEED_POINTER(m_buyerOrderID);
		DECLARE_SINGLE_FEED_POINTER(m_sellerOrderID);
		DECLARE_SINGLE_FEED_POINTER(m_tradeTime);
		DECLARE_SINGLE_FEED_POINTER(m_isBuyerTheMarketMaker);

		friend std::ostream& operator<<(std::ostream& os, const TradeData& o)
		{
			os
				<< "{"
				<< indentation << "\"eventTime\":\"" << o.m_eventTimeMs->GetUnsignedIntData() << "\","
				<< indentation << "\"tradeId\":\"" << o.m_tradeId->GetUnsignedIntData() << "\","
				<< indentation << "\"price\":\"" << o.m_price->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"quantity\":\"" << o.m_quantity->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"buyerOrderID\":\"" << o.m_buyerOrderID->GetUnsignedIntData() << "\","
				<< indentation << "\"sellerOrderID\":\"" << o.m_sellerOrderID->GetUnsignedIntData() << "\","
				<< indentation << "\"tradeTime\":\"" << o.m_tradeTime->GetUnsignedIntData() << "\","
				<< indentation << "\"isBuyerTheMarketMaker\":\"" << o.m_isBuyerTheMarketMaker->GetBooleanData() << "\""
				<< "}";

			return os;
		}
	};

	class IndividualMarketTickerData // market_ticker_t
	{
	public:
		DECLARE_SINGLE_FEED_POINTER(m_eventTimeMs);
		DECLARE_SINGLE_FEED_POINTER(m_symbol);
		DECLARE_SINGLE_FEED_POINTER(m_priceChange);
		DECLARE_SINGLE_FEED_POINTER(m_priceChangePercent);
		DECLARE_SINGLE_FEED_POINTER(m_weightedAvgPrice);
		DECLARE_SINGLE_FEED_POINTER(m_firstTradePrice);
		DECLARE_SINGLE_FEED_POINTER(m_lastPrice);
		DECLARE_SINGLE_FEED_POINTER(m_lastQuantity);
		DECLARE_SINGLE_FEED_POINTER(m_bestBidPrice);
		DECLARE_SINGLE_FEED_POINTER(m_bestBidQty);
		DECLARE_SINGLE_FEED_POINTER(m_bestAskPrice);
		DECLARE_SINGLE_FEED_POINTER(m_bestAskQty);
		DECLARE_SINGLE_FEED_POINTER(m_openPrice);
		DECLARE_SINGLE_FEED_POINTER(m_highPrice);
		DECLARE_SINGLE_FEED_POINTER(m_lowPrice);
		DECLARE_SINGLE_FEED_POINTER(m_totalTradedBaseAssetVolume);
		DECLARE_SINGLE_FEED_POINTER(m_totalTradedQuoteAssetVolume);
		DECLARE_SINGLE_FEED_POINTER(m_statisticsOpenTimeMs);
		DECLARE_SINGLE_FEED_POINTER(m_statisticsCloseTimeMs);
		DECLARE_SINGLE_FEED_POINTER(m_firstTradeId);
		DECLARE_SINGLE_FEED_POINTER(m_lastTradeId);
		DECLARE_SINGLE_FEED_POINTER(m_totalNumberOfTrades);

		friend std::ostream& operator<<(std::ostream& os, const IndividualMarketTickerData& o)
		{
			os
				<< "{"
				<< indentation << "\"eventTime\":\"" << o.m_eventTimeMs->GetUnsignedIntData() << "\","
				<< indentation << "\"priceChange\":\"" << o.m_priceChange->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"priceChangePercent\":\"" << o.m_priceChangePercent->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"weightedAvgPrice\":\"" << o.m_weightedAvgPrice->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"firstTradePrice\":\"" << o.m_firstTradePrice->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"lastPrice\":\"" << o.m_lastPrice->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"lastQuantity\":\"" << o.m_lastQuantity->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"bestBidPrice\":\"" << o.m_bestBidPrice->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"bestBidQty\":\"" << o.m_bestBidQty->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"bestAskPrice\":\"" << o.m_bestAskPrice->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"bestAskQty\":\"" << o.m_bestAskQty->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"openPrice\":\"" << o.m_openPrice->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"highPrice\":\"" << o.m_highPrice->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"lowPrice\":\"" << o.m_lowPrice->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"totalTradedBaseAssetVolume\":\"" << o.m_totalTradedBaseAssetVolume->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"totalTradedQuoteAssetVolume\":\"" << o.m_totalTradedQuoteAssetVolume->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"statisticsOpenTime\":\"" << o.m_statisticsOpenTimeMs->GetUnsignedIntData() << "\","
				<< indentation << "\"statisticsCloseTime\":\"" << o.m_statisticsCloseTimeMs->GetUnsignedIntData() << "\","
				<< indentation << "\"firstTradeId\":\"" << o.m_firstTradeId->GetUnsignedIntData() << "\","
				<< indentation << "\"lastTradeId\":\"" << o.m_lastTradeId->GetUnsignedIntData() << "\","
				<< indentation << "\"totalNumberOfTrades\":\"" << o.m_totalNumberOfTrades->GetUnsignedIntData() << "\""
				<< "}";

			return os;
		}
	};

	class AllMarketTickersData
	{
	public:
		DECLARE_SINGLE_FEED_POINTER(m_eventTimeMs);

		// TODO: to check the correctness of the map key
		std::map<std::string, IndividualMarketTickerData> m_tickers; // map of symbol and its data

		friend std::ostream& operator<<(std::ostream& os, const AllMarketTickersData& o)
		{
			os
				<< "{"
				<< indentation << "\"eventTime\":\"" << o.m_eventTimeMs->GetUnsignedIntData() << "\","
				<< indentation << "\"tickers\":[";

			for (const auto& ticker : o.m_tickers)
			{
				os << indentation << indentation << ticker.second << ",";
			}

			if (!o.m_tickers.empty())
			{
				os.seekp(-1, os.cur); // remove last comma
			}

			os << indentation << "]";
			os << "}";

			return os;
		}
	};

	class IndividualMiniTickerData
	{
	public:
		DECLARE_SINGLE_FEED_POINTER(m_eventTimeMs);
		DECLARE_SINGLE_FEED_POINTER(m_closePrice);
		DECLARE_SINGLE_FEED_POINTER(m_openPrice);
		DECLARE_SINGLE_FEED_POINTER(m_highPrice);
		DECLARE_SINGLE_FEED_POINTER(m_lowPrice);
		DECLARE_SINGLE_FEED_POINTER(m_totalTradedBaseAssetVolume);
		DECLARE_SINGLE_FEED_POINTER(m_totalTradedQuoteAssetVolume);

		friend std::ostream& operator<<(std::ostream& os, const IndividualMiniTickerData& o)
		{
			os
				<< "{"
				<< indentation << "\"eventTime\":\"" << o.m_eventTimeMs->GetUnsignedIntData() << "\","
				<< indentation << "\"closePrice\":\"" << o.m_closePrice->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"openPrice\":\"" << o.m_openPrice->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"highPrice\":\"" << o.m_highPrice->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"lowPrice\":\"" << o.m_lowPrice->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"totalTradedBaseAssetVolume\":\"" << o.m_totalTradedBaseAssetVolume->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"totalTradedQuoteAssetVolume\":\"" << o.m_totalTradedQuoteAssetVolume->GetDoubleMultiprecisionData() << "\""
				<< "}";

			return os;
		}
	};

	class AllMiniTickersData
	{
	public:
		DECLARE_SINGLE_FEED_POINTER(m_eventTimeMs);

		// TODO: to check the correctness of the map key
		std::map<std::string, IndividualMiniTickerData> m_tickers; // map of symbol and its data

		friend std::ostream& operator<<(std::ostream& os, const AllMiniTickersData& o)
		{
			os
				<< "{"
				<< indentation << "\"eventTime\":\"" << o.m_eventTimeMs->GetUnsignedIntData() << "\","
				<< indentation << "\"tickers\":[";

			for (const auto& ticker : o.m_tickers)
			{
				os << indentation << indentation << ticker.second << ",";
			}

			if (!o.m_tickers.empty())
			{
				os.seekp(-1, os.cur); // remove last comma
			}

			os << indentation << "]";
			os << "}";

			return os;
		}
	};

	class AggregateTradeData
	{
	public:
		DECLARE_SINGLE_FEED_POINTER(m_eventTimeMs);
		DECLARE_SINGLE_FEED_POINTER(m_aggregateTradeId);
		DECLARE_SINGLE_FEED_POINTER(m_price);
		DECLARE_SINGLE_FEED_POINTER(m_quantity);
		DECLARE_SINGLE_FEED_POINTER(m_firstTradeId);
		DECLARE_SINGLE_FEED_POINTER(m_lastTradeId);
		DECLARE_SINGLE_FEED_POINTER(m_totalNumberOfTrades);
		DECLARE_SINGLE_FEED_POINTER(m_tradeTimeMs);
		DECLARE_SINGLE_FEED_POINTER(m_isBuyerMarketMaker);

		friend std::ostream& operator<<(std::ostream& os, const AggregateTradeData& o)
		{
			os
				<< "{"
				<< indentation << "\"eventTime\":\"" << o.m_eventTimeMs->GetUnsignedIntData() << "\","
				<< indentation << "\"aggregateTradeId\":\"" << o.m_aggregateTradeId->GetUnsignedIntData() << "\","
				<< indentation << "\"price\":\"" << o.m_price->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"quantity\":\"" << o.m_quantity->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"firstTradeId\":\"" << o.m_firstTradeId->GetUnsignedIntData() << "\","
				<< indentation << "\"lastTradeId\":\"" << o.m_lastTradeId->GetUnsignedIntData() << "\","
				<< indentation << "\"totalNumberOfTrades\":\"" << o.m_totalNumberOfTrades->GetUnsignedIntData() << "\","
				<< indentation << "\"tradeTime\":\"" << o.m_tradeTimeMs->GetUnsignedIntData() << "\","
				<< indentation << "\"isBuyerTheMarketMaker\":\"" << o.m_isBuyerMarketMaker->GetBooleanData() << "\""
				<< "}";

			return os;
		}
	};

	class KlineCandleStickData
	{
	public:
		DECLARE_SINGLE_FEED_POINTER(m_eventTimeMs);
		DECLARE_SINGLE_FEED_POINTER(m_klineStartTime);
		DECLARE_SINGLE_FEED_POINTER(m_klineCloseTime);
		DECLARE_SINGLE_FEED_POINTER(m_interval);
		DECLARE_SINGLE_FEED_POINTER(m_firstTradeId);
		DECLARE_SINGLE_FEED_POINTER(m_lastTradeId);
		DECLARE_SINGLE_FEED_POINTER(m_openPrice);
		DECLARE_SINGLE_FEED_POINTER(m_closePrice);
		DECLARE_SINGLE_FEED_POINTER(m_highPrice);
		DECLARE_SINGLE_FEED_POINTER(m_lowPrice);
		DECLARE_SINGLE_FEED_POINTER(m_baseAssetVolume);
		DECLARE_SINGLE_FEED_POINTER(m_numberOfTrades);
		DECLARE_SINGLE_FEED_POINTER(m_isThisKlineClosed);
		DECLARE_SINGLE_FEED_POINTER(m_quoteAssetVolume);
		DECLARE_SINGLE_FEED_POINTER(m_takerBuyBaseAssetVolume);
		DECLARE_SINGLE_FEED_POINTER(m_takerBuyQuoteAssetVolume);

		friend std::ostream& operator<<(std::ostream& os, const KlineCandleStickData& o)
		{
			os
				<< "{"
				<< indentation << "\"eventTime\":\"" << o.m_eventTimeMs->GetUnsignedIntData() << "\","
				<< indentation << "\"klineStartTime\":\"" << o.m_klineStartTime->GetUnsignedIntData() << "\","
				<< indentation << "\"klineCloseTime\":\"" << o.m_klineCloseTime->GetUnsignedIntData() << "\","
				// << indentation << "\"interval\":\"" << o.m_interval->GetStringData() << "\","
				<< indentation << "\"firstTradeId\":\"" << o.m_firstTradeId->GetUnsignedIntData() << "\","
				<< indentation << "\"lastTradeId\":\"" << o.m_lastTradeId->GetUnsignedIntData() << "\","
				<< indentation << "\"openPrice\":\"" << o.m_openPrice->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"closePrice\":\"" << o.m_closePrice->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"highPrice\":\"" << o.m_highPrice->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"lowPrice\":\"" << o.m_lowPrice->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"baseAssetVolume\":\"" << o.m_baseAssetVolume->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"numberOfTrades\":\"" << o.m_numberOfTrades->GetUnsignedIntData() << "\","
				<< indentation << "\"isKlineClosed\":\"" << o.m_isThisKlineClosed->GetBooleanData() << "\","
				<< indentation << "\"quoteAssetVolume\":\"" << o.m_quoteAssetVolume->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"takerBuyBaseAssetVolume\":\"" << o.m_takerBuyBaseAssetVolume->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"takerBuyQuoteAssetVolume\":\"" << o.m_takerBuyQuoteAssetVolume->GetDoubleMultiprecisionData()  << "\""
				<< "}";

			return os;
		}
	};

	class PartDepthData
	{
	public:
		DECLARE_SINGLE_FEED_POINTER(m_eventTimeMs);
		DECLARE_SINGLE_FEED_POINTER(m_price);
		DECLARE_SINGLE_FEED_POINTER(m_amount);

		friend std::ostream& operator<<(std::ostream& os, const PartDepthData& o)
		{
			os
				<< "{"
				<< indentation << "\"eventTime\":\"" << o.m_eventTimeMs->GetUnsignedIntData() << "\","
				<< indentation << "\"price\":\"" << o.m_price->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"amount\":\"" << o.m_amount->GetDoubleMultiprecisionData() << "\""
				<< "}";

			return os;
		}
	};

	class DiffDepthData
	{
	public:
		DECLARE_SINGLE_FEED_POINTER(m_eventTimeMs);
		DECLARE_SINGLE_FEED_POINTER(m_price);
		DECLARE_SINGLE_FEED_POINTER(m_amount);

		friend std::ostream& operator<<(std::ostream& os, const DiffDepthData& o)
		{
			os
				<< "{"
				<< indentation << "\"eventTime\":\"" << o.m_eventTimeMs->GetUnsignedIntData() << "\","
				<< indentation << "\"price\":\"" << o.m_price->GetDoubleMultiprecisionData() << "\","
				<< indentation << "\"amount\":\"" << o.m_amount->GetDoubleMultiprecisionData() << "\""
				<< "}";

			return os;
		}
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
		AllMarketTickersData m_allMarketTickersData;
		IndividualMiniTickerData m_individualMiniTickerData;
		AllMiniTickersData m_allMiniTickersData;
		AggregateTradeData m_aggregateTradeData;
		KlineCandleStickData m_klineCandleStickData;
		PartDepthData m_partDepthData;
		DiffDepthData m_diffDepthData;

		template <typename FeedType>
		SingleMarketDataFeed* GetFeed(const FeedType id) const;

		std::string GetSymbol() const { return m_symbol; }

		friend std::ostream& operator<<(std::ostream& os, const SynchronousMarketData& o)
		{
			os
				<< "{"
				<< "\"symbol\":\"" << o.m_symbol << "\","
				<< "}";

			return os;
		}
	protected:
		std::string m_symbol;
	};

	template <>
	inline SingleMarketDataFeed* SynchronousMarketData::GetFeed(const IndividualBookTickerID id) const
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
	inline SingleMarketDataFeed* SynchronousMarketData::GetFeed(const TradeID id) const
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
	inline SingleMarketDataFeed* SynchronousMarketData::GetFeed(const IndividualMarketTickerID id) const
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
	inline SingleMarketDataFeed* SynchronousMarketData::GetFeed(const AllMarketTickersID id) const
	{
		switch (id)
		{
		case AllMarketTickersID::TICKERS:
			return m_allMarketTickersData.m_tickers.begin()->second.m_eventTimeMs.get();
		case AllMarketTickersID::EVENT_TIME_MS:
			return m_allMarketTickersData.m_eventTimeMs.get();
		default:
			break;
		}
		return nullptr;
	}

	template <>
	inline SingleMarketDataFeed* SynchronousMarketData::GetFeed(const IndividualMiniTickerID id) const
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
	inline SingleMarketDataFeed* SynchronousMarketData::GetFeed(const AllMiniTickersID id) const
	{
		switch (id)
		{
		case AllMiniTickersID::TICKERS:
			return m_allMiniTickersData.m_tickers.begin()->second.m_eventTimeMs.get();
		case AllMiniTickersID::EVENT_TIME_MS:
			return m_allMiniTickersData.m_eventTimeMs.get();
		default:
			break;
		}
		return nullptr;
	}

	template <>
	inline SingleMarketDataFeed* SynchronousMarketData::GetFeed(const AggregateTradeID id) const
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
	inline SingleMarketDataFeed* SynchronousMarketData::GetFeed(const KlineCandleStickID id) const
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

	template <>
	inline SingleMarketDataFeed* SynchronousMarketData::GetFeed(const PartDepthID id) const
	{
		switch (id)
		{
		case PartDepthID::PRICE:
			return m_partDepthData.m_price.get();
		case PartDepthID::AMOUNT:
			return m_partDepthData.m_amount.get();
		case PartDepthID::EVENT_TIME_MS:
			return m_partDepthData.m_eventTimeMs.get();
		default:
			break;
		}
		return nullptr;
	}

	template <>
	inline SingleMarketDataFeed* SynchronousMarketData::GetFeed(const DiffDepthID id) const
	{
		switch (id)
		{
		case DiffDepthID::PRICE:
			return m_diffDepthData.m_price.get();
		case DiffDepthID::AMOUNT:
			return m_diffDepthData.m_amount.get();
		case DiffDepthID::EVENT_TIME_MS:
			return m_diffDepthData.m_eventTimeMs.get();
		default:
			break;
		}
		return nullptr;
	}
};