/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "UpstreamOrder.h"
#include "DownstreamOrderBook.h"

#include <memory>
#include <mutex>

namespace LibraryUtils {
    class Logger;
};

/**
 * @class Participant
 * @brief Represents a downstream trader in the order matching engine.
 *
 * The Participant class models a trader who submits buy and sell orders to the matching engine.
 * Each participant has unique attributes, such as an identifier, account balance, and trading history,
 * which influence their interactions with the engine. The class provides functionality to manage
 * the participant's orders, track their activities, and integrate with the core matching process.
 *
 * Key Responsibilities:
 * - Store and manage trader-specific data such as order history, account details, and trading preferences.
 * - Facilitate order submission, modification, and cancellation.
 * - Provide information for order matching, including available funds or asset holdings.
 */

namespace ExchangeSimulator {

    enum class ParticipantType : unsigned
    {
        UNDEF,
        SIMULATOR, // Participant is trader simulation
		REAL_TIME_SPOT_MARKET_DATA, // Participant is from real-time spot market data
		REAL_TIME_FUTURE_MARKET_DATA, // Participant is from real-time future market data
        HISTORICAL_DATA, // Participant is from historical market data
    };

    enum class FillType : unsigned
    {
		BASED_ON_LAST_TRADE, // fill algo's order bases on last trade price
		BASED_ON_BEST_BID_ASK, // fill algo's order bases on best bid/ask price
    };

    class UserAccountManager;
	class UserTradeProfileManager;

    class Participant
    {
    public:
        Participant(const ParticipantType& mode,
            UserAccountManager* userAccountManager,
            UserTradeProfileManager* userTradeProfileManager);
        virtual ~Participant();

		// this method is used to match incoming orders from upstream order queue
        virtual bool TryToMatchOrder(OrderManagement::BinanceNewOrder& order) = 0;
		// this method is used to update user balance after cancel order
        virtual void HandleUserBalanceAfterCancelOrder(const OrderManagement::BinanceNewOrder& order) = 0;

        ParticipantType GetParticipantType() const { return m_tradeMode; };
    protected:
        std::unique_ptr<LibraryUtils::Logger> m_logger;
        std::unique_ptr<DownstreamOrderBook> m_downstreamOrderBook;
        UserAccountManager* m_userAccountManager{ nullptr };
        UserTradeProfileManager* m_userTradeProfileManager{ nullptr };
        std::mutex m_mutex;
        ParticipantType m_tradeMode{ ParticipantType::UNDEF };
    };
};

