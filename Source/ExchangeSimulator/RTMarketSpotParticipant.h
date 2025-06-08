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
#include "DownstreamOrderBook.h"

#include <queue>
#include <unordered_set>

namespace ExchangeSimulator {

    class UserAccountManager;

    /**
     * @class RTMarketSpotParticipant
     * @brief Represents a real-time market spot participant in the exchange simulator.
     *
     * This class is responsible for handling order matching for upstream orders and managing
     * downstream order books for subscribed symbols. It observes market data changes and updates
     * the best bid and ask order books accordingly. The class interacts with the UserAccountManager
     * to manage user accounts and ensures efficient order book management for spot trading simulation.
     *
     * Inherits from Participant and MarketDataObserver to integrate with the trading and market data systems.
     */
    class RTMarketSpotParticipant final
        : public Participant,
          public MarketData::MarketDataObserver
    {
    public:
        RTMarketSpotParticipant(const size_t maxDownstreamOrderBookSize, UserAccountManager* userAccountManager);
        ~RTMarketSpotParticipant() override;

        // order matching for upstream orders
        bool TryToMatchOrder(OrderManagement::BinanceNewOrder& newUpstreamOrder) override;
        // book quote for downstream orders
        bool OnIndividualBookTickerChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;

        void CreateDownstreamOrderBooks(const std::unordered_set<std::string>& subcribedSymbols);
    private:
        void UpdateBestMarketBidOrderBook(const std::string& symbol, const double price, const double quantity);
        void UpdateBestMarketAskOrderBook(const std::string& symbol, const double price, const double quantity);

        size_t m_maxDownstreamOrderBookSize{ 0 };
        std::unique_ptr<DownstreamOrderBook> m_bidDownstreamOrderBooks;
        std::unique_ptr<DownstreamOrderBook> m_askDownstreamOrderBooks;
    };
};
