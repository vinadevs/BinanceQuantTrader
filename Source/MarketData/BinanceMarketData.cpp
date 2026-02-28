/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "BinanceMarketData.h"

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

	/********************************************************************************/
	// IndividualBookTickerData
	/********************************************************************************/
	IndividualBookTickerData::IndividualBookTickerData()
		: m_updateId{ std::make_unique<SingleMarketDataFeed>() },
		m_bestBidPrice{ std::make_unique<SingleMarketDataFeed>() },
		m_bestBidQty{ std::make_unique<SingleMarketDataFeed>() },
		m_bestAskPrice{ std::make_unique<SingleMarketDataFeed>() },
		m_bestAskQty{ std::make_unique<SingleMarketDataFeed>() },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>() }
	{
		m_dataName = "IndividualBookTickerData";
	}

	IndividualBookTickerData::IndividualBookTickerData(const IndividualBookTickerData& other)
		: m_updateId{ std::make_unique<SingleMarketDataFeed>(*other.m_updateId) },
		m_bestBidPrice{ std::make_unique<SingleMarketDataFeed>(*other.m_bestBidPrice) },
		m_bestBidQty{ std::make_unique<SingleMarketDataFeed>(*other.m_bestBidQty) },
		m_bestAskPrice{ std::make_unique<SingleMarketDataFeed>(*other.m_bestAskPrice) },
		m_bestAskQty{ std::make_unique<SingleMarketDataFeed>(*other.m_bestAskQty) },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs) }
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
			<< "EVENT_TIME_MS=" << o.m_eventTimeMs->GetStringDataFromEventTimeMs();
		return os;
	}

	std::string IndividualBookTickerData::ToString() {
		std::ostringstream oss;
		oss << *this;
		return oss.str();
	}

	/********************************************************************************/
	// TradeData
	/********************************************************************************/

	TradeData::TradeData()
		: m_price{ std::make_unique<SingleMarketDataFeed>() },
		m_quantity{ std::make_unique<SingleMarketDataFeed>() },
		m_tradeId{ std::make_unique<SingleMarketDataFeed>() },
		m_buyerOrderID{ std::make_unique<SingleMarketDataFeed>() },
		m_sellerOrderID{ std::make_unique<SingleMarketDataFeed>() },
		m_tradeTime{ std::make_unique<SingleMarketDataFeed>() },
		m_isBuyerTheMarketMaker{ std::make_unique<SingleMarketDataFeed>() },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>() }
	{
		m_dataName = "TradeData";
	}

	TradeData::TradeData(const TradeData& other)
		: m_price{ std::make_unique<SingleMarketDataFeed>(*other.m_price) },
		m_quantity{ std::make_unique<SingleMarketDataFeed>(*other.m_quantity) },
		m_tradeId{ std::make_unique<SingleMarketDataFeed>(*other.m_tradeId) },
		m_buyerOrderID{ std::make_unique<SingleMarketDataFeed>(*other.m_buyerOrderID) },
		m_sellerOrderID{ std::make_unique<SingleMarketDataFeed>(*other.m_sellerOrderID) },
		m_tradeTime{ std::make_unique<SingleMarketDataFeed>(*other.m_tradeTime) },
		m_isBuyerTheMarketMaker{ std::make_unique<SingleMarketDataFeed>(*other.m_isBuyerTheMarketMaker) },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs) }
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
			<< "TRADE_TIME=" << o.m_tradeTime->GetStringDataFromEventTimeMs() << "|"
			<< "IS_BUYER_MARKET_MAKER=" << *o.m_isBuyerTheMarketMaker << "|"
			<< "EVENT_TIME_MS=" << o.m_eventTimeMs->GetStringDataFromEventTimeMs();
		return os;
	}

	std::string TradeData::ToString() {
		std::ostringstream oss;
		oss << *this;
		return oss.str();
	}

	/********************************************************************************/
	// IndividualMarketTickerData
	/********************************************************************************/
	IndividualMarketTickerData::IndividualMarketTickerData()
		: m_priceChange{ std::make_unique<SingleMarketDataFeed>() },
		m_priceChangePercent{ std::make_unique<SingleMarketDataFeed>() },
		m_weightedAvgPrice{ std::make_unique<SingleMarketDataFeed>() },
		m_firstTradePrice{ std::make_unique<SingleMarketDataFeed>() },
		m_lastPrice{ std::make_unique<SingleMarketDataFeed>() },
		m_lastQuantity{ std::make_unique<SingleMarketDataFeed>() },
		m_bestBidPrice{ std::make_unique<SingleMarketDataFeed>() },
		m_bestBidQty{ std::make_unique<SingleMarketDataFeed>() },
		m_bestAskPrice{ std::make_unique<SingleMarketDataFeed>() },
		m_bestAskQty{ std::make_unique<SingleMarketDataFeed>() },
		m_openPrice{ std::make_unique<SingleMarketDataFeed>() },
		m_highPrice{ std::make_unique<SingleMarketDataFeed>() },
		m_lowPrice{ std::make_unique<SingleMarketDataFeed>() },
		m_totalTradedBaseAssetVolume{ std::make_unique<SingleMarketDataFeed>() },
		m_totalTradedQuoteAssetVolume{ std::make_unique<SingleMarketDataFeed>() },
		m_statisticsOpenTimeMs{ std::make_unique<SingleMarketDataFeed>() },
		m_statisticsCloseTimeMs{ std::make_unique<SingleMarketDataFeed>() },
		m_firstTradeId{ std::make_unique<SingleMarketDataFeed>() },
		m_lastTradeId{ std::make_unique<SingleMarketDataFeed>() },
		m_totalNumberOfTrades{ std::make_unique<SingleMarketDataFeed>() },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>() }
	{
		m_dataName = "IndividualMarketTickerData";
	}

	IndividualMarketTickerData::IndividualMarketTickerData(const IndividualMarketTickerData& other)
		: m_priceChange{ std::make_unique<SingleMarketDataFeed>(*other.m_priceChange) },
		m_priceChangePercent{ std::make_unique<SingleMarketDataFeed>(*other.m_priceChangePercent) },
		m_weightedAvgPrice{ std::make_unique<SingleMarketDataFeed>(*other.m_weightedAvgPrice) },
		m_firstTradePrice{ std::make_unique<SingleMarketDataFeed>(*other.m_firstTradePrice) },
		m_lastPrice{ std::make_unique<SingleMarketDataFeed>(*other.m_lastPrice) },
		m_lastQuantity{ std::make_unique<SingleMarketDataFeed>(*other.m_lastQuantity) },
		m_bestBidPrice{ std::make_unique<SingleMarketDataFeed>(*other.m_bestBidPrice) },
		m_bestBidQty{ std::make_unique<SingleMarketDataFeed>(*other.m_bestBidQty) },
		m_bestAskPrice{ std::make_unique<SingleMarketDataFeed>(*other.m_bestAskPrice) },
		m_bestAskQty{ std::make_unique<SingleMarketDataFeed>(*other.m_bestAskQty) },
		m_openPrice{ std::make_unique<SingleMarketDataFeed>(*other.m_openPrice) },
		m_highPrice{ std::make_unique<SingleMarketDataFeed>(*other.m_highPrice) },
		m_lowPrice{ std::make_unique<SingleMarketDataFeed>(*other.m_lowPrice) },
		m_totalTradedBaseAssetVolume{ std::make_unique<SingleMarketDataFeed>(*other.m_totalTradedBaseAssetVolume) },
		m_totalTradedQuoteAssetVolume{ std::make_unique<SingleMarketDataFeed>(*other.m_totalTradedQuoteAssetVolume) },
		m_statisticsOpenTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_statisticsOpenTimeMs) },
		m_statisticsCloseTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_statisticsCloseTimeMs) },
		m_firstTradeId{ std::make_unique<SingleMarketDataFeed>(*other.m_firstTradeId) },
		m_lastTradeId{ std::make_unique<SingleMarketDataFeed>(*other.m_lastTradeId) },
		m_totalNumberOfTrades{ std::make_unique<SingleMarketDataFeed>(*other.m_totalNumberOfTrades) },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs) }
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
			<< "STATISTICS_OPEN_TIME=" << o.m_statisticsOpenTimeMs->GetStringDataFromEventTimeMs() << "|"
			<< "STATISTICS_CLOSE_TIME=" << o.m_statisticsCloseTimeMs->GetStringDataFromEventTimeMs() << "|"
			<< "FIRST_TRADE_ID=" << *o.m_firstTradeId << "|"
			<< "LAST_TRADE_ID=" << *o.m_lastTradeId << "|"
			<< "TOTAL_NUMBER_OF_TRADES=" << *o.m_totalNumberOfTrades << "|"
			<< "EVENT_TIME_MS=" << o.m_eventTimeMs->GetStringDataFromEventTimeMs();
		return os;
	}

	std::string IndividualMarketTickerData::ToString() {
		std::ostringstream oss;
		oss << *this;
		return oss.str();
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
		os << "\"eventTimeMs\": \"" << o.m_eventTimeMs->GetStringDataFromEventTimeMs() << "\"\n";
		os << "}";

		return os;
	}

	std::string AllMarketTickerData::ToString() {
		std::ostringstream oss;
		oss << *this;
		return oss.str();
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
			<< "EVENT_TIME_MS=" << o.m_eventTimeMs->GetStringDataFromEventTimeMs();
		return os;
	}

	std::string IndividualMiniTickerData::ToString() {
		std::ostringstream oss;
		oss << *this;
		return oss.str();
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
		os << "\"eventTimeMs\": \"" << o.m_eventTimeMs->GetStringDataFromEventTimeMs() << "\"\n";
		os << "}";
		return os;
	}

	std::string AllMiniTickerData::ToString() {
		std::ostringstream oss;
		oss << *this;
		return oss.str();
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
			<< "TRADE_TIME_MS=" << o.m_tradeTimeMs->GetStringDataFromEventTimeMs() << "|"
			<< "IS_BUYER_MARKET_MAKER=" << *o.m_isBuyerMarketMaker << "|"
			<< "EVENT_TIME_MS=" << o.m_eventTimeMs->GetStringDataFromEventTimeMs();
		return os;
	}

	std::string AggregateTradeData::ToString() {
		std::ostringstream oss;
		oss << *this;
		return oss.str();
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
			<< "KLINE_START_TIME=" << o.m_klineStartTime->GetStringDataFromEventTimeMs() << "|"
			<< "KLINE_CLOSE_TIME=" << o.m_klineCloseTime->GetStringDataFromEventTimeMs() << "|"
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
			<< "EVENT_TIME_MS=" << o.m_eventTimeMs->GetStringDataFromEventTimeMs();
		return os;
	}

	std::string KlineCandleStickData::ToString() {
		std::ostringstream oss;
		oss << *this;
		return oss.str();
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
			<< "EVENT_TIME_MS=" << o.m_eventTimeMs->GetStringDataFromEventTimeMs();

		return os;
	}

	std::string DepthData::ToString() {
		std::ostringstream oss;
		oss << *this;
		return oss.str();
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

		os << "\"eventTimeMs\": \"" << o.m_eventTimeMs->GetStringDataFromEventTimeMs() << "\"\n";
		os << "}";

		return os;
	}

	std::string AllPartDepthData::ToString() {
		std::ostringstream oss;
		oss << *this;
		return oss.str();
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

		os << "\"eventTimeMs\": \"" << o.m_eventTimeMs->GetStringDataFromEventTimeMs() << "\"\n";
		os << "}";

		return os;
	}

	std::string AllDiffDepthData::ToString() {
		std::ostringstream oss;
		oss << *this;
		return oss.str();
	}

	/********************************************************************************/
	// UserDataAccount
	/********************************************************************************/
	
	UserDataAccount::UserDataAccount()
		: m_asset{ std::make_unique<SingleMarketDataFeed>() },
		m_free{ std::make_unique<SingleMarketDataFeed>() },
		m_locked{ std::make_unique<SingleMarketDataFeed>() },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>() },
		m_eventType{ std::make_unique<SingleMarketDataFeed>() },
		m_timeOfLastAccountUpdateMs{ std::make_unique<SingleMarketDataFeed>() }
	{
		m_dataName = "UserDataAccount";
	}

	UserDataAccount::UserDataAccount(const UserDataAccount& other)
		: m_asset{ std::make_unique<SingleMarketDataFeed>(*other.m_asset) },
		m_free{ std::make_unique<SingleMarketDataFeed>(*other.m_free) },
		m_locked{ std::make_unique<SingleMarketDataFeed>(*other.m_locked) },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs) },
		m_eventType{ std::make_unique<SingleMarketDataFeed>(*other.m_eventType) },
		m_timeOfLastAccountUpdateMs{ std::make_unique<SingleMarketDataFeed>(*other.m_timeOfLastAccountUpdateMs) }
	{
	}

	UserDataAccount& UserDataAccount::operator=(const UserDataAccount& other)
	{
		if (this != &other)
		{
			m_asset = std::make_unique<SingleMarketDataFeed>(*other.m_asset);
			m_free = std::make_unique<SingleMarketDataFeed>(*other.m_free);
			m_locked = std::make_unique<SingleMarketDataFeed>(*other.m_locked);
			m_eventTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs);
			m_eventType = std::make_unique<SingleMarketDataFeed>(*other.m_eventType);
			m_timeOfLastAccountUpdateMs = std::make_unique<SingleMarketDataFeed>(*other.m_timeOfLastAccountUpdateMs);
		}
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const UserDataAccount& o)
	{
		os
			<< "DATA_NAME=" << o.m_dataName << "|"
			<< "ASSET=" << *o.m_asset << "|"
			<< "FREE=" << *o.m_free << "|"
			<< "LOCKED=" << *o.m_locked << "|"
			<< "EVENT_TIME_MS=" << o.m_eventTimeMs->GetStringDataFromEventTimeMs() << "|"
			<< "EVENT_TYPE=" << *o.m_eventType << "|"
			<< "TIME_OF_LAST_ACCOUNT_UPDATE_MS=" << o.m_timeOfLastAccountUpdateMs->GetStringDataFromEventTimeMs();
		return os;
	}

	std::string UserDataAccount::ToString() {
		std::ostringstream oss;
		oss << *this;
		return oss.str();
	}

	/********************************************************************************/
	// UserDataBalance
	/********************************************************************************/

	UserDataBalance::UserDataBalance()
		: m_asset{ std::make_unique<SingleMarketDataFeed>() },
		m_balanceDelta{ std::make_unique<SingleMarketDataFeed>() },
		m_clearTimeMs{ std::make_unique<SingleMarketDataFeed>() },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>() },
		m_eventType{ std::make_unique<SingleMarketDataFeed>() }
	{
		m_dataName = "UserDataBalance";
	}

	UserDataBalance::UserDataBalance(const UserDataBalance& other)
		: m_asset{ std::make_unique<SingleMarketDataFeed>(*other.m_asset) },
		m_balanceDelta{ std::make_unique<SingleMarketDataFeed>(*other.m_balanceDelta) },
		m_clearTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_clearTimeMs) },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs) },
		m_eventType{ std::make_unique<SingleMarketDataFeed>(*other.m_eventType) }
	{
	}

	UserDataBalance& UserDataBalance::operator=(const UserDataBalance& other)
	{
		if (this != &other)
		{
			m_asset = std::make_unique<SingleMarketDataFeed>(*other.m_asset);
			m_balanceDelta = std::make_unique<SingleMarketDataFeed>(*other.m_balanceDelta);
			m_clearTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_clearTimeMs);
			m_eventTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs);
			m_eventType = std::make_unique<SingleMarketDataFeed>(*other.m_eventType);
		}
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const UserDataBalance& o)
	{
		os
			<< "DATA_NAME=" << o.m_dataName << "|"
			<< "ASSET=" << *o.m_asset << "|"
			<< "BALANCE_DELTA=" << *o.m_balanceDelta << "|"
			<< "CLEAR_TIME_MS=" << o.m_clearTimeMs->GetStringDataFromEventTimeMs() << "|"
			<< "EVENT_TIME_MS=" << o.m_eventTimeMs->GetStringDataFromEventTimeMs() << "|"
			<< "EVENT_TYPE=" << *o.m_eventType;
		return os;
	}

	std::string UserDataBalance::ToString() {
		std::ostringstream oss;
		oss << *this;
		return oss.str();
	}

	/********************************************************************************/
	// UserDataOrder
	/********************************************************************************/

	UserDataOrder::UserDataOrder()
		: m_eventType{ std::make_unique<SingleMarketDataFeed>() },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>() },
		m_symbol{ std::make_unique<SingleMarketDataFeed>() },
		m_clientOrderId{ std::make_unique<SingleMarketDataFeed>() },
		m_side{ std::make_unique<SingleMarketDataFeed>() },
		m_orderType{ std::make_unique<SingleMarketDataFeed>() },
		m_timeInForce{ std::make_unique<SingleMarketDataFeed>() },
		m_orderQuantity{ std::make_unique<SingleMarketDataFeed>() },
		m_orderPrice{ std::make_unique<SingleMarketDataFeed>() },
		m_stopPrice{ std::make_unique<SingleMarketDataFeed>() },
		m_icebergQuantity{ std::make_unique<SingleMarketDataFeed>() },
		m_orderListId{ std::make_unique<SingleMarketDataFeed>() },
		m_originalClientOrderId{ std::make_unique<SingleMarketDataFeed>() },
		m_currentExecutionType{ std::make_unique<SingleMarketDataFeed>() },
		m_currentOrderStatus{ std::make_unique<SingleMarketDataFeed>() },
		m_orderRejectReason{ std::make_unique<SingleMarketDataFeed>() },
		m_orderId{ std::make_unique<SingleMarketDataFeed>() },
		m_lastExecutedQuantity{ std::make_unique<SingleMarketDataFeed>() },
		m_cumulativeFilledQuantity{ std::make_unique<SingleMarketDataFeed>() },
		m_lastExecutedPrice{ std::make_unique<SingleMarketDataFeed>() },
		m_commissionAmount{ std::make_unique<SingleMarketDataFeed>() },
		m_commissionAsset{ std::make_unique<SingleMarketDataFeed>() },
		m_transactionTimeMs{ std::make_unique<SingleMarketDataFeed>() },
		m_tradeId{ std::make_unique<SingleMarketDataFeed>() },
		m_preventedMatchId{ std::make_unique<SingleMarketDataFeed>() },
		m_executionId{ std::make_unique<SingleMarketDataFeed>() },
		m_isOrderOnBook{ std::make_unique<SingleMarketDataFeed>() },
		m_isMakerSide{ std::make_unique<SingleMarketDataFeed>() },
		m_ignore{ std::make_unique<SingleMarketDataFeed>() },
		m_orderCreationTimeMs{ std::make_unique<SingleMarketDataFeed>() },
		m_cumulativeQuoteAssetQty{ std::make_unique<SingleMarketDataFeed>() },
		m_lastQuoteAssetQty{ std::make_unique<SingleMarketDataFeed>() },
		m_quoteOrderQuantity{ std::make_unique<SingleMarketDataFeed>() },
		m_workingTimeMs{ std::make_unique<SingleMarketDataFeed>() },
		m_selfTradePreventionMode{ std::make_unique<SingleMarketDataFeed>() }
	{
		m_dataName = "UserDataOrder";
	}

	UserDataOrder::UserDataOrder(const UserDataOrder& other)
		: m_eventType{ std::make_unique<SingleMarketDataFeed>(*other.m_eventType) },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs) },
		m_symbol{ std::make_unique<SingleMarketDataFeed>(*other.m_symbol) },
		m_clientOrderId{ std::make_unique<SingleMarketDataFeed>(*other.m_clientOrderId) },
		m_side{ std::make_unique<SingleMarketDataFeed>(*other.m_side) },
		m_orderType{ std::make_unique<SingleMarketDataFeed>(*other.m_orderType) },
		m_timeInForce{ std::make_unique<SingleMarketDataFeed>(*other.m_timeInForce) },
		m_orderQuantity{ std::make_unique<SingleMarketDataFeed>(*other.m_orderQuantity) },
		m_orderPrice{ std::make_unique<SingleMarketDataFeed>(*other.m_orderPrice) },
		m_stopPrice{ std::make_unique<SingleMarketDataFeed>(*other.m_stopPrice) },
		m_icebergQuantity{ std::make_unique<SingleMarketDataFeed>(*other.m_icebergQuantity) },
		m_orderListId{ std::make_unique<SingleMarketDataFeed>(*other.m_orderListId) },
		m_originalClientOrderId{ std::make_unique<SingleMarketDataFeed>(*other.m_originalClientOrderId) },
		m_currentExecutionType{ std::make_unique<SingleMarketDataFeed>(*other.m_currentExecutionType) },
		m_currentOrderStatus{ std::make_unique<SingleMarketDataFeed>(*other.m_currentOrderStatus) },
		m_orderRejectReason{ std::make_unique<SingleMarketDataFeed>(*other.m_orderRejectReason) },
		m_orderId{ std::make_unique<SingleMarketDataFeed>(*other.m_orderId) },
		m_lastExecutedQuantity{ std::make_unique<SingleMarketDataFeed>(*other.m_lastExecutedQuantity) },
		m_cumulativeFilledQuantity{ std::make_unique<SingleMarketDataFeed>(*other.m_cumulativeFilledQuantity) },
		m_lastExecutedPrice{ std::make_unique<SingleMarketDataFeed>(*other.m_lastExecutedPrice) },
		m_commissionAmount{ std::make_unique<SingleMarketDataFeed>(*other.m_commissionAmount) },
		m_commissionAsset{ std::make_unique<SingleMarketDataFeed>(*other.m_commissionAsset) },
		m_transactionTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_transactionTimeMs) },
		m_tradeId{ std::make_unique<SingleMarketDataFeed>(*other.m_tradeId) },
		m_preventedMatchId{ std::make_unique<SingleMarketDataFeed>(*other.m_preventedMatchId) },
		m_executionId{ std::make_unique<SingleMarketDataFeed>(*other.m_executionId) },
		m_isOrderOnBook{ std::make_unique<SingleMarketDataFeed>(*other.m_isOrderOnBook) },
		m_isMakerSide{ std::make_unique<SingleMarketDataFeed>(*other.m_isMakerSide) },
		m_ignore{ std::make_unique<SingleMarketDataFeed>(*other.m_ignore) },
		m_orderCreationTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_orderCreationTimeMs) },
		m_cumulativeQuoteAssetQty{ std::make_unique<SingleMarketDataFeed>(*other.m_cumulativeQuoteAssetQty) },
		m_lastQuoteAssetQty{ std::make_unique<SingleMarketDataFeed>(*other.m_lastQuoteAssetQty) },
		m_quoteOrderQuantity{ std::make_unique<SingleMarketDataFeed>(*other.m_quoteOrderQuantity) },
		m_workingTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_workingTimeMs) },
		m_selfTradePreventionMode{ std::make_unique<SingleMarketDataFeed>(*other.m_selfTradePreventionMode) }
	{
	}

	UserDataOrder& UserDataOrder::operator=(const UserDataOrder& other)
	{
		if (this != &other)
		{
			m_eventType = std::make_unique<SingleMarketDataFeed>(*other.m_eventType);
			m_eventTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs);
			m_symbol = std::make_unique<SingleMarketDataFeed>(*other.m_symbol);
			m_clientOrderId = std::make_unique<SingleMarketDataFeed>(*other.m_clientOrderId);
			m_side = std::make_unique<SingleMarketDataFeed>(*other.m_side);
			m_orderType = std::make_unique<SingleMarketDataFeed>(*other.m_orderType);
			m_timeInForce = std::make_unique<SingleMarketDataFeed>(*other.m_timeInForce);
			m_orderQuantity = std::make_unique<SingleMarketDataFeed>(*other.m_orderQuantity);
			m_orderPrice = std::make_unique<SingleMarketDataFeed>(*other.m_orderPrice);
			m_stopPrice = std::make_unique<SingleMarketDataFeed>(*other.m_stopPrice);
			m_icebergQuantity = std::make_unique<SingleMarketDataFeed>(*other.m_icebergQuantity);
			m_orderListId = std::make_unique<SingleMarketDataFeed>(*other.m_orderListId);
			m_originalClientOrderId = std::make_unique<SingleMarketDataFeed>(*other.m_originalClientOrderId);
			m_currentExecutionType = std::make_unique<SingleMarketDataFeed>(*other.m_currentExecutionType);
			m_currentOrderStatus = std::make_unique<SingleMarketDataFeed>(*other.m_currentOrderStatus);
			m_orderRejectReason = std::make_unique<SingleMarketDataFeed>(*other.m_orderRejectReason);
			m_orderId = std::make_unique<SingleMarketDataFeed>(*other.m_orderId);
			m_lastExecutedQuantity = std::make_unique<SingleMarketDataFeed>(*other.m_lastExecutedQuantity);
			m_cumulativeFilledQuantity = std::make_unique<SingleMarketDataFeed>(*other.m_cumulativeFilledQuantity);
			m_lastExecutedPrice = std::make_unique<SingleMarketDataFeed>(*other.m_lastExecutedPrice);
			m_commissionAmount = std::make_unique<SingleMarketDataFeed>(*other.m_commissionAmount);
			m_commissionAsset = std::make_unique<SingleMarketDataFeed>(*other.m_commissionAsset);
			m_transactionTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_transactionTimeMs);
			m_tradeId = std::make_unique<SingleMarketDataFeed>(*other.m_tradeId);
			m_preventedMatchId = std::make_unique<SingleMarketDataFeed>(*other.m_preventedMatchId);
			m_executionId = std::make_unique<SingleMarketDataFeed>(*other.m_executionId);
			m_isOrderOnBook = std::make_unique<SingleMarketDataFeed>(*other.m_isOrderOnBook);
			m_isMakerSide = std::make_unique<SingleMarketDataFeed>(*other.m_isMakerSide);
			m_ignore = std::make_unique<SingleMarketDataFeed>(*other.m_ignore);
			m_orderCreationTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_orderCreationTimeMs);
			m_cumulativeQuoteAssetQty = std::make_unique<SingleMarketDataFeed>(*other.m_cumulativeQuoteAssetQty);
			m_lastQuoteAssetQty = std::make_unique<SingleMarketDataFeed>(*other.m_lastQuoteAssetQty);
			m_quoteOrderQuantity = std::make_unique<SingleMarketDataFeed>(*other.m_quoteOrderQuantity);
			m_workingTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_workingTimeMs);
			m_selfTradePreventionMode = std::make_unique<SingleMarketDataFeed>(*other.m_selfTradePreventionMode);
		}
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const UserDataOrder& o)
	{
		os
			<< "DATA_NAME=" << o.m_dataName << "|"
			<< "EVENT_TYPE=" << *o.m_eventType << "|"
			<< "EVENT_TIME_MS=" << o.m_eventTimeMs->GetStringDataFromEventTimeMs() << "|"
			<< "SYMBOL=" << *o.m_symbol << "|"
			<< "CLIENT_ORDER_ID=" << *o.m_clientOrderId << "|"
			<< "SIDE=" << *o.m_side << "|"
			<< "ORDER_TYPE=" << *o.m_orderType << "|"
			<< "TIME_IN_FORCE=" << *o.m_timeInForce << "|"
			<< "ORDER_QUANTITY=" << *o.m_orderQuantity << "|"
			<< "ORDER_PRICE=" << *o.m_orderPrice << "|"
			<< "STOP_PRICE=" << *o.m_stopPrice << "|"
			<< "ICEBERG_QUANTITY=" << *o.m_icebergQuantity << "|"
			<< "ORDER_LIST_ID=" << *o.m_orderListId << "|"
			<< "ORIGINAL_CLIENT_ORDER_ID=" << *o.m_originalClientOrderId << "|"
			<< "CURRENT_EXECUTION_TYPE=" << *o.m_currentExecutionType << "|"
			<< "CURRENT_ORDER_STATUS=" << *o.m_currentOrderStatus << "|"
			<< "ORDER_REJECT_REASON=" << *o.m_orderRejectReason << "|"
			<< "ORDER_ID=" << *o.m_orderId << "|"
			<< "LAST_EXECUTED_QUANTITY=" << *o.m_lastExecutedQuantity << "|"
			<< "CUMULATIVE_FILLED_QUANTITY=" << *o.m_cumulativeFilledQuantity << "|"
			<< "LAST_EXECUTED_PRICE=" << *o.m_lastExecutedPrice << "|"
			<< "COMMISSION_AMOUNT=" << *o.m_commissionAmount << "|"
			<< "COMMISSION_ASSET=" << *o.m_commissionAsset << "|"
			<< "TRANSACTION_TIME_MS=" << o.m_transactionTimeMs->GetStringDataFromEventTimeMs() << "|"
			<< "TRADE_ID=" << *o.m_tradeId << "|"
			<< "PREVENTED_MATCH_ID=" << *o.m_preventedMatchId << "|"
			<< "EXECUTION_ID=" << *o.m_executionId << "|"
			<< "IS_ORDER_ON_BOOK=" << *o.m_isOrderOnBook << "|"
			<< "IS_MAKER_SIDE=" << *o.m_isMakerSide << "|"
			<< "IGNORE=" << *o.m_ignore << "|"
			<< "ORDER_CREATION_TIME_MS=" << o.m_orderCreationTimeMs->GetStringDataFromEventTimeMs() << "|"
			<< "CUMULATIVE_QUOTE_ASSET_QTY=" << *o.m_cumulativeQuoteAssetQty << "|"
			<< "LAST_QUOTE_ASSET_QTY=" << *o.m_lastQuoteAssetQty << "|"
			<< "QUOTE_ORDER_QUANTITY=" << *o.m_quoteOrderQuantity << "|"
			<< "WORKING_TIME_MS=" << o.m_workingTimeMs->GetStringDataFromEventTimeMs() << "|"
			<< "SELF_TRADE_PREVENTION_MODE=" << *o.m_selfTradePreventionMode;
		return os;
	}

	std::string UserDataOrder::ToString() {
		std::ostringstream oss;
		oss << *this;
		return oss.str();
	}

	/********************************************************************************/
	// FutureTradeData
	/********************************************************************************/

	FutureTradeData::FutureTradeData()
		: m_aggregatedTradeId{ std::make_unique<SingleMarketDataFeed>() },
		m_price{ std::make_unique<SingleMarketDataFeed>() },
		m_quantity{ std::make_unique<SingleMarketDataFeed>() },
		m_firstTradeId{ std::make_unique<SingleMarketDataFeed>() },
		m_lastTradeId{ std::make_unique<SingleMarketDataFeed>() },
		m_tradeTimeMs{ std::make_unique<SingleMarketDataFeed>() },
		m_isBuyerTheMarketMaker{ std::make_unique<SingleMarketDataFeed>() },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>() }
	{
		m_dataName = "FutureTradeData";
	}

	FutureTradeData::FutureTradeData(const FutureTradeData& other)
		: m_aggregatedTradeId{ std::make_unique<SingleMarketDataFeed>(*other.m_aggregatedTradeId) },
		m_price{ std::make_unique<SingleMarketDataFeed>(*other.m_price) },
		m_quantity{ std::make_unique<SingleMarketDataFeed>(*other.m_quantity) },
		m_firstTradeId{ std::make_unique<SingleMarketDataFeed>(*other.m_firstTradeId) },
		m_lastTradeId{ std::make_unique<SingleMarketDataFeed>(*other.m_lastTradeId) },
		m_tradeTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_tradeTimeMs) },
		m_isBuyerTheMarketMaker{ std::make_unique<SingleMarketDataFeed>(*other.m_isBuyerTheMarketMaker) },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs) }
	{
	}

	FutureTradeData& FutureTradeData::operator=(const FutureTradeData& other)
	{
		if (this != &other)
		{
			m_aggregatedTradeId = std::make_unique<SingleMarketDataFeed>(*other.m_aggregatedTradeId);
			m_price = std::make_unique<SingleMarketDataFeed>(*other.m_price);
			m_quantity = std::make_unique<SingleMarketDataFeed>(*other.m_quantity);
			m_firstTradeId = std::make_unique<SingleMarketDataFeed>(*other.m_firstTradeId);
			m_lastTradeId = std::make_unique<SingleMarketDataFeed>(*other.m_lastTradeId);
			m_tradeTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_tradeTimeMs);
			m_isBuyerTheMarketMaker = std::make_unique<SingleMarketDataFeed>(*other.m_isBuyerTheMarketMaker);
			m_eventTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs);
		}
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const FutureTradeData& o)
	{
		os
			<< "DATA_NAME=" << o.m_dataName << "|"
			<< "AGGREGATED_TRADE_ID=" << *o.m_aggregatedTradeId << "|"
			<< "PRICE=" << *o.m_price << "|"
			<< "QUANTITY=" << *o.m_quantity << "|"
			<< "FIRST_TRADE_ID=" << *o.m_firstTradeId << "|"
			<< "LAST_TRADE_ID=" << *o.m_lastTradeId << "|"
			<< "TRADE_TIME_MS=" << o.m_tradeTimeMs->GetStringDataFromEventTimeMs() << "|"
			<< "IS_BUYER_THE_MARKET_MAKER=" << *o.m_isBuyerTheMarketMaker << "|"
			<< "EVENT_TIME_MS=" << o.m_eventTimeMs->GetStringDataFromEventTimeMs();
		return os;
	}

	std::string FutureTradeData::ToString() {
		std::ostringstream oss;
		oss << *this;
		return oss.str();
	}

	/********************************************************************************/
	// FutureBookData
	/********************************************************************************/

	FutureBookData::FutureBookData()
		: m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>() },
		m_transactionTimeMs{ std::make_unique<SingleMarketDataFeed>() },
		m_firstUpdateId{ std::make_unique<SingleMarketDataFeed>() },
		m_finalUpdateId{ std::make_unique<SingleMarketDataFeed>() },
		m_bids{ std::make_unique<ArrayMarketDataFeed<DepthData>>() },
		m_asks{ std::make_unique<ArrayMarketDataFeed<DepthData>>() }
	{
		m_dataName = "FutureBookData";
	}

	FutureBookData::FutureBookData(const FutureBookData& other)
		: m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs) },
		m_transactionTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_transactionTimeMs) },
		m_firstUpdateId{ std::make_unique<SingleMarketDataFeed>(*other.m_firstUpdateId) },
		m_finalUpdateId{ std::make_unique<SingleMarketDataFeed>(*other.m_finalUpdateId) },
		m_bids{ std::make_unique<ArrayMarketDataFeed<DepthData>>(*other.m_bids) },
		m_asks{ std::make_unique<ArrayMarketDataFeed<DepthData>>(*other.m_asks) }
	{
	}

	FutureBookData& FutureBookData::operator=(const FutureBookData& other)
	{
		if (this != &other)
		{
			m_eventTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs);
			m_transactionTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_transactionTimeMs);
			m_firstUpdateId = std::make_unique<SingleMarketDataFeed>(*other.m_firstUpdateId);
			m_finalUpdateId = std::make_unique<SingleMarketDataFeed>(*other.m_finalUpdateId);
			m_bids = std::make_unique<ArrayMarketDataFeed<DepthData>>(*other.m_bids);
			m_asks = std::make_unique<ArrayMarketDataFeed<DepthData>>(*other.m_asks);
		}
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const FutureBookData& o)
	{
		os << "DATA_NAME=" << o.m_dataName << "|\n";
		os << "{\n";
		os << "\"eventTimeMs\": \"" << o.m_eventTimeMs->GetStringDataFromEventTimeMs() << "\",\n";
		os << "\"transactionTimeMs\": \"" << o.m_transactionTimeMs->GetStringDataFromEventTimeMs() << "\",\n";
		os << "\"firstUpdateID\": \"" << *o.m_firstUpdateId << "\",\n";
		os << "\"finalUpdateID\": \"" << *o.m_finalUpdateId << "\",\n";
		// bids
		os << "\"bids\": ";
		PrintArrayMarktData(o.m_bids.get(), os);
		os << ",\n";
		// asks
		os << "\"asks\": ";
		PrintArrayMarktData(o.m_asks.get(), os);
		os << "\n}";
		return os;
	}

	std::string FutureBookData::ToString() {
		std::ostringstream oss;
		oss << *this;
		return oss.str();
	}

	/********************************************************************************/
	// FutureFundingData
	/********************************************************************************/

	FutureFundingData::FutureFundingData()
		: m_fundingRate{ std::make_unique<SingleMarketDataFeed>() },
		m_fundingTimeMs{ std::make_unique<SingleMarketDataFeed>() },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>() }
	{
		m_dataName = "FutureFundingData";
	}

	FutureFundingData::FutureFundingData(const FutureFundingData& other)
		: m_fundingRate{ std::make_unique<SingleMarketDataFeed>(*other.m_fundingRate) },
		m_fundingTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_fundingTimeMs) },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs) }
	{
	}

	FutureFundingData& FutureFundingData::operator=(const FutureFundingData& other)
	{
		if (this != &other)
		{
			m_fundingRate = std::make_unique<SingleMarketDataFeed>(*other.m_fundingRate);
			m_fundingTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_fundingTimeMs);
			m_eventTimeMs = std::make_unique<SingleMarketDataFeed>(*other.m_eventTimeMs);
		}
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const FutureFundingData& o)
	{
		os
			<< "DATA_NAME=" << o.m_dataName << "|"
			<< "FUNDING_RATE=" << *o.m_fundingRate << "|"
			<< "FUNDING_TIME_MS=" << o.m_fundingTimeMs->GetStringDataFromEventTimeMs() << "|"
			<< "EVENT_TIME_MS=" << o.m_eventTimeMs->GetStringDataFromEventTimeMs();
		return os;
	}

	std::string FutureFundingData::ToString()
	{
		std::ostringstream oss;
		oss << *this;
		return oss.str();
	}
} // namespace MarketData