/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include <string>

namespace MarketDataCapture {
    namespace FieldLabels {

        // =============================
        // Header
        // =============================
        namespace Header {
            const std::string MessageType = "MessageType";
            const std::string Symbol = "Symbol";
            const std::string SendingTime = "SendingTime";
            const std::string EventTimeMs = "EventTimeMs";
        }

        // =============================
        // IndividualBookTicker
        // =============================
        namespace IndividualBookTicker {
            const std::string BestBidPrice = "BestBidPrice";
            const std::string BestBidQuantity = "BestBidQuantity";
            const std::string BestAskPrice = "BestAskPrice";
            const std::string BestAskQuantity = "BestAskQuantity";
        }

        // =============================
        // Trade
        // =============================
        namespace Trade {
            const std::string TradeId = "TradeId";
            const std::string Price = "Price";
            const std::string Quantity = "Quantity";
            const std::string BuyerOrderId = "BuyerOrderId";
            const std::string SellerOrderId = "SellerOrderId";
            const std::string TradeTime = "TradeTime";
            const std::string IsBuyerMarketMaker = "IsBuyerMarketMaker";
        }

        // =============================
        // IndividualMarketTicker
        // =============================
        namespace IndividualMarketTicker {
            const std::string PriceChange = "PriceChange";
            const std::string PriceChangePercent = "PriceChangePercent";
            const std::string WeightedAveragePrice = "WeightedAveragePrice";
            const std::string FirstTrade = "FirstTrade";
            const std::string LastPrice = "LastPrice";
            const std::string LastQuantity = "LastQuantity";
            const std::string BestBidPrice = "BestBidPrice";
            const std::string BestBidQuantity = "BestBidQuantity";
            const std::string BestAskPrice = "BestAskPrice";
            const std::string BestAskQuantity = "BestAskQuantity";
            const std::string OpenPrice = "OpenPrice";
            const std::string HighPrice = "HighPrice";
            const std::string LowPrice = "LowPrice";
            const std::string TotalTradedBaseAssetVolume = "TotalTradedBaseAssetVolume";
            const std::string TotalTradedQuoteAssetVolume = "TotalTradedQuoteAssetVolume";
            const std::string StatisticsOpenTime = "StatisticsOpenTime";
            const std::string StatisticsCloseTime = "StatisticsCloseTime";
            const std::string FirstTradeId = "FirstTradeId";
            const std::string LastTradeId = "LastTradeId";
            const std::string TotalNumberOfTrades = "TotalNumberOfTrades";
        }

        // =============================
        // AllBookTicker
        // =============================
        namespace AllBookTicker {
            const std::string Tickers = "Tickers";
        }

        // =============================
        // IndividualMiniTicker
        // =============================
        namespace IndividualMiniTicker {
            const std::string ClosePrice = "ClosePrice";
            const std::string OpenPrice = "OpenPrice";
            const std::string HighPrice = "HighPrice";
            const std::string LowPrice = "LowPrice";
            const std::string TotalTradedBaseAssetVolume = "TotalTradedBaseAssetVolume";
            const std::string TotalTradedQuoteAssetVolume = "TotalTradedQuoteAssetVolume";
        }

        // =============================
        // AllMiniTicker
        // =============================
        namespace AllMiniTicker {
            const std::string Tickers = "Tickers";
        }

        // =============================
        // AggregateTrade
        // =============================
        namespace AggregateTrade {
            const std::string AggregateTradeId = "AggregateTradeId";
            const std::string Price = "Price";
            const std::string Quantity = "Quantity";
            const std::string FirstTradeId = "FirstTradeId";
            const std::string LastTradeId = "LastTradeId";
            const std::string TradeTime = "TradeTime";
            const std::string IsBuyerMarketMaker = "IsBuyerMarketMaker";
        }

        // =============================
        // KlineCandleStick
        // =============================
        namespace KlineCandleStick {
            const std::string KlineStartTime = "KlineStartTime";
            const std::string KlineCloseTime = "KlineCloseTime";
            const std::string Interval = "Interval";
            const std::string FirstTradeId = "FirstTradeId";
            const std::string LastTradeId = "LastTradeId";
            const std::string OpenPrice = "OpenPrice";
            const std::string ClosePrice = "ClosePrice";
            const std::string HighPrice = "HighPrice";
            const std::string LowPrice = "LowPrice";
            const std::string BaseAssetVolume = "BaseAssetVolume";
            const std::string NumberOfTrades = "NumberOfTrades";
            const std::string IsThisKlineClosed = "IsThisKlineClosed";
            const std::string QuoteAssetVolume = "QuoteAssetVolume";
            const std::string TakerBuyBaseAssetVolume = "TakerBuyBaseAssetVolume";
            const std::string TakerBuyQuoteAssetVolume = "TakerBuyQuoteAssetVolume";
        }
    } // namespace FieldLabels
} // namespace MarketDataCapture
