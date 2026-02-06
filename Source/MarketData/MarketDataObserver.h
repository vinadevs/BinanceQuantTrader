/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include <string>

namespace MarketData {

class MarketDataSubject;

/**
  * @class MarketDataObserver
  * @brief Observer interface for receiving asynchronous market data updates.
  *
  * This class follows the Observer pattern. MarketDataSubject pushes events
  * to all registered observers whenever new data arrives from exchange streams.
  *
  * IMPORTANT:
  *  - All callbacks are asynchronous.
  *  - They may be invoked from I/O or network threads.
  *  - Implementations MUST be non-blocking and low-latency.
  *  - Avoid heavy allocations or locks inside hot paths.
  *
  * Return value:
  *  - true  : event handled successfully
  *  - false : handler failed (subject may log or ignore)
  */

class DLL_CLASS_MARKETDATA_EXPORTS MarketDataObserver
{
public:
    MarketDataObserver() = default;
    virtual ~MarketDataObserver() = default;

    /**
     * @brief Best bid/ask update for a single symbol (BookTicker stream).
     *
     * Triggered whenever the top-of-book changes:
     *  - best bid price/qty
     *  - best ask price/qty
     *
     * Frequency: VERY HIGH (microseconds level).
     * Typical usage:
     *  - spread strategies
     *  - market making
     *  - arbitrage
     *
     * @param marketData Source subject containing latest book snapshot.
     * @param symbol Trading pair (e.g., BTCUSDT).
     */
    virtual bool OnIndividualBookTickerChange(
        MarketDataSubject* marketData,
        const std::string& symbol);

    /**
     * @brief Raw trade execution event for a single symbol.
     *
     * Fired for every matched trade on the exchange.
     *
     * Contains:
     *  - price
     *  - quantity
     *  - trade side
     *  - trade id
     *
     * Typical usage:
     *  - tape reading
     *  - volume signals
     *  - momentum detection
     */
    virtual bool OnTradeChange(
        MarketDataSubject* marketData,
        const std::string& symbol);

    /**
     * @brief 24h rolling statistics update for one symbol.
     *
     * Contains:
     *  - last price
     *  - high/low
     *  - volume
     *  - price change percent
     *
     * Frequency: medium (≈ 1s).
     *
     * Typical usage:
     *  - UI dashboards
     *  - volatility signals
     */
    virtual bool OnIndividualMarketTickerChange(
        MarketDataSubject* marketData,
        const std::string& symbol);

    /**
     * @brief Lightweight ticker (mini version of market ticker).
     *
     * Provides only:
     *  - last price
     *  - volume
     *
     * Lower bandwidth than full ticker.
     *
     * Typical usage:
     *  - scanning many symbols efficiently
     */
    virtual bool OnMiniTickerChange(
        MarketDataSubject* marketData,
        const std::string& symbol);

    /**
     * @brief Aggregated trade event (multiple trades grouped).
     *
     * Exchange aggregates consecutive trades at same price.
     *
     * Reduces message rate compared to raw trade stream.
     *
     * Typical usage:
     *  - backtesting
     *  - coarse analytics
     */
    virtual bool OnAggregateTradeChange(
        MarketDataSubject* marketData,
        const std::string& symbol);

    /**
     * @brief Kline/Candlestick update for a symbol.
     *
     * Triggered periodically for each interval:
     *  1m, 5m, 15m, 1h, 1d, etc.
     *
     * Contains:
     *  - open/high/low/close
     *  - volume
     *  - closed flag
     *
     * Typical usage:
     *  - indicators
     *  - strategy signals
     *  - charting
     */
    virtual bool OnKlineCandleStickChange(
        MarketDataSubject* marketData,
        const std::string& symbol);

    /**
     * @brief Batch update for ALL market tickers.
     *
     * Provides 24h stats for every symbol in one message.
     *
     * Useful for:
     *  - global scanning
     *  - ranking top movers
     */
    virtual bool OnAllMarketTickersChange(
        MarketDataSubject* marketData,
        const std::string& symbol);

    /**
     * @brief Batch update for ALL mini tickers.
     *
     * Lightweight version of all-market stats.
     *
     * Best for:
     *  - low latency monitoring
     *  - multi-symbol strategies
     */
    virtual bool OnAllMiniTickersChange(
        MarketDataSubject* marketData,
        const std::string& symbol);

    /**
     * @brief Partial order book depth update.
     *
     * Provides top N levels only (e.g., 5/10/20 levels).
     *
     * Suitable for:
     *  - market making
     *  - shallow book strategies
     *
     * Lower bandwidth than full diff depth.
     */
    virtual bool OnAllPartDepthChange(
        MarketDataSubject* marketData,
        const std::string& symbol);

    /**
     * @brief Incremental (diff) depth update.
     *
     * Sends only changed levels since last update.
     *
     * Requires:
     *  - local order book reconstruction
     *  - strict sequence handling
     *
     * Frequency: EXTREMELY HIGH.
     *
     * Critical for:
     *  - HFT
     *  - precise liquidity modeling
     */
    virtual bool OnAllDiffDepthChange(
        MarketDataSubject* marketData,
        const std::string& symbol);

    /**
     * @brief Account-level update event.
     *
     * Triggered when:
     *  - position changes
     *  - margin changes
     *  - account status changes
     *
     * Used by:
     *  - risk engine
     *  - PnL tracking
     */
    virtual bool OnUserDataAccountUpdateChange(
        MarketDataSubject* marketData,
        const std::string& symbol);

    /**
     * @brief Balance update event.
     *
     * Triggered when wallet balance changes due to:
     *  - deposits
     *  - withdrawals
     *  - funding
     *  - realized PnL
     */
    virtual bool OnUserDataBalanceUpdateChange(
        MarketDataSubject* marketData,
        const std::string& symbol);

    /**
     * @brief Order status update.
     *
     * Fired when:
     *  - new order
     *  - fill/partial fill
     *  - cancel
     *  - reject
     *
     * Critical for:
     *  - order state machine
     *  - execution engine
     */
    virtual bool OnUserDataOrderUpdateChange(
        MarketDataSubject* marketData,
        const std::string& symbol);
};
};