/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include "../LibraryUtils/SourceBuildFlags.h"
#include "../OrderRouting/OrderAckEventHandler.h"
#if USE_BACK_TEST_TRADING
#include "../MiddlewareMQ/MessageHandler.h"
#include "../MiddlewareMQ/BqtJsonMessage.h"
#endif
#include "CompilanceChecker.h"
#include "ExternalRequestReceiver.h"
#include <memory>
#include <string>
#if USE_MULTITHREADING
#include <atomic>
#endif

namespace MarketData {
	class RealTimeMarketData;
}

namespace UserAccount {
	class Trader;
	class BinanceTrader;
	class FutureTrader;
}

namespace ComplianceNRegulatory {
	class BinanceTradingRules;
}

namespace tinyxml2 {
	class XMLDocument;
}

namespace OrderManagement {
	class ParentOrderManager;
}

// -All Algos, Strategies should follow this base class
// Strategy/Algo should take actions when signal/indicator event
// triggered or alarm system is triggered after preset interval time...

// -If USE_MULTITHREADING flag is enabled then Strategies is trading
// as ansynchronous with signal/indicator events
// It means, it is running in a separared thread to have better
// market data analysis operations when avoiding stale data

namespace TradingStrategies {
	// We are supporting 3 types of algorithms
	enum class StrategyType : unsigned
	{
		UNDEF,
		FULL_AUTO, // no control from trader at runtime
		SEMI_AUTO, // can control from trader at runtime
		ADVISING, // only give trading hints to trader, no order placement
	};

	enum class StrategyRunStatus : unsigned
	{
		UNDEF,
		STOP, // algo/strategy is not started 
		LIVE, // algo/strategy is running and can trade
		PENDING, // algo/strategy is running but can not trade
	};

	enum class StrategyLifeTime : unsigned
	{
		UNDEF,
		INTRA_DAY, //strategy run in a day
		INTRA_WEEK, // strategy run in a week
		INTRA_MONTH, // strategy run in a month
	};

	enum class StrategyOrderScheduler : unsigned
	{
		UNDEF,
		ALARM_BASED, // use alarm system to schedule trading
		SIGNAL_BASED, // use signal/indicator events to schedule trading
	};

	// -A trading strategy is a predefined set of rules and guidelines that a trader
	// follows to make buying or selling decisions in financial markets, 
	// such as stocks, cryptocurrencies, forex, or commodities.
	// -The strategy is designed to maximize returns, manage risk,
	// and achieve specific investment objectives.Trading strategies
	// can vary in complexity, from simple approaches based on a few
	// indicators to complex algorithms involving data analysis...

