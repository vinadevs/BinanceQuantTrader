/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "../MiddlewareMQ/MessageHandler.h"
#include "../MiddlewareMQ/BqtJsonMessage.h"

#include "ExchangeServiceInterface.h"
#include "UpstreamOrder.h"

#include <memory>
#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include <condition_variable>

namespace tinyxml2 {
    class XMLElement;
	class XMLDocument;
};

namespace LibraryUtils {
    class Logger;
};

namespace MarketData {
    class RealTimeMarketData;
};

/*
 * Class: MatchingEngine
 *
 * Description:
 * The MatchingEngine class is responsible for handling the core functionality of
 * an order-matching system in a financial or cryptocurrency exchange. It processes
 * buy and sell orders, matches them based on price and time priority, and executes
 * trades when conditions are met. The engine ensures fair and efficient execution
 * of orders in a high-performance, low-latency environment.
 *
 * Responsibilities:
 * - Maintain an order book for each trading pair.
 * - Match buy and sell orders based on price and time priority.
 * - Execute trades and update order statuses.
 * - Handle edge cases such as partial fills, cancellations, and invalid orders.
 * - Provide real-time data on market depth and executed trades.
 *
 * Features:
 * - Supports limit orders, market orders, and stop-limit orders.
 * - Optimized for high-frequency trading with minimal latency.
 * - Thread-safe design for handling multiple concurrent orders.
 * - Extensible architecture to support additional order types and trading pairs.
 *
 * Disclaimer:
 * This class is a simulation and may not fully adhere to real-world regulatory
 * or exchange-specific requirements. It is intended for educational or testing purposes.
 *
 */

namespace ExchangeSimulator {

    enum class DownstreamAckBehaviour : unsigned
    {
        USING_HTTP_REQUEST, // get acks via http request
        USING_TCPIP_MQ, // get auto acks via tcpip connection from macthing engine
    };

    class Participant; // simulative traders
	class RTMarketSpotParticipant; // simulative spot traders
	class RTMarketFutureParticipant; // simulative future traders
    class UserAccountManager; // user wallet info 
	class UserTradeProfileManager; // user trade profile info
    class UpstreamOrderQueueMgr; // lisst pre-orders matching process
    class UpstreamOrderMatchedMgr; // list post-orders matching process

    class MatchingEngine final 
        : public MiddlewareMQ::MessageHandler,
          public ExchangeServiceInterface
    {
    public:
        MatchingEngine(const tinyxml2::XMLElement* matchingEngineXmlCfg,
                       UserAccountManager* userAccountManager,
                       UserTradeProfileManager* userTradeProfileManager);
        ~MatchingEngine();

        void Start() override;
        void Stop() override;

        void OnHandlingReceivedSimulatorMessage(const MiddlewareMQ::BqtJsonMessage& message) override;
    private:
        void SubscribeTargetSymbols(const tinyxml2::XMLDocument* matchingEngineXmlCfg);
        void ProcessIncommingOrders();

        bool VerifyUpstreamBinanceNewOrder(const OrderManagement::BinanceNewOrder& order);
        bool VerifyUpstreamBinanceCancelOrder(const OrderManagement::BinanceCancelOrder& order);
        bool VerifyUpstreamBinanceReplaceOrder(const OrderManagement::BinanceReplaceOrder& order);
        bool VerifyUpstreamBinanceQueryOrder(const OrderManagement::BinanceQueryOrder& order);
       
        void PostProcessingMatchedNewOrder(OrderManagement::BinanceNewOrder& order);
        void PostProcessingMatchedCancelOrder(OrderManagement::BinanceCancelOrder& order);
        void PostProcessingMatchedReplaceOrder(OrderManagement::BinanceReplaceOrder& order);
        void PostProcessingMatchedQueryOrder(OrderManagement::BinanceQueryOrder& order);

        OrderManagement::BinanceNewOrder ConstructUpstreamNewOrder(
            const MiddlewareMQ::BqtJsonMessage& message);
        OrderManagement::BinanceCancelOrder ConstructUpstreamCancelOrder(
            const MiddlewareMQ::BqtJsonMessage& message);
        OrderManagement::BinanceReplaceOrder ConstructUpstreamReplaceOrder(
            const MiddlewareMQ::BqtJsonMessage& message);
        OrderManagement::BinanceQueryOrder ConstructUpstreamQueryOrder(
            const MiddlewareMQ::BqtJsonMessage& message);

        std::unique_ptr<LibraryUtils::Logger> m_logger;
        std::thread m_thread;
        std::condition_variable m_orderQueueCond; // avoid polling thread
        std::atomic<bool> m_isRunning{ false }; // lock free thread
        std::atomic<bool> m_hasNewOrder{ false }; // lock free thread
        std::mutex m_mutex;
        std::unique_ptr<UpstreamOrderQueueMgr> m_upstreamOrderQueueMgr;
        std::unique_ptr<UpstreamOrderMatchedMgr> m_upstreamOrderMatchedMgr;
        UserAccountManager* m_userAccountManager{ nullptr };
        std::unique_ptr<Participant> m_participant;
        RTMarketSpotParticipant* m_rtMarketSpotParticipant{ nullptr };
        RTMarketFutureParticipant* m_rtMarketFutureParticipant{ nullptr };
        std::unique_ptr<MarketData::RealTimeMarketData> m_marketData;
        std::unique_ptr<tinyxml2::XMLDocument> m_binanceMarketDataConfig;
        DownstreamAckBehaviour m_downstreamAckBehaviour{ DownstreamAckBehaviour::USING_HTTP_REQUEST };
    };
};

