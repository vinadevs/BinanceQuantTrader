/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "../MarketData/MarketDataObserver.h"
#include "../LibraryUtils/BQTDeque.h"

#include "Participant.h"

#include <queue>
#include <string>
#include <unordered_set>
#include <memory>

namespace ExchangeSimulator {

    class UserAccountManager;
	class UserTradeProfileManager;

	/**
	 * @class DownstreamFuturePriceManager
	 * @brief Represents a downstream future price in the exchange simulator.
	 *
	 * This class encapsulates the symbol and price information for a downstream future price.
	 */
    class DownstreamFuturePriceManager final
    {
    public:
        DownstreamFuturePriceManager(const std::string& symbol, const double price)
			: m_symbol(symbol), m_currentMarketPrice(price) {
		}
		const std::string& GetSymbol() const { return m_symbol; }
		const double GetCurrentMarketPrice() const { return m_currentMarketPrice; }
		void SetNewMarketPrice(const double price) { m_currentMarketPrice = price; }
	private:
		std::string m_symbol;
		double m_currentMarketPrice{ 0 };
    };

    /**
     * @class RTMarketFutureParticipant
     * @brief Represents a real-time market spot participant in the exchange simulator.
     *
     * This class is responsible for handling order matching for upstream orders and managing
     * downstream order books for subscribed symbols. It observes market data changes and updates
     * the best bid and ask order books accordingly. The class interacts with the UserAccountManager
     * to manage user accounts and ensures efficient order book management for spot trading simulation.
     *
     * Inherits from Participant and MarketDataObserver to integrate with the trading and market data systems.
     */
    class RTMarketFutureParticipant final
        : public Participant,
        public MarketData::MarketDataObserver
    {
    public:
        RTMarketFutureParticipant(const size_t maxDownstreamOrderBookSize, UserAccountManager* userAccountManager);
        ~RTMarketFutureParticipant() override;

        // order matching for upstream orders
        bool TryToMatchOrder(OrderManagement::BinanceNewOrder& newUpstreamOrder) override;
		// future market price from exchange
        bool OnTradeChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;

        void CreateDownstreamFuturePriceManagers(const std::unordered_set<std::string>& subcribedSymbols);
    private:
        void UpdateCurrentMarketPrice(const std::string& symbol, const double price);

		std::unordered_map<std::string, std::unique_ptr<DownstreamFuturePriceManager>> m_downstreamFuturePriceManagers;
		std::unique_ptr<UserTradeProfileManager> m_userTradeProfileManager;
    };
};