	class DLL_CLASS_TRADING_TRATEGIES_EXPORTS
		TradingStrategyBase :
#if USE_BACK_TEST_TRADING
		public MiddlewareMQ::MessageHandler, // to receive messages from exchange simulator
#endif
		public OrderRouting::OrderAckEventHandler, // to receive order acks for sent orders
		public TradingStrategies::ExternalRequestReceiver // to receive external parent orders
	{
	public:
		// This construtcor is used for strategies that needs to send order to exchange
		TradingStrategyBase(
			const std::string& strategyName,
			const std::string& strategyDescription,
			const std::string& strategyCfgPath,
			MarketData::RealTimeMarketData* marketData,
			UserAccount::Trader* trader,
			ComplianceNRegulatory::BinanceTradingRules* tradingRules);
		
		// This constructor is used for strategies that only need to receive market data
		TradingStrategyBase(
			const std::string& strategyName,
			const std::string& strategyDescription,
			const std::string& strategyCfgPath,
			MarketData::RealTimeMarketData* marketData);

		virtual ~TradingStrategyBase();

		// -After this function called, then real trading will start, the preparation is finished.
		// -There are many algo/strategy trading types from full-auto to semi-auto.
		// -So each strategy with have a different behaviour with other types
		// -We should create all necessary components for the strategy inside this function
		// -We will not add any specific logic, just make it as a pure method
		// and let derived class implement the detail.
		virtual void StartTrade() = 0;
		// Shutdown strategy or stop trading
		// We need to set these variables to unlive, m_strategyRunStatus,
		// m_isThreadTradeOngoing (in case we use mutiple threads)
		// We should also unsubscribe all symbols to stop receiving market data
		virtual void StopTrade() = 0;

		// Report PNL, trades, ...
		virtual void ReportTradeResults(const std::string& symbol) = 0;

		// -We will control strategy parameters from external file...
		virtual void InitializeParameters(const std::string& strategyCfgPath) = 0;
		// -Can we trade now?
		StrategyRunStatus GetStrategyRunStatus() const;
		// -Gets the strategy type, which defines how the strategy operates.
		StrategyType GetStrategyType() const;
		// -Get order scheduler type, which defines how the strategy schedules orders.
		StrategyOrderScheduler GetStrategyOrderScheduler() const;
		// -Sets the strategy type, which defines how the strategy operates.
		void SetStrategyType(const StrategyType strategyType);
		// -Gets the strategy lifetime, which defines how long the strategy will run.
		std::string GetStrategyTypeStr(const StrategyType strategyType);
		// -Gets the strategy name, which is a human-readable identifier for the strategy.
		const std::string& GetStrategyName() const;
		// -Gets the strategy identifier, which is a unique identifier for the strategy.
		const std::string& GetStrategyID() const;
		// Sets up the lifetime of the trading strategy based on the provided XML configuration.
		// The configuration defines whether the strategy runs intra-day, intra-week, or intra-month.
		void SetupStrategyLifeTime(tinyxml2::XMLDocument* strategyCfgPathXml);
		// -Checks if the trading strategy has not exceeded the defined trading rules.
		// This ensures compliance with exchange regulations and prevents violations.
		bool IsNotIsNotExceededTradingRules() const;
		// -Increases the counter for REST API requests made to the exchange for compliance tracking.
		// This helps monitor and limit the number of requests to avoid exceeding API rate limits.
		void IncreaseComplianceRestAPIRequestCounter(const size_t noOfRequests);
		// -Get parent order manager to manage all parent orders for this strategy
		OrderManagement::ParentOrderManager* GetParentOrderManager() const {
			return m_parentOrderManager.get();
		}

protected:
		// -Logs the hard limits for trading, such as maximum orders or API requests allowed.
		// This is useful for debugging and ensuring the strategy operates within defined constraints.
		void LogTradingHardLimits();
		// -Sets up the quantitative strategist for the trading strategy.
		void InitQuantStrategist();
		// -Init parent order manager to manage all parent orders for this strategy
		void InitParentOrderManager();
#if USE_BACK_TEST_TRADING
		// -Handles messages received from the exchange simulator during backtesting.
		// This function processes simulated market data or order responses for testing purposes.
		void OnHandlingReceivedSimulatorMessage(
			const MiddlewareMQ::BqtJsonMessage& message) override;
#endif
		/// <strategy data members>
		const std::string m_strategyName; // what is algo's name?
		const std::string m_strategyDescription; // what will algo do?
		const std::string m_strategyID; // must be unique ID, we may use it for routing orders
		const std::string& m_strategyCfgPath; // strategy config paramters file
		MarketData::RealTimeMarketData* m_marketData {nullptr}; // real time market data
		UserAccount::Trader* m_trader{ nullptr }; // user account and trade actions
		UserAccount::BinanceTrader* m_spotTrader{ nullptr }; // binance spot trader
		UserAccount::FutureTrader* m_futureTrader{ nullptr }; // binance future trader
		ComplianceNRegulatory::BinanceTradingRules* m_tradingRules{ nullptr }; // exchange compliance and regulatory
		std::unique_ptr<CompilanceChecker> m_compilanceChecker; // reset trading hard limits from exchange
		std::unique_ptr<LibraryUtils::Logger> m_logger; // log message
		StrategyType m_strategyType { StrategyType::UNDEF};
		StrategyLifeTime m_StrategyLifeTime { StrategyLifeTime::INTRA_DAY };
		// -For strategies that run in a same thread with main thread
		// then we can check this flag to know if the strategy is still live
		StrategyRunStatus m_strategyRunStatus{ StrategyRunStatus::UNDEF };
		// Which order scheduler will be used for this strategy
		StrategyOrderScheduler m_strategyOrderScheduler{ StrategyOrderScheduler::UNDEF };
		// -For strategies that need to be run in a separated thread
		// then it will need to maintain a TradingLoop() function to 
		// keep the thread live, this hack will help to keep the while loop running
#if USE_MULTITHREADING
		std::atomic<bool> m_isThreadTradeOngoing; // non blocking
#endif
		// -Config for strategy, we will use it to setup strategy parameters
		std::unique_ptr<tinyxml2::XMLDocument> m_strategyCfgXml;
		// -Parent order manager to manage all parent orders for this strategy
		std::unique_ptr<OrderManagement::ParentOrderManager> m_parentOrderManager;
	};
};

//---------------------------------------------------------------------------------------------
// This pair of macros should be used to wrap all trading activities
// start of trading activity macro
#define BEGIN_STRATEGY_ORDER_SENDING_ACTIVITY \
try \
{ \
	if (m_strategyRunStatus == StrategyRunStatus::LIVE) \
	{ \
		if (IsNotIsNotExceededTradingRules()) \
		{
// end of trading activity macro with return value
#define END_STRATEGY_ORDER_SENDING_RETURN \
} \
		else \
		{ \
			m_logger->Debug("Warning: number of REST request is exceeded exchange rule/limitations."); \
		} \
	} \
	else \
	{ \
		m_logger->Warning("Strategy is not living now."); \
	} \
} \
catch (const std::exception& e) \
{ \
	m_logger->Exception(std::string(e.what())); \
	return false; \
} \
catch (...) \
{ \
	m_logger->Exception("Unknown exception occurred."); \
	return false; \
}
// end of trading activity macro without return value
#define END_STRATEGY_ORDER_SENDING_NO_RETURN \
} \
		else \
		{ \
			m_logger->Debug("Warning: number of REST request is exceeded exchange rule/limitations."); \
		} \
	} \
	else \
	{ \
		m_logger->Warning("Strategy is not living now."); \
	} \
} \
catch (const std::exception& e) \
{ \
	m_logger->Exception(std::string(e.what())); \
	return; \
} \
catch (...) \
{ \
	m_logger->Exception("Unknown exception occurred."); \
	return; \
}
//---------------------------------------------------------------------------------------------
