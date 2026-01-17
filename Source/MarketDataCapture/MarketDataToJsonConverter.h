/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include <string>

#include "../MiddlewareMQ/BqtJsonMessage.h"
#include "../MarketData/SynchronousMarketData.h"

#include "MarketDataJSONTags.h"

namespace MarketDataCapture {
	namespace PythonMessage {

		MiddlewareMQ::BqtJsonMessage IndividualBookTickerToJsonMessage(
			const MarketData::IndividualBookTickerData& bookTickerData, const std::string& symbol) {
			MiddlewareMQ::BqtJsonMessage message;
			message.AddTag(MarketDataCapture::FieldLabels::Header::Symbol, symbol);
			message.AddTag(MarketDataCapture::FieldLabels::Header::MessageType, "IndividualBookTicker");
			message.AddTag(MarketDataCapture::FieldLabels::IndividualBookTicker::BestBidPrice,
				bookTickerData.m_bestBidPrice->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::IndividualBookTicker::BestBidQuantity,
				bookTickerData.m_bestBidQty->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::IndividualBookTicker::BestAskPrice,
				bookTickerData.m_bestAskPrice->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::IndividualBookTicker::BestAskQuantity,
				bookTickerData.m_bestAskQty->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::Header::EventTimeMs,
				bookTickerData.m_eventTimeMs->GetStringDataFromEventTimeMs());
			return message;
		}

		MiddlewareMQ::BqtJsonMessage TradeToJsonMessage(
			const MarketData::TradeData& tradeData, const std::string& symbol) {
			MiddlewareMQ::BqtJsonMessage message;
			message.AddTag(MarketDataCapture::FieldLabels::Header::Symbol, symbol);
			message.AddTag(MarketDataCapture::FieldLabels::Header::MessageType, "Trade");
			message.AddTag(MarketDataCapture::FieldLabels::Trade::TradeId,
				tradeData.m_tradeId->GetStringDataFromStdUnsignedIntData());
			message.AddTag(MarketDataCapture::FieldLabels::Trade::Price,
				tradeData.m_price->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::Trade::Quantity,
				tradeData.m_quantity->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::Trade::BuyerOrderId,
				tradeData.m_buyerOrderID->GetStringDataFromStdUnsignedIntData());
			message.AddTag(MarketDataCapture::FieldLabels::Trade::SellerOrderId,
				tradeData.m_sellerOrderID->GetStringDataFromStdUnsignedIntData());
			message.AddTag(MarketDataCapture::FieldLabels::Trade::TradeTime,
				tradeData.m_tradeTime->GetStringDataFromEventTimeMs());
			message.AddTag(MarketDataCapture::FieldLabels::Trade::IsBuyerMarketMaker,
				tradeData.m_isBuyerTheMarketMaker->GetStringDataFromStdBoolData());
			message.AddTag(MarketDataCapture::FieldLabels::Header::EventTimeMs,
				tradeData.m_eventTimeMs->GetStringDataFromEventTimeMs());
			return message;
		}

		MiddlewareMQ::BqtJsonMessage IndividualMarketTickerToJsonMessage(
			const MarketData::IndividualMarketTickerData& marketTickerData, const std::string& symbol) {
			MiddlewareMQ::BqtJsonMessage message;
			message.AddTag(MarketDataCapture::FieldLabels::Header::Symbol, symbol);
			message.AddTag(MarketDataCapture::FieldLabels::Header::MessageType, "IndividualMarketTicker");
			message.AddTag(MarketDataCapture::FieldLabels::IndividualMarketTicker::PriceChange,
				marketTickerData.m_priceChange->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::IndividualMarketTicker::PriceChangePercent,
				marketTickerData.m_priceChangePercent->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::IndividualMarketTicker::WeightedAveragePrice,
				marketTickerData.m_weightedAvgPrice->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::IndividualMarketTicker::FirstTrade,
				marketTickerData.m_firstTradePrice->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::IndividualMarketTicker::LastPrice,
				marketTickerData.m_lastPrice->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::IndividualMarketTicker::LastQuantity,
				marketTickerData.m_lastQuantity->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::IndividualMarketTicker::BestBidPrice,
				marketTickerData.m_bestBidPrice->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::IndividualMarketTicker::BestBidQuantity,
				marketTickerData.m_bestBidQty->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::IndividualMarketTicker::BestAskPrice,
				marketTickerData.m_bestAskPrice->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::IndividualMarketTicker::BestAskQuantity,
				marketTickerData.m_bestAskQty->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::IndividualMarketTicker::OpenPrice,
				marketTickerData.m_openPrice->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::IndividualMarketTicker::HighPrice,
				marketTickerData.m_highPrice->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::IndividualMarketTicker::LowPrice,
				marketTickerData.m_lowPrice->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::Header::EventTimeMs,
				marketTickerData.m_eventTimeMs->GetStringDataFromEventTimeMs());
			return message;
		}

