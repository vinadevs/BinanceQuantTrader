/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "SynchronousMarketData.h"

namespace MarketData {
	template <typename T>
	inline void PrintTableMarketData(const TableMarketDataFeed<T>* table, std::ostream& os, int indentationLevel)
	{
		os << "{";
		auto data = table->GetData();
		for (auto it = std::begin(data); it != std::end(data); ++it) {
			os << "\"" << it->first << "\": " << it->second;
			if (std::next(it) != std::end(data)) {
				os << ",";
			}
		}
		os << "}";
	}

	template <typename T>
	inline void PrintArrayMarktData(const ArrayMarketDataFeed<T>* arr, std::ostream& os)
	{
		os << "[";
		auto data = arr->GetData();
		for (auto it = std::begin(data); it != std::end(data); ++it) {
			os << *it;
			if (std::next(it) != std::end(data)) {
				os << ",";
			}
		}
		os << "]";
	}

	SynchronousMarketData::SynchronousMarketData(const std::string& symbol)
		: m_symbol(symbol) {
	}

	std::ostream& operator<<(std::ostream& os, const SynchronousMarketData& o)
	{
		os
			<< "{\n"
			<< "\"Symbol\": \"" << o.m_symbol << "\",\n"
			<< "}";

		return os;
	}

	/********************************************************************************/
	// IndividualBookTickerData
	/********************************************************************************/
	IndividualBookTickerData::IndividualBookTickerData()
		: m_updateId{std::make_unique<SingleMarketDataFeed>()},
		m_bestBidPrice{std::make_unique<SingleMarketDataFeed>()},
		m_bestBidQty{std::make_unique<SingleMarketDataFeed>()},
		m_bestAskPrice{std::make_unique<SingleMarketDataFeed>()},
		m_bestAskQty{std::make_unique<SingleMarketDataFeed>()},
		m_eventTimeMs{std::make_unique<SingleMarketDataFeed>()}
	{
		m_dataName = "IndividualBookTickerData";
	}

