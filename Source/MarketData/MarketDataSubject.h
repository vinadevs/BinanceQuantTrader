/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "SynchronousMarketData.h"

#include <list>
#include <string>

namespace MarketData {

class MarketDataObserver;

/**
 * @class MarketDataSubject
 * @brief Represents the subject in the Observer design pattern.
 *
 * The Subject class maintains a list of observers and notifies them of any changes
 * to its state. Observers register themselves with the subject to receive updates
 * whenever a specific event occurs.
 *
 * Key Responsibilities:
 * - Maintain a collection of observers.
 * - Provide methods to add, remove, and notify observers.
 * - Notify all registered observers when a state change occurs.
 */

class DLL_CLASS_MARKETDATA_EXPORTS MarketDataSubject
{
public:
	MarketDataSubject() = default;
	virtual ~MarketDataSubject();

    /**
     * @brief Register an observer to receive market data callbacks.
     *
     * NOT thread-safe unless caller provides synchronization.
     * Duplicate attaches should be avoided.
     *
     * @param observer Non-owning pointer to observer.
     */
    virtual void AttachMarketDataObserver(MarketDataObserver* observer);

    /**
     * @brief Remove an observer from notification list.
     *
     * Must be called before observer destruction.
     *
     * @param observer Observer to detach.
     */
    virtual void DettachMarketDataObserver(MarketDataObserver* observer);

    /**
     * @brief Remove all observers.
     *
     * Typically used during shutdown/cleanup.
     */
    virtual void ClearMarketDataObservers();

	// ============================================================
	// These functions are sycnchronous calls, only one notify message can
	// be delivered to listeners each time...
	// We should call Update Market Data functions before call this function
	// Return int as we need to count number of succeeded updates
	// Input symbol param specifics for symbol has data change

    /**
     * @brief Notify observers that best bid/ask (top-of-book) changed.
     *
     * Highest frequency path.
     * Must remain extremely low latency.
     *
     * @return number of observers that handled event successfully.
     */
    virtual int NotifyIndividualBookTickerChange(const std::string& symbol);

    /**
     * @brief Notify observers of a raw trade execution.
     *
     * Fired for every trade.
     *
     * @return success count.
     */
    virtual int NotifyTradeChange(const std::string& symbol);

    /**
     * @brief Notify 24h rolling ticker statistics update for one symbol.
     *
     * Medium frequency.
     * Mostly analytics/UI usage.
     */
    virtual int NotifyIndividualMarketTickerChange(const std::string& symbol);

    /**
     * @brief Notify lightweight ticker update for one symbol.
     *
     * Optimized bandwidth version of full ticker.
     */
    virtual int NotifyMiniTickerChange(const std::string& symbol);

    /**
     * @brief Notify candlestick/kline update.
     *
     * Interval-based updates (1m/5m/1h/etc).
     * Used by indicators or strategies.
     */
    virtual int NotifyKlineCandleStickChange(const std::string& symbol);

    /**
     * @brief Notify aggregated trade update.
     *
     * Aggregated trades reduce event rate compared to raw trades.
     */
    virtual int NotifyAggregateTradeChange(const std::string& symbol);

    /**
     * @brief Notify batch update containing ALL market tickers.
     *
     * Efficient for global scans or ranking symbols.
     */
    virtual int NotifyAllMarketTickersChange(const std::string& symbol);

    /**
     * @brief Notify batch update containing ALL mini tickers.
     *
     * Lower bandwidth global stats stream.
     */
    virtual int NotifyAllMiniTickersChange(const std::string& symbol);

    /**
     * @brief Notify partial order book depth update.
     *
     * Provides top-N levels only.
     * Good trade-off between precision and bandwidth.
     */
    virtual int NotifyAllPartDepthChange(const std::string& symbol);

    /**
     * @brief Notify incremental diff depth update.
     *
     * Sends only changed levels.
     *
     * Observers must:
     *   - maintain local order book
     *   - apply sequence numbers strictly
     */
    virtual int NotifyAllDiffDepthChange(const std::string& symbol);

    /**
     * @brief Notify account-level state update.
     *
     * Includes:
     *   - position changes
     *   - margin updates
     *   - account status
     */
    virtual int NotifyUserDataAccountUpdateChange(const std::string& symbol);

    /**
     * @brief Notify wallet/balance change.
     *
     * Triggered by:
     *   - funding
     *   - deposit/withdraw
     *   - realized PnL
     */
    virtual int NotifyUserDataBalanceUpdateChange(const std::string& symbol);

    /**
     * @brief Notify order lifecycle update.
     *
     * Covers:
     *   - new
     *   - filled/partial
     *   - canceled
     *   - rejected
     */
    virtual int NotifyUserDataOrderUpdateChange(const std::string& symbol);


    // ============================================================
    //  Synchronous data access
    // ============================================================
    /**
     * @brief Access latest cached market data synchronously.
     *
     * Allows observers or external components to query current state of market data
     * without waiting for next event.
     *
     * May return nullptr if symbol not found or not supported.
     *
     * @return pointer to current market data cache.
     */
    virtual SynchronousMarketData* GetSynchronousMarketData(const std::string& symbol) { return nullptr; }
protected:
	// collection of observers
	using ListMarketDataObserver = std::list<MarketDataObserver*>;
	ListMarketDataObserver m_marketDataObservers;
};

};