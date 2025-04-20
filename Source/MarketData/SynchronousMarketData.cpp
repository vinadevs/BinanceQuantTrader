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
			<< "{\n"
			<< "\"updateId\": \"" << *o.m_updateId << "\",\n"
			<< "\"bestBidPrice\": \"" << *o.m_bestBidPrice << "\",\n"
			<< "\"bestBidQty\": \"" << *o.m_bestBidQty << "\",\n"
			<< "\"bestAskPrice\": \"" << *o.m_bestAskPrice << "\",\n"
			<< "\"bestAskQty\": \"" << *o.m_bestAskQty << "\",\n"
			<< "\"eventTimeMs\": \"" << *o.m_eventTimeMs << "\"\n"
			<< "}";

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
			<< "{\n"
			<< "\"tradeId\": \"" << *o.m_tradeId << "\",\n"
			<< "\"price\": \"" << *o.m_price << "\",\n"
			<< "\"quantity\": \"" << *o.m_quantity << "\",\n"
			<< "\"buyerOrderID\": \"" << *o.m_buyerOrderID << "\",\n"
			<< "\"sellerOrderID\": \"" << *o.m_sellerOrderID << "\",\n"
			<< "\"tradeTime\": \"" << *o.m_tradeTime << "\",\n"
			<< "\"isBuyerTheMarketMaker\": \"" << *o.m_isBuyerTheMarketMaker << "\",\n"
			<< "\"eventTimeMs\": \"" << *o.m_eventTimeMs << "\"\n"
			<< "}";
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
			<< "{\n"
			<< "\"priceChange\": \"" << *o.m_priceChange << "\",\n"
			<< "\"priceChangePercent\": \"" << *o.m_priceChangePercent << "\",\n"
			<< "\"weightedAvgPrice\": \"" << *o.m_weightedAvgPrice << "\",\n"
			<< "\"firstTradePrice\": \"" << *o.m_firstTradePrice << "\",\n"
			<< "\"lastPrice\": \"" << *o.m_lastPrice << "\",\n"
			<< "\"lastQuantity\": \"" << *o.m_lastQuantity << "\",\n"
			<< "\"bestBidPrice\": \"" << *o.m_bestBidPrice << "\",\n"
			<< "\"bestBidQty\": \"" << *o.m_bestBidQty << "\",\n"
			<< "\"bestAskPrice\": \"" << *o.m_bestAskPrice << "\",\n"
			<< "\"bestAskQty\": \"" << *o.m_bestAskQty << "\",\n"
			<< "\"openPrice\": \"" << *o.m_openPrice << "\",\n"
			<< "\"highPrice\": \"" << *o.m_highPrice << "\",\n"
			<< "\"lowPrice\": \"" << *o.m_lowPrice << "\",\n"
			<< "\"totalTradedBaseAssetVolume\": \"" << *o.m_totalTradedBaseAssetVolume << "\",\n"
			<< "\"totalTradedQuoteAssetVolume\": \"" << *o.m_totalTradedQuoteAssetVolume << "\",\n"
			<< "\"statisticsOpenTimeMs\": \"" << *o.m_statisticsOpenTimeMs << "\",\n"
			<< "\"statisticsCloseTimeMs\": \"" << *o.m_statisticsCloseTimeMs << "\",\n"
			<< "\"firstTradeId\": \"" << *o.m_firstTradeId << "\",\n"
			<< "\"lastTradeId\": \"" << *o.m_lastTradeId << "\",\n"
			<< "\"totalNumberOfTrades\": \"" << *o.m_totalNumberOfTrades << "\",\n"
			<< "\"eventTimeMs\": \"" << *o.m_eventTimeMs << "\"\n"
			<< "}";

		return os;
	}

	/********************************************************************************/
	// AllMarketTickersData
	/********************************************************************************/
	AllMarketTickerData::AllMarketTickerData()
		: m_allIndividualMarketTicker{ std::make_unique<TableMarketDataFeed<IndividualMarketTickerData>>() },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>() }
	{
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
			<< "{\n"
			<< "\"closePrice\": \"" << *o.m_closePrice << "\",\n"
			<< "\"openPrice\": \"" << *o.m_openPrice << "\",\n"
			<< "\"highPrice\": \"" << *o.m_highPrice << "\",\n"
			<< "\"lowPrice\": \"" << *o.m_lowPrice << "\",\n"
			<< "\"totalTradedBaseAssetVolume\": \"" << *o.m_totalTradedBaseAssetVolume << "\",\n"
			<< "\"totalTradedQuoteAssetVolume\": \"" << *o.m_totalTradedQuoteAssetVolume << "\",\n"
			<< "\"eventTimeMs\": \"" << *o.m_eventTimeMs << "\"\n"
			<< "}";
		return os;
	}

	/********************************************************************************/
	// AllMiniTickersData
	/********************************************************************************/
	AllMiniTickerData::AllMiniTickerData()
		: m_tableMarketTicker{ std::make_unique<TableMarketDataFeed<IndividualMiniTickerData>>() },
		m_eventTimeMs{ std::make_unique<SingleMarketDataFeed>() }
	{
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
			<< "{\n"
			<< "\"aggregateTradeId\": \"" << *o.m_aggregateTradeId << "\",\n"
			<< "\"price\": \"" << *o.m_price << "\",\n"
			<< "\"quantity\": \"" << *o.m_quantity << "\",\n"
			<< "\"firstTradeId\": \"" << *o.m_firstTradeId << "\",\n"
			<< "\"lastTradeId\": \"" << *o.m_lastTradeId << "\",\n"
			<< "\"totalNumberOfTrades\": \"" << *o.m_totalNumberOfTrades << "\",\n"
			<< "\"tradeTimeMs\": \"" << *o.m_tradeTimeMs << "\",\n"
			<< "\"isBuyerMarketMaker\": \"" << *o.m_isBuyerMarketMaker << "\",\n"
			<< "\"eventTimeMs\": \"" << *o.m_eventTimeMs << "\"\n"
			<< "}";
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
			<< "{\n"
			<< "\"klineStartTime\": \"" << *o.m_klineStartTime << "\",\n"
			<< "\"klineCloseTime\": \"" << *o.m_klineCloseTime << "\",\n"
			<< "\"interval\": \"" << *o.m_interval << "\",\n"
			<< "\"firstTradeId\": \"" << *o.m_firstTradeId << "\",\n"
			<< "\"lastTradeId\": \"" << *o.m_lastTradeId << "\",\n"
			<< "\"openPrice\": \"" << *o.m_openPrice << "\",\n"
			<< "\"closePrice\": \"" << *o.m_closePrice << "\",\n"
			<< "\"highPrice\": \"" << *o.m_highPrice << "\",\n"
			<< "\"lowPrice\": \"" << *o.m_lowPrice << "\",\n"
			<< "\"baseAssetVolume\": \"" << *o.m_baseAssetVolume << "\",\n"
			<< "\"numberOfTrades\": \"" << *o.m_numberOfTrades << "\",\n"
			<< "\"isThisKlineClosed\": \"" << *o.m_isThisKlineClosed << "\",\n"
			<< "\"quoteAssetVolume\": \"" << *o.m_quoteAssetVolume << "\",\n"
			<< "\"takerBuyBaseAssetVolume\": \"" << *o.m_takerBuyBaseAssetVolume << "\",\n"
			<< "\"takerBuyQuoteAssetVolume\": \"" << *o.m_takerBuyQuoteAssetVolume << "\",\n"
			<< "\"eventTimeMs\": \"" << *o.m_eventTimeMs << "\"\n"
			<< "}";
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
			<< "{\n"
			<< "\"price\": \"" << *o.m_price << "\",\n"
			<< "\"amount\": \"" << *o.m_amount << "\",\n"
			<< "\"eventTimeMs\": \"" << *o.m_eventTimeMs << "\"\n"
			<< "}";

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