	IndividualBookTickerData::IndividualBookTickerData(const IndividualBookTickerData& other)
		: m_updateId{std::make_unique<SingleMarketDataFeed>(*other.m_updateId)},
		m_bestBidPrice{std::make_unique<SingleMarketDataFeed>(*other.m_bestBidPrice)},
		m_bestBidQty{std::make_unique<SingleMarketDataFeed>(*other.m_bestBidQty)},
		m_bestAskPrice{std::make_unique<SingleMarketDataFeed>(*other.m_bestAskPrice)},
		m_bestAskQty{std::make_unique<SingleMarketDataFeed>(*other.m_bestAskQty)},
		m_eventTimeMs{std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs)}
	{
	}

	IndividualBookTickerData& IndividualBookTickerData::operator=(const IndividualBookTickerData& other)
	{
		if (this != &other)
		{
			m_updateId = std::make_unique<SingleMarketDataFeed>(*other.m_updateId);
			m_bestBidPrice = std::make_unique<SingleMarketDataFeed>(*other.m_bestBidPrice);
			m_bestBidQty = std::make_unique<SingleMarketDataFeed>(*other.m_bestBidQty);
			m_bestAskPrice = std::make_unique<SingleMarketDataFeed>(*other.m_bestAskPrice);
			m_bestAskQty = std::make_unique<SingleMarketDataFeed>(*other.m_bestAskQty);
			m_eventTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs);
		}

		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const IndividualBookTickerData& o)
	{
		os
			<< "DATA_NAME=" << o.m_dataName << "|"
			<< "UPDATE_ID=" << *o.m_updateId << "|"
			<< "BEST_BID_PRICE=" << *o.m_bestBidPrice << "|"
			<< "BEST_BID_QTY=" << *o.m_bestBidQty << "|"
			<< "BEST_ASK_PRICE=" << *o.m_bestAskPrice << "|"
			<< "BEST_ASK_QTY=" << *o.m_bestAskQty << "|"
			<< "EVENT_TIME_MS=" << *o.m_eventTimeMs;
		return os;
	}



	/********************************************************************************/
	// TradeData
	/********************************************************************************/
	
	TradeData::TradeData()
		: m_price{std::make_unique<SingleMarketDataFeed>()},
		m_quantity{std::make_unique<SingleMarketDataFeed>()},
		m_tradeId{std::make_unique<SingleMarketDataFeed>()},
		m_buyerOrderID{std::make_unique<SingleMarketDataFeed>()},
		m_sellerOrderID{std::make_unique<SingleMarketDataFeed>()},
		m_tradeTime{std::make_unique<SingleMarketDataFeed>()},
		m_isBuyerTheMarketMaker{std::make_unique<SingleMarketDataFeed>()},
		m_eventTimeMs{std::make_unique<SingleMarketDataFeed>()}
	{
		m_dataName = "TradeData";
	}

	TradeData::TradeData(const TradeData& other)
		: m_price{std::make_unique<SingleMarketDataFeed>(*other.m_price)},
		m_quantity{std::make_unique<SingleMarketDataFeed>(*other.m_quantity)},
		m_tradeId{std::make_unique<SingleMarketDataFeed>(*other.m_tradeId)},
		m_buyerOrderID{std::make_unique<SingleMarketDataFeed>(*other.m_buyerOrderID)},
		m_sellerOrderID{std::make_unique<SingleMarketDataFeed>(*other.m_sellerOrderID)},
		m_tradeTime{std::make_unique<SingleMarketDataFeed>(*other.m_tradeTime)},
		m_isBuyerTheMarketMaker{std::make_unique<SingleMarketDataFeed>(*other.m_isBuyerTheMarketMaker)},
		m_eventTimeMs{std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs)}
	{
	}

	TradeData& TradeData::operator=(const TradeData& other)
	{
		if (this != &other)
		{
			m_price = std::make_unique<SingleMarketDataFeed>(*other.m_price);
			m_quantity = std::make_unique<SingleMarketDataFeed>(*other.m_quantity);
			m_tradeId = std::make_unique<SingleMarketDataFeed>(*other.m_tradeId);
			m_buyerOrderID = std::make_unique<SingleMarketDataFeed>(*other.m_buyerOrderID);
			m_sellerOrderID = std::make_unique<SingleMarketDataFeed>(*other.m_sellerOrderID);
			m_tradeTime = std::make_unique<SingleMarketDataFeed>(*other.m_tradeTime);
			m_isBuyerTheMarketMaker = std::make_unique<SingleMarketDataFeed>(*other.m_isBuyerTheMarketMaker);
			m_eventTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs);
		}

		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const TradeData& o)
	{
		os
			<< "DATA_NAME=" << o.m_dataName << "|"
			<< "TRADE_ID=" << *o.m_tradeId << "|"
			<< "PRICE=" << *o.m_price << "|"
			<< "QUANTITY=" << *o.m_quantity << "|"
			<< "BUYER_ORDER_ID=" << *o.m_buyerOrderID << "|"
			<< "SELLER_ORDER_ID=" << *o.m_sellerOrderID << "|"
			<< "TRADE_TIME=" << *o.m_tradeTime << "|"
			<< "IS_BUYER_MARKET_MAKER=" << *o.m_isBuyerTheMarketMaker << "|"
			<< "EVENT_TIME_MS=" << *o.m_eventTimeMs;
		return os;
	}



	/********************************************************************************/
	// IndividualMarketTickerData
	/********************************************************************************/
	IndividualMarketTickerData::IndividualMarketTickerData()
	: m_priceChange{std::make_unique<SingleMarketDataFeed>()},
		m_priceChangePercent{std::make_unique<SingleMarketDataFeed>()},
		m_weightedAvgPrice{std::make_unique<SingleMarketDataFeed>()},
		m_firstTradePrice{std::make_unique<SingleMarketDataFeed>()},
		m_lastPrice{std::make_unique<SingleMarketDataFeed>()},
		m_lastQuantity{std::make_unique<SingleMarketDataFeed>()},
		m_bestBidPrice{std::make_unique<SingleMarketDataFeed>()},
		m_bestBidQty{std::make_unique<SingleMarketDataFeed>()},
		m_bestAskPrice{std::make_unique<SingleMarketDataFeed>()},
		m_bestAskQty{std::make_unique<SingleMarketDataFeed>()},
		m_openPrice{std::make_unique<SingleMarketDataFeed>()},
		m_highPrice{std::make_unique<SingleMarketDataFeed>()},
		m_lowPrice{std::make_unique<SingleMarketDataFeed>()},
		m_totalTradedBaseAssetVolume{std::make_unique<SingleMarketDataFeed>()},
		m_totalTradedQuoteAssetVolume{std::make_unique<SingleMarketDataFeed>()},
		m_statisticsOpenTimeMs{std::make_unique<SingleMarketDataFeed>()},
		m_statisticsCloseTimeMs{std::make_unique<SingleMarketDataFeed>()},
		m_firstTradeId{std::make_unique<SingleMarketDataFeed>()},
		m_lastTradeId{std::make_unique<SingleMarketDataFeed>()},
		m_totalNumberOfTrades{std::make_unique<SingleMarketDataFeed>()},
		m_eventTimeMs{std::make_unique<SingleMarketDataFeed>()}
	{
		m_dataName = "IndividualMarketTickerData";
	}
		
	IndividualMarketTickerData::IndividualMarketTickerData(const IndividualMarketTickerData& other)
	: m_priceChange{std::make_unique<SingleMarketDataFeed>(*other.m_priceChange)},
		m_priceChangePercent{std::make_unique<SingleMarketDataFeed>(*other.m_priceChangePercent)},
		m_weightedAvgPrice{std::make_unique<SingleMarketDataFeed>(*other.m_weightedAvgPrice)},
		m_firstTradePrice{std::make_unique<SingleMarketDataFeed>(*other.m_firstTradePrice)},
		m_lastPrice{std::make_unique<SingleMarketDataFeed>(*other.m_lastPrice)},
		m_lastQuantity{std::make_unique<SingleMarketDataFeed>(*other.m_lastQuantity)},
		m_bestBidPrice{std::make_unique<SingleMarketDataFeed>(*other.m_bestBidPrice)},
		m_bestBidQty{std::make_unique<SingleMarketDataFeed>(*other.m_bestBidQty)},
		m_bestAskPrice{std::make_unique<SingleMarketDataFeed>(*other.m_bestAskPrice)},
		m_bestAskQty{std::make_unique<SingleMarketDataFeed>(*other.m_bestAskQty)},
		m_openPrice{std::make_unique<SingleMarketDataFeed>(*other.m_openPrice)},
		m_highPrice{std::make_unique<SingleMarketDataFeed>(*other.m_highPrice)},
		m_lowPrice{std::make_unique<SingleMarketDataFeed>(*other.m_lowPrice)},
		m_totalTradedBaseAssetVolume{std::make_unique<SingleMarketDataFeed>(*other.m_totalTradedBaseAssetVolume)},
		m_totalTradedQuoteAssetVolume{std::make_unique<SingleMarketDataFeed>(*other.m_totalTradedQuoteAssetVolume)},
		m_statisticsOpenTimeMs{std::make_unique<SingleMarketDataFeed>(*other.m_statisticsOpenTimeMs)},
		m_statisticsCloseTimeMs{std::make_unique<SingleMarketDataFeed>(*other.m_statisticsCloseTimeMs)},
		m_firstTradeId{std::make_unique<SingleMarketDataFeed>(*other.m_firstTradeId)},
		m_lastTradeId{std::make_unique<SingleMarketDataFeed>(*other.m_lastTradeId)},
		m_totalNumberOfTrades{std::make_unique<SingleMarketDataFeed>(*other.m_totalNumberOfTrades)},
		m_eventTimeMs{std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs)}
	{
	}

	IndividualMarketTickerData& IndividualMarketTickerData::operator=(const IndividualMarketTickerData& other)
	{
		if (this != &other)
		{
			m_priceChange = std::make_unique<SingleMarketDataFeed>(*other.m_priceChange);
			m_priceChangePercent = std::make_unique<SingleMarketDataFeed>(*other.m_priceChangePercent);
			m_weightedAvgPrice = std::make_unique<SingleMarketDataFeed>(*other.m_weightedAvgPrice);
			m_firstTradePrice = std::make_unique<SingleMarketDataFeed>(*other.m_firstTradePrice);
			m_lastPrice = std::make_unique<SingleMarketDataFeed>(*other.m_lastPrice);
			m_lastQuantity = std::make_unique<SingleMarketDataFeed>(*other.m_lastQuantity);
			m_bestBidPrice = std::make_unique<SingleMarketDataFeed>(*other.m_bestBidPrice);
			m_bestBidQty = std::make_unique<SingleMarketDataFeed>(*other.m_bestBidQty);
			m_bestAskPrice = std::make_unique<SingleMarketDataFeed>(*other.m_bestAskPrice);
			m_bestAskQty = std::make_unique<SingleMarketDataFeed>(*other.m_bestAskQty);
			m_openPrice = std::make_unique<SingleMarketDataFeed>(*other.m_openPrice);
			m_highPrice = std::make_unique<SingleMarketDataFeed>(*other.m_highPrice);
			m_lowPrice = std::make_unique<SingleMarketDataFeed>(*other.m_lowPrice);
			m_totalTradedBaseAssetVolume = std::make_unique<SingleMarketDataFeed>(*other.m_totalTradedBaseAssetVolume);
			m_totalTradedQuoteAssetVolume = std::make_unique<SingleMarketDataFeed>(*other.m_totalTradedQuoteAssetVolume);
			m_statisticsOpenTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_statisticsOpenTimeMs);
			m_statisticsCloseTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_statisticsCloseTimeMs);
			m_firstTradeId = std::make_unique<SingleMarketDataFeed>(*other.m_firstTradeId);
			m_lastTradeId = std::make_unique<SingleMarketDataFeed>(*other.m_lastTradeId);
			m_totalNumberOfTrades = std::make_unique<SingleMarketDataFeed>(*other.m_totalNumberOfTrades);
			m_eventTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs);
		}

		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const IndividualMarketTickerData& o)
	{
		os
			<< "DATA_NAME=" << o.m_dataName << "|"
			<< "PRICE_CHANGE=" << *o.m_priceChange << "|"
			<< "PRICE_CHANGE_PERCENT=" << *o.m_priceChangePercent << "|"
			<< "WEIGHTED_AVERAGE_PRICE=" << *o.m_weightedAvgPrice << "|"
			<< "FIRST_TRADE_PRICE=" << *o.m_firstTradePrice << "|"
			<< "LAST_PRICE=" << *o.m_lastPrice << "|"
			<< "LAST_QUANTITY=" << *o.m_lastQuantity << "|"
			<< "BEST_BID_PRICE=" << *o.m_bestBidPrice << "|"
			<< "BEST_BID_QTY=" << *o.m_bestBidQty << "|"
			<< "BEST_ASK_PRICE=" << *o.m_bestAskPrice << "|"
			<< "BEST_ASK_QTY=" << *o.m_bestAskQty << "|"
			<< "OPEN_PRICE=" << *o.m_openPrice << "|"
			<< "HIGH_PRICE=" << *o.m_highPrice << "|"
			<< "LOW_PRICE=" << *o.m_lowPrice << "|"
			<< "TOTAL_TRADED_BASE_ASSET_VOLUME=" << *o.m_totalTradedBaseAssetVolume << "|"
			<< "TOTAL_TRADED_QUOTE_ASSET_VOLUME=" << *o.m_totalTradedQuoteAssetVolume << "|"
			<< "STATISTICS_OPEN_TIME=" << *o.m_statisticsOpenTimeMs << "|"
			<< "STATISTICS_CLOSE_TIME=" << *o.m_statisticsCloseTimeMs << "|"
			<< "FIRST_TRADE_ID=" << *o.m_firstTradeId << "|"
			<< "LAST_TRADE_ID=" << *o.m_lastTradeId << "|"
			<< "TOTAL_NUMBER_OF_TRADES=" << *o.m_totalNumberOfTrades << "|"
			<< "EVENT_TIME_MS=" << *o.m_eventTimeMs;
		return os;
	}

	/********************************************************************************/
	// AllMarketTickersData
	/********************************************************************************/
	AllMarketTickerData::AllMarketTickerData()
		: m_allIndividualMarketTicker{ std::make_unique<TableMarketDataFeed<IndividualMarketTickerData>>() },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>() }
	{
		m_dataName = "AllMarketTickerData";
	}

	AllMarketTickerData::AllMarketTickerData(const AllMarketTickerData& other)
		: m_allIndividualMarketTicker{ std::make_unique<TableMarketDataFeed<IndividualMarketTickerData>>(*other.m_allIndividualMarketTicker) },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs) }
	{
	}

	AllMarketTickerData& AllMarketTickerData::operator=(const AllMarketTickerData& other)
	{
		if (this != &other)
		{
			m_allIndividualMarketTicker = std::make_unique<TableMarketDataFeed<IndividualMarketTickerData>>(*other.m_allIndividualMarketTicker);
			m_eventTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs);
		}

		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const AllMarketTickerData& o)
	{
		os << "DATA_NAME=" << o.m_dataName << "\n";
		os << "{\n";
		os << "[\n";

		auto data = o.m_allIndividualMarketTicker->GetData();

		for (auto it = std::begin(data); it != std::end(data); ++it) {
			os << it->second;
			if (std::next(it) != std::end(data)) {
				os << ",";
			}
		}

		os << "],\n";
		os << "\"eventTimeMs\": \"" << *o.m_eventTimeMs << "\"\n";
		os << "}";

		return os;
	}

	/********************************************************************************/
	// IndividualMiniTickerData
	/********************************************************************************/
	IndividualMiniTickerData::IndividualMiniTickerData()
		: m_closePrice{ std::make_unique<SingleMarketDataFeed>() },
		m_openPrice{ std::make_unique<SingleMarketDataFeed>() },
		m_highPrice{ std::make_unique<SingleMarketDataFeed>() },
		m_lowPrice{ std::make_unique<SingleMarketDataFeed>() },
		m_totalTradedBaseAssetVolume{ std::make_unique<SingleMarketDataFeed>() },
		m_totalTradedQuoteAssetVolume{ std::make_unique<SingleMarketDataFeed>() },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>() }
	{
		m_dataName = "IndividualMiniTickerData";
	}

	IndividualMiniTickerData::IndividualMiniTickerData(const IndividualMiniTickerData& other)
		: m_closePrice{ std::make_unique<SingleMarketDataFeed>(*other.m_closePrice) },
		m_openPrice{ std::make_unique<SingleMarketDataFeed>(*other.m_openPrice) },
		m_highPrice{ std::make_unique<SingleMarketDataFeed>(*other.m_highPrice) },
		m_lowPrice{ std::make_unique<SingleMarketDataFeed>(*other.m_lowPrice) },
		m_totalTradedBaseAssetVolume{ std::make_unique<SingleMarketDataFeed>(*other.m_totalTradedBaseAssetVolume) },
		m_totalTradedQuoteAssetVolume{ std::make_unique<SingleMarketDataFeed>(*other.m_totalTradedQuoteAssetVolume) },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs) }
	{
	}

	IndividualMiniTickerData& IndividualMiniTickerData::operator=(const IndividualMiniTickerData& other)
	{
		if (this != &other)
		{
			m_closePrice = std::make_unique<SingleMarketDataFeed>(*other.m_closePrice);
			m_openPrice = std::make_unique<SingleMarketDataFeed>(*other.m_openPrice);
			m_highPrice = std::make_unique<SingleMarketDataFeed>(*other.m_highPrice);
			m_lowPrice = std::make_unique<SingleMarketDataFeed>(*other.m_lowPrice);
			m_totalTradedBaseAssetVolume = std::make_unique<SingleMarketDataFeed>(*other.m_totalTradedBaseAssetVolume);
			m_totalTradedQuoteAssetVolume = std::make_unique<SingleMarketDataFeed>(*other.m_totalTradedQuoteAssetVolume);
			m_eventTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs);
		}
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const IndividualMiniTickerData& o)
	{
		os
			<< "DATA_NAME=" << o.m_dataName << "|"
			<< "CLOSE_PRICE=" << *o.m_closePrice << "|"
			<< "OPEN_PRICE=" << *o.m_openPrice << "|"
			<< "HIGH_PRICE=" << *o.m_highPrice << "|"
			<< "LOW_PRICE=" << *o.m_lowPrice << "|"
			<< "TOTAL_TRADED_BASE_ASSET_VOLUME=" << *o.m_totalTradedBaseAssetVolume << "|"
			<< "TOTAL_TRADED_QUOTE_ASSET_VOLUME=" << *o.m_totalTradedQuoteAssetVolume << "|"
			<< "EVENT_TIME_MS=" << *o.m_eventTimeMs;
		return os;
	}

	/********************************************************************************/
	// AllMiniTickersData
	/********************************************************************************/
	AllMiniTickerData::AllMiniTickerData()
		: m_tableMarketTicker{ std::make_unique<TableMarketDataFeed<IndividualMiniTickerData>>() },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>() }
	{
		m_dataName = "AllMiniTickerData";
	}

	AllMiniTickerData::AllMiniTickerData(const AllMiniTickerData& other)
		: m_tableMarketTicker{ std::make_unique<TableMarketDataFeed<IndividualMiniTickerData>>(*other.m_tableMarketTicker) },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs) }
	{
	}

	AllMiniTickerData& AllMiniTickerData::operator=(const AllMiniTickerData& other)
	{
		if (this != &other)
		{
			m_tableMarketTicker = std::make_unique<TableMarketDataFeed<IndividualMiniTickerData>>(*other.m_tableMarketTicker);
			m_eventTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs);
		}
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const AllMiniTickerData& o)
	{
		os << "DATA_NAME=" << o.m_dataName << "\n";
		os << "{\n";
		os << "[\n";
		auto data = o.m_tableMarketTicker->GetData();
		for (auto it = std::begin(data); it != std::end(data); ++it) {
			os << it->second;
			if (std::next(it) != std::end(data)) {
				os << ",";
			}
		}
		os << "],\n";
		os << "\"eventTimeMs\": \"" << *o.m_eventTimeMs << "\"\n";
		os << "}";
		return os;
	}

	/********************************************************************************/
	// AggregateTradeData
	/********************************************************************************/
	AggregateTradeData::AggregateTradeData()
		: m_aggregateTradeId{ std::make_unique<SingleMarketDataFeed>() },
		m_price{ std::make_unique<SingleMarketDataFeed>() },
		m_quantity{ std::make_unique<SingleMarketDataFeed>() },
		m_firstTradeId{ std::make_unique<SingleMarketDataFeed>() },
		m_lastTradeId{ std::make_unique<SingleMarketDataFeed>() },
		m_totalNumberOfTrades{ std::make_unique<SingleMarketDataFeed>() },
		m_tradeTimeMs{ std::make_unique<SingleMarketDataFeed>() },
		m_isBuyerMarketMaker{ std::make_unique<SingleMarketDataFeed>() },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>() }
	{
		m_dataName = "AggregateTradeData";
	}

	AggregateTradeData::AggregateTradeData(const AggregateTradeData& other)
		: m_aggregateTradeId{ std::make_unique<SingleMarketDataFeed>(*other.m_aggregateTradeId) },
		m_price{ std::make_unique<SingleMarketDataFeed>(*other.m_price) },
		m_quantity{ std::make_unique<SingleMarketDataFeed>(*other.m_quantity) },
		m_firstTradeId{ std::make_unique<SingleMarketDataFeed>(*other.m_firstTradeId) },
		m_lastTradeId{ std::make_unique<SingleMarketDataFeed>(*other.m_lastTradeId) },
		m_totalNumberOfTrades{ std::make_unique<SingleMarketDataFeed>(*other.m_totalNumberOfTrades) },
		m_tradeTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_tradeTimeMs) },
		m_isBuyerMarketMaker{ std::make_unique<SingleMarketDataFeed>(*other.m_isBuyerMarketMaker) },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs) }
	{
	}

	AggregateTradeData& AggregateTradeData::operator=(const AggregateTradeData& other)
	{
		if (this != &other)
		{
			m_aggregateTradeId = std::make_unique<SingleMarketDataFeed>(*other.m_aggregateTradeId);
			m_price = std::make_unique<SingleMarketDataFeed>(*other.m_price);
			m_quantity = std::make_unique<SingleMarketDataFeed>(*other.m_quantity);
			m_firstTradeId = std::make_unique<SingleMarketDataFeed>(*other.m_firstTradeId);
			m_lastTradeId = std::make_unique<SingleMarketDataFeed>(*other.m_lastTradeId);
			m_totalNumberOfTrades = std::make_unique<SingleMarketDataFeed>(*other.m_totalNumberOfTrades);
			m_tradeTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_tradeTimeMs);
			m_isBuyerMarketMaker = std::make_unique<SingleMarketDataFeed>(*other.m_isBuyerMarketMaker);
			m_eventTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs);
		}
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const AggregateTradeData& o)
	{
		os
			<< "DATA_NAME=" << o.m_dataName << "|"
			<< "AGGREGATE_TRADE_ID=" << *o.m_aggregateTradeId << "|"
			<< "PRICE=" << *o.m_price << "|"
			<< "QUANTITY=" << *o.m_quantity << "|"
			<< "FIRST_TRADE_ID=" << *o.m_firstTradeId << "|"
			<< "LAST_TRADE_ID=" << *o.m_lastTradeId << "|"
			<< "TOTAL_NUMBER_OF_TRADES=" << *o.m_totalNumberOfTrades << "|"
			<< "TRADE_TIME_MS=" << *o.m_tradeTimeMs << "|"
			<< "IS_BUYER_MARKET_MAKER=" << *o.m_isBuyerMarketMaker << "|"
			<< "EVENT_TIME_MS=" << *o.m_eventTimeMs;
		return os;
	}



	/********************************************************************************/
	// KlineCandleStickData
	/********************************************************************************/
	KlineCandleStickData::KlineCandleStickData()
		: m_klineStartTime{ std::make_unique<SingleMarketDataFeed>() },
		m_klineCloseTime{ std::make_unique<SingleMarketDataFeed>() },
		m_interval{ std::make_unique<SingleMarketDataFeed>() },
		m_firstTradeId{ std::make_unique<SingleMarketDataFeed>() },
		m_lastTradeId{ std::make_unique<SingleMarketDataFeed>() },
		m_openPrice{ std::make_unique<SingleMarketDataFeed>() },
		m_closePrice{ std::make_unique<SingleMarketDataFeed>() },
		m_highPrice{ std::make_unique<SingleMarketDataFeed>() },
		m_lowPrice{ std::make_unique<SingleMarketDataFeed>() },
		m_baseAssetVolume{ std::make_unique<SingleMarketDataFeed>() },
		m_numberOfTrades{ std::make_unique<SingleMarketDataFeed>() },
		m_isThisKlineClosed{ std::make_unique<SingleMarketDataFeed>() },
		m_quoteAssetVolume{ std::make_unique<SingleMarketDataFeed>() },
		m_takerBuyBaseAssetVolume{ std::make_unique<SingleMarketDataFeed>() },
		m_takerBuyQuoteAssetVolume{ std::make_unique<SingleMarketDataFeed>() },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>() }
	{
		m_dataName = "KlineCandleStickData";
	}

	KlineCandleStickData::KlineCandleStickData(const KlineCandleStickData& other)
		: m_klineStartTime{ std::make_unique<SingleMarketDataFeed>(*other.m_klineStartTime) },
		m_klineCloseTime{ std::make_unique<SingleMarketDataFeed>(*other.m_klineCloseTime) },
		m_interval{ std::make_unique<SingleMarketDataFeed>(*other.m_interval) },
		m_firstTradeId{ std::make_unique<SingleMarketDataFeed>(*other.m_firstTradeId) },
		m_lastTradeId{ std::make_unique<SingleMarketDataFeed>(*other.m_lastTradeId) },
		m_openPrice{ std::make_unique<SingleMarketDataFeed>(*other.m_openPrice) },
		m_closePrice{ std::make_unique<SingleMarketDataFeed>(*other.m_closePrice) },
		m_highPrice{ std::make_unique<SingleMarketDataFeed>(*other.m_highPrice) },
		m_lowPrice{ std::make_unique<SingleMarketDataFeed>(*other.m_lowPrice) },
		m_baseAssetVolume{ std::make_unique<SingleMarketDataFeed>(*other.m_baseAssetVolume) },
		m_numberOfTrades{ std::make_unique<SingleMarketDataFeed>(*other.m_numberOfTrades) },
		m_isThisKlineClosed{ std::make_unique<SingleMarketDataFeed>(*other.m_isThisKlineClosed) },
		m_quoteAssetVolume{ std::make_unique<SingleMarketDataFeed>(*other.m_quoteAssetVolume) },
		m_takerBuyBaseAssetVolume{ std::make_unique<SingleMarketDataFeed>(*other.m_takerBuyBaseAssetVolume) },
		m_takerBuyQuoteAssetVolume{ std::make_unique<SingleMarketDataFeed>(*other.m_takerBuyQuoteAssetVolume) },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs) }
	{
	}

	KlineCandleStickData& KlineCandleStickData::operator=(const KlineCandleStickData& other)
	{
		if (this != &other)
		{
			m_klineStartTime = std::make_unique<SingleMarketDataFeed>(*other.m_klineStartTime);
			m_klineCloseTime = std::make_unique<SingleMarketDataFeed>(*other.m_klineCloseTime);
			m_interval = std::make_unique<SingleMarketDataFeed>(*other.m_interval);
			m_firstTradeId = std::make_unique<SingleMarketDataFeed>(*other.m_firstTradeId);
			m_lastTradeId = std::make_unique<SingleMarketDataFeed>(*other.m_lastTradeId);
			m_openPrice = std::make_unique<SingleMarketDataFeed>(*other.m_openPrice);
			m_closePrice = std::make_unique<SingleMarketDataFeed>(*other.m_closePrice);
			m_highPrice = std::make_unique<SingleMarketDataFeed>(*other.m_highPrice);
			m_lowPrice = std::make_unique<SingleMarketDataFeed>(*other.m_lowPrice);
			m_baseAssetVolume = std::make_unique<SingleMarketDataFeed>(*other.m_baseAssetVolume);
			m_numberOfTrades = std::make_unique<SingleMarketDataFeed>(*other.m_numberOfTrades);
			m_isThisKlineClosed = std::make_unique<SingleMarketDataFeed>(*other.m_isThisKlineClosed);
			m_quoteAssetVolume = std::make_unique<SingleMarketDataFeed>(*other.m_quoteAssetVolume);
			m_takerBuyBaseAssetVolume = std::make_unique<SingleMarketDataFeed>(*other.m_takerBuyBaseAssetVolume);
			m_takerBuyQuoteAssetVolume = std::make_unique<SingleMarketDataFeed>(*other.m_takerBuyQuoteAssetVolume);
			m_eventTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs);
		}
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const KlineCandleStickData& o)
	{
		os
			<< "DATA_NAME=" << o.m_dataName << "|"
			<< "KLINE_START_TIME=" << *o.m_klineStartTime << "|"
			<< "KLINE_CLOSE_TIME=" << *o.m_klineCloseTime << "|"
			<< "INTERVAL=" << *o.m_interval << "|"
			<< "FIRST_TRADE_ID=" << *o.m_firstTradeId << "|"
			<< "LAST_TRADE_ID=" << *o.m_lastTradeId << "|"
			<< "OPEN_PRICE=" << *o.m_openPrice << "|"
			<< "CLOSE_PRICE=" << *o.m_closePrice << "|"
			<< "HIGH_PRICE=" << *o.m_highPrice << "|"
			<< "LOW_PRICE=" << *o.m_lowPrice << "|"
			<< "BASE_ASSET_VOLUME=" << *o.m_baseAssetVolume << "|"
			<< "NUMBER_OF_TRADES=" << *o.m_numberOfTrades << "|"
			<< "IS_THIS_KLINE_CLOSED=" << *o.m_isThisKlineClosed << "|"
			<< "QUOTE_ASSET_VOLUME=" << *o.m_quoteAssetVolume << "|"
			<< "TAKER_BUY_BASE_ASSET_VOLUME=" << *o.m_takerBuyBaseAssetVolume << "|"
			<< "TAKER_BUY_QUOTE_ASSET_VOLUME=" << *o.m_takerBuyQuoteAssetVolume << "|"
			<< "EVENT_TIME_MS=" << *o.m_eventTimeMs;
		return os;
	}



	/********************************************************************************/
	// DepthData
	/********************************************************************************/
	DepthData::DepthData()
		: m_price{ std::make_unique<SingleMarketDataFeed>() },
		m_amount{ std::make_unique<SingleMarketDataFeed>() },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>() }
	{
		m_dataName = "DepthData";
	}

	DepthData::DepthData(const DepthData& other)
		: m_price{ std::make_unique<SingleMarketDataFeed>(*other.m_price) },
		m_amount{ std::make_unique<SingleMarketDataFeed>(*other.m_amount) },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs) }
	{
	}

	DepthData& DepthData::operator=(const DepthData& other)
	{
		if (this != &other)
		{
			m_price = std::make_unique<SingleMarketDataFeed>(*other.m_price);
			m_amount = std::make_unique<SingleMarketDataFeed>(*other.m_amount);
			m_eventTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs);
		}

		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const DepthData& o)
	{
		os
			<< "PRICE=" << *o.m_price << "|"
			<< "AMOUNT=" << *o.m_amount << "|"
			<< "EVENT_TIME_MS=" << *o.m_eventTimeMs;

		return os;
	}


	/********************************************************************************/
	// AllPartDepthData
	/********************************************************************************/

	AllPartDepthData::AllPartDepthData()
		: m_bids{ std::make_unique<ArrayMarketDataFeed<DepthData>>() },
		m_asks{ std::make_unique<ArrayMarketDataFeed<DepthData>>() },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>() }
	{
		m_dataName = "AllPartDepthData";
	}

	AllPartDepthData::AllPartDepthData(const AllPartDepthData& other)
		: m_bids{ std::make_unique<ArrayMarketDataFeed<DepthData>>(*other.m_bids) },
		m_asks{ std::make_unique<ArrayMarketDataFeed<DepthData>>(*other.m_asks) },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs) }
	{
	}

	AllPartDepthData& AllPartDepthData::operator=(const AllPartDepthData& other)
	{
		if (this != &other)
		{
			m_bids = std::make_unique<ArrayMarketDataFeed<DepthData>>(*other.m_bids);
			m_asks = std::make_unique<ArrayMarketDataFeed<DepthData>>(*other.m_asks);
			m_eventTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs);
		}

		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const AllPartDepthData& o)
	{
		os << "DATA_NAME=" << o.m_dataName << "|\n";
		os << "{\n";

		// bids
		os << "\"bids\": ";
		PrintArrayMarktData(o.m_bids.get(), os);
		os << ",\n";

		// asks
		os << "\"asks\": ";
		PrintArrayMarktData(o.m_asks.get(), os);
		os << ",\n";

		os << "\"eventTimeMs\": \"" << *o.m_eventTimeMs << "\"\n";
		os << "}";

		return os;
	}

	/********************************************************************************/
	// AllDiffDepthData
	/********************************************************************************/

	AllDiffDepthData::AllDiffDepthData()
		: m_firstUpdateID{ std::make_unique<SingleMarketDataFeed>() },
		m_finalUpdateID{ std::make_unique<SingleMarketDataFeed>() },
		m_bids{ std::make_unique<ArrayMarketDataFeed<DepthData>>() },
		m_asks{ std::make_unique<ArrayMarketDataFeed<DepthData>>() },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>() }
	{
		m_dataName = "AllDiffDepthData";
	}

	AllDiffDepthData::AllDiffDepthData(const AllDiffDepthData& other)
		: m_firstUpdateID{ std::make_unique<SingleMarketDataFeed>(*other.m_firstUpdateID) },
		m_finalUpdateID{ std::make_unique<SingleMarketDataFeed>(*other.m_finalUpdateID) },
		m_bids{ std::make_unique<ArrayMarketDataFeed<DepthData>>(*other.m_bids) },
		m_asks{ std::make_unique<ArrayMarketDataFeed<DepthData>>(*other.m_asks) },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs) }
	{
	}

	AllDiffDepthData& AllDiffDepthData::operator=(const AllDiffDepthData& other)
	{
		if (this != &other)
		{
			m_firstUpdateID = std::make_unique<SingleMarketDataFeed>(*other.m_firstUpdateID);
			m_finalUpdateID = std::make_unique<SingleMarketDataFeed>(*other.m_finalUpdateID);
			m_bids = std::make_unique<ArrayMarketDataFeed<DepthData>>(*other.m_bids);
			m_asks = std::make_unique<ArrayMarketDataFeed<DepthData>>(*other.m_asks);
			m_eventTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs);
		}

		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const AllDiffDepthData& o)
	{
		os << "DATA_NAME=" << o.m_dataName << "|\n";
		os << "{\n";

		os << "\"firstUpdateID\": \"" << *o.m_firstUpdateID << "\",\n";
		os << "\"finalUpdateID\": \"" << *o.m_finalUpdateID << "\",\n";

		// bids
		os << "\"bids\": ";
		PrintArrayMarktData(o.m_bids.get(), os);
		os << ",\n";

		// asks
		os << "\"asks\": ";
		PrintArrayMarktData(o.m_asks.get(), os);
		os << ",\n";

		os << "\"eventTimeMs\": \"" << *o.m_eventTimeMs << "\"\n";
		os << "}";

		return os;
	}
}
