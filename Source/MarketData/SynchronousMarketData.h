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

#pragma warning(disable : 4251)
#pragma warning(disable : 4267)
#pragma warning(disable : 4715)

namespace MarketData {

	// Level 2 market data: Bid Ask price, quantity
	class IndividualBookTickerData
	{
	public:
		DECLARE_SINGLE_FEED_POINTER(m_updateId);
		DECLARE_SINGLE_FEED_POINTER(m_bestBidPrice);
		DECLARE_SINGLE_FEED_POINTER(m_bestBidQty);
		DECLARE_SINGLE_FEED_POINTER(m_bestAskPrice);
		DECLARE_SINGLE_FEED_POINTER(m_bestAskQty);

		friend std::ostream& operator<<(std::ostream& os, const IndividualBookTickerData& o)
		{
			os
				<< "{"
				<< "\"updateId\":\"" << o.m_updateId->GetUnsignedIntData() << "\","
				<< "\"bestBidPrice\":\"" << o.m_bestBidPrice->GetDoubleMultiprecisionData() << "\","
				<< "\"bestBidQt\":\"" << o.m_bestBidQty->GetDoubleMultiprecisionData() << "\","
				<< "\"bestAskPrice\":\"" << o.m_bestAskPrice->GetDoubleMultiprecisionData() << "\","
				<< "\"bestAskQty\":\"" << o.m_bestAskQty->GetDoubleMultiprecisionData() << "\""
				<< "}";

			return os;
		}
	};

	// Level 1 market data: other trading data: Kline, VWAP, Volume,...
	class TradeData
	{
	public:
		DECLARE_SINGLE_FEED_POINTER(m_evenTime);
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
				<< "\"bestBidPrice\":\"" << o.m_evenTime->GetUnsignedIntData() << "\","
				<< "\"bestBidQt\":\"" << o.m_tradeId->GetUnsignedIntData() << "\","
				<< "\"bestAskPrice\":\"" << o.m_price->GetDoubleMultiprecisionData() << "\","
				<< "\"bestAskQty\":\"" << o.m_quantity->GetDoubleMultiprecisionData() << "\","
				<< "\"bestAskPrice\":\"" << o.m_buyerOrderID->GetUnsignedIntData() << "\","
				<< "\"bestAskPrice\":\"" << o.m_sellerOrderID->GetUnsignedIntData() << "\","
				<< "\"bestAskPrice\":\"" << o.m_tradeTime->GetUnsignedIntData() << "\","
				<< "\"bestAskPrice\":\"" << o.m_isBuyerTheMarketMaker->GetBooleanData() << "\""
				<< "}";

			return os;
		}
	};

	class IndividualMarketTickerData
	{

	};

	class AllMarketTickersData
	{

	};

	class IndividualMiniTickerData
	{

	};

	class AllMiniTickersData
	{

	};

	class AggregateTradeData
	{

	};

	class KlineCandleStickData
	{

	};

	class PartDepthData
	{

	};

	class DiffDepthData
	{

	};

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

		SingleMarketDataFeed* GetFeed(const FeedID id) const;
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
};