		MiddlewareMQ::BqtJsonMessage IndividualMiniTickerToJsonMessage(
			const MarketData::IndividualMiniTickerData& miniTickerData, const std::string& symbol) {
			MiddlewareMQ::BqtJsonMessage message;
			message.AddTag(MarketDataCapture::FieldLabels::Header::Symbol, symbol);
			message.AddTag(MarketDataCapture::FieldLabels::Header::MessageType, "MiniTicker");
			message.AddTag(MarketDataCapture::FieldLabels::IndividualMiniTicker::ClosePrice,
				miniTickerData.m_closePrice->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::IndividualMiniTicker::OpenPrice,
				miniTickerData.m_openPrice->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::IndividualMiniTicker::HighPrice,
				miniTickerData.m_highPrice->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::IndividualMiniTicker::LowPrice,
				miniTickerData.m_lowPrice->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::IndividualMiniTicker::TotalTradedBaseAssetVolume,
				miniTickerData.m_totalTradedBaseAssetVolume->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::IndividualMiniTicker::TotalTradedQuoteAssetVolume,
				miniTickerData.m_totalTradedQuoteAssetVolume->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::Header::EventTimeMs,
				miniTickerData.m_eventTimeMs->GetStringDataFromEventTimeMs());
			return message;
		}

		MiddlewareMQ::BqtJsonMessage AggregateTradeToJsonMessage(
			const MarketData::AggregateTradeData& aggregateTradeData, const std::string& symbol) {
			MiddlewareMQ::BqtJsonMessage message;
			message.AddTag(MarketDataCapture::FieldLabels::Header::Symbol, symbol);
			message.AddTag(MarketDataCapture::FieldLabels::Header::MessageType, "AggregateTrade");
			message.AddTag(MarketDataCapture::FieldLabels::AggregateTrade::AggregateTradeId,
				aggregateTradeData.m_aggregateTradeId->GetStringDataFromStdUnsignedIntData());
			message.AddTag(MarketDataCapture::FieldLabels::AggregateTrade::Price,
				aggregateTradeData.m_price->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::AggregateTrade::Quantity,
				aggregateTradeData.m_quantity->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::AggregateTrade::FirstTradeId,
				aggregateTradeData.m_firstTradeId->GetStringDataFromStdUnsignedIntData());
			message.AddTag(MarketDataCapture::FieldLabels::AggregateTrade::LastTradeId,
				aggregateTradeData.m_lastTradeId->GetStringDataFromStdUnsignedIntData());
			message.AddTag(MarketDataCapture::FieldLabels::AggregateTrade::TradeTime,
				aggregateTradeData.m_tradeTimeMs->GetStringDataFromEventTimeMs());
			message.AddTag(MarketDataCapture::FieldLabels::AggregateTrade::IsBuyerMarketMaker,
				aggregateTradeData.m_isBuyerMarketMaker->GetStringDataFromStdBoolData());
			message.AddTag(MarketDataCapture::FieldLabels::Header::EventTimeMs,
				aggregateTradeData.m_eventTimeMs->GetStringDataFromEventTimeMs());
			return message;
		}

		MiddlewareMQ::BqtJsonMessage AllMarketTickersToJsonMessage(
			const MarketData::AllMarketTickerData& allMarketTickerData, const std::string& symbol) {
			MiddlewareMQ::BqtJsonMessage message;
			message.AddTag(MarketDataCapture::FieldLabels::Header::Symbol, symbol);
			message.AddTag(MarketDataCapture::FieldLabels::Header::MessageType, "AllMarketTickers");

			return message;
		}

		MiddlewareMQ::BqtJsonMessage AllMiniTickersToJsonMessage(
			const MarketData::AllMiniTickerData& allMiniTickerData, const std::string& symbol) {
			MiddlewareMQ::BqtJsonMessage message;
			message.AddTag(MarketDataCapture::FieldLabels::Header::Symbol, symbol);
			message.AddTag(MarketDataCapture::FieldLabels::Header::MessageType, "AllMiniTickers");
			message.AddTag(MarketDataCapture::FieldLabels::AllMiniTicker::Tickers, "[]");
			message.AddTag(MarketDataCapture::FieldLabels::Header::SendingTime,
				allMiniTickerData.m_eventTimeMs->GetStringDataFromEventTimeMs());
			return message;
		}

		MiddlewareMQ::BqtJsonMessage AllDiffDepthToJsonMessage(
			const MarketData::AllDiffDepthData& allDiffDepthData, const std::string& symbol) {
			MiddlewareMQ::BqtJsonMessage message;
			message.AddTag(MarketDataCapture::FieldLabels::Header::Symbol, symbol);
			message.AddTag(MarketDataCapture::FieldLabels::Header::MessageType, "DiffDepth");
			message.AddTag(MarketDataCapture::FieldLabels::Header::EventTimeMs,
				allDiffDepthData.m_eventTimeMs->GetStringDataFromEventTimeMs());
			return message;
		}

		MiddlewareMQ::BqtJsonMessage AllPartDepthDataToJsonMessage(
			const MarketData::AllPartDepthData& allPartDepthData, const std::string& symbol) {
			MiddlewareMQ::BqtJsonMessage message;
			message.AddTag(MarketDataCapture::FieldLabels::Header::Symbol, symbol);
			message.AddTag(MarketDataCapture::FieldLabels::Header::MessageType, "PartDepth");
			message.AddTag(MarketDataCapture::FieldLabels::Header::EventTimeMs,
				allPartDepthData.m_eventTimeMs->GetStringDataFromEventTimeMs());
			return message;
		}

		MiddlewareMQ::BqtJsonMessage KlineCandleStickToJsonMessage(
			const MarketData::KlineCandleStickData& klineCandleStickData, const std::string& symbol) {
			MiddlewareMQ::BqtJsonMessage message;
			message.AddTag(MarketDataCapture::FieldLabels::Header::Symbol, symbol);
			message.AddTag(MarketDataCapture::FieldLabels::Header::MessageType, "KlineCandleStick");
			message.AddTag(MarketDataCapture::FieldLabels::Header::EventTimeMs,
				klineCandleStickData.m_eventTimeMs->GetStringDataFromEventTimeMs());
			message.AddTag(MarketDataCapture::FieldLabels::KlineCandleStick::KlineStartTime,
				klineCandleStickData.m_klineStartTime->GetStringDataFromEventTimeMs());
			message.AddTag(MarketDataCapture::FieldLabels::KlineCandleStick::KlineCloseTime,
				klineCandleStickData.m_klineCloseTime->GetStringDataFromEventTimeMs());
			message.AddTag(MarketDataCapture::FieldLabels::KlineCandleStick::Interval,
				klineCandleStickData.m_interval->GetStringData());
			message.AddTag(MarketDataCapture::FieldLabels::KlineCandleStick::FirstTradeId,
				klineCandleStickData.m_firstTradeId->GetStringDataFromStdUnsignedIntData());
			message.AddTag(MarketDataCapture::FieldLabels::KlineCandleStick::LastTradeId,
				klineCandleStickData.m_lastTradeId->GetStringDataFromStdUnsignedIntData());
			message.AddTag(MarketDataCapture::FieldLabels::KlineCandleStick::OpenPrice,
				klineCandleStickData.m_openPrice->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::KlineCandleStick::ClosePrice,
				klineCandleStickData.m_closePrice->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::KlineCandleStick::HighPrice,
				klineCandleStickData.m_highPrice->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::KlineCandleStick::LowPrice,
				klineCandleStickData.m_lowPrice->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::KlineCandleStick::BaseAssetVolume,
				klineCandleStickData.m_baseAssetVolume->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::KlineCandleStick::NumberOfTrades,
				klineCandleStickData.m_numberOfTrades->GetStringDataFromStdUnsignedIntData());
			message.AddTag(MarketDataCapture::FieldLabels::KlineCandleStick::IsThisKlineClosed,
				klineCandleStickData.m_isThisKlineClosed->GetStringDataFromStdBoolData());
			message.AddTag(MarketDataCapture::FieldLabels::KlineCandleStick::QuoteAssetVolume,
				klineCandleStickData.m_quoteAssetVolume->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::KlineCandleStick::TakerBuyBaseAssetVolume,
				klineCandleStickData.m_takerBuyBaseAssetVolume->GetStringDataFromBoostDouble());
			message.AddTag(MarketDataCapture::FieldLabels::KlineCandleStick::TakerBuyQuoteAssetVolume,
				klineCandleStickData.m_takerBuyQuoteAssetVolume->GetStringDataFromBoostDouble());
			return message;
		}
	} // namespace PythonMessage
}; // namespace MarketDataCapture
