/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include "../LibraryUtils/Logger.h"
#include "../LibraryUtils/SourceBuildFlags.h"
#if USE_BACK_TEST_TRADING
#include "../MiddlewareMQ/MessageHandler.h"
#include "../MiddlewareMQ/BqtJsonMessage.h"
#endif
#include "CompilanceChecker.h"

#include <memory>
#include <string>
#if USE_MULTITHREADING
#include <atomic>
#endif

namespace MarketData {
	class RealTimeMarketData;
}

namespace UserSpotAccount {
	class BinanceTrader;
}

namespace ComplianceNRegulatory {
	class BinanceTradingRules;
}

namespace tinyxml2 {
	class XMLDocument;
};

// -All Algos, Strategies should follow this base class
// Strategy/Algo should take actions when signal/indicator event
// triggered...

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

	// -A trading strategy is a predefined set of rules and guidelines that a trader
	// follows to make buying or selling decisions in financial markets, 
	// such as stocks, cryptocurrencies, forex, or commodities.
	// -The strategy is designed to maximize returns, manage risk,
	// and achieve specific investment objectives.Trading strategies
	// can vary in complexity, from simple approaches based on a few
	// indicators to complex algorithms involving data analysis...

	class DLL_CLASS_TRADING_TRATEGIES_EXPORTS
		TradingStrategyBase
#if USE_BACK_TEST_TRADING  
		: public MiddlewareMQ::MessageHandler
#endif
	{
	public:
		// This construcor is used for strategies that needs to send order to exchange
		TradingStrategyBase(
			const std::string& strategyName,
			const std::string& strategyDescription,
			const std::string& strategyCfgPath,
			MarketData::RealTimeMarketData* marketData,
			UserSpotAccount::BinanceTrader* trader,
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
		// -We will not add any specific logic, just make it as a pure method
		// and let derived class implement the detail.
		virtual void StartLive() = 0;
		// Shutdown strategy or stop trading
		// We need to set these variables to unlive, m_strategyRunStatus,
		// m_isThreadTradeOngoing (in case we use mutiple threads)
		// We should also unsubscribe all symbols to stop receiving market data
		virtual void StopLive() = 0;

		// Report PNL, trades, ...
		virtual void ReportTradeResults(const std::string& symbol) = 0;

		// -We will control strategy parameters from external file...
		virtual void InitializeParameters(const std::string& strategyCfgPath) = 0;
		// -Can we trade now?
		StrategyRunStatus GetStrategyRunStatus() const;
		StrategyType GetStrategyType() const;
		void SetStrategyType(const StrategyType strategyType);
		std::string GetStrategyTypeStr(const StrategyType strategyType);
		const std::string& GetStrategyName() const;
		const std::string& GetStrategyID() const;
		// Sets up the lifetime of the trading strategy based on the provided XML configuration.
		// The configuration defines whether the strategy runs intra-day, intra-week, or intra-month.
		void SetupStrategyLifeTime(tinyxml2::XMLDocument* strategyCfgPathXml);

		// Checks if the trading strategy has not exceeded the defined trading rules.
		// This ensures compliance with exchange regulations and prevents violations.
		bool IsNotIsNotExceededTradingRules() const;

		// Increases the counter for REST API requests made to the exchange for compliance tracking.
		// This helps monitor and limit the number of requests to avoid exceeding API rate limits.
		void IncreaseComplianceRestAPIRequestCounter(const size_t noOfRequests);
protected:
		// Logs the hard limits for trading, such as maximum orders or API requests allowed.
		// This is useful for debugging and ensuring the strategy operates within defined constraints.
		void LogTradingHardLimits();

#if USE_BACK_TEST_TRADING
		// Handles messages received from the exchange simulator during backtesting.
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
		UserSpotAccount::BinanceTrader* m_trader{ nullptr }; // user account and trade actions
		ComplianceNRegulatory::BinanceTradingRules* m_tradingRules{ nullptr }; // exchange compliance and regulatory
		std::unique_ptr<CompilanceChecker> m_compilanceChecker; // reset trading hard limits from exchange
		std::unique_ptr<LibraryUtils::Logger> m_logger; // log message
		StrategyType m_strategyType { StrategyType::UNDEF};
		StrategyLifeTime m_StrategyLifeTime { StrategyLifeTime::INTRA_DAY };
		// For strategies that run in a same thread with main thread
		// then we can check this flag to know if the strategy is still live
		StrategyRunStatus m_strategyRunStatus{ StrategyRunStatus::UNDEF };
		// For strategies that need to be run in a separated thread
		// then it will need to maintain a TradingLoop() function to 
		// keep the thread live, this hack will help to keep the while loop running
#if USE_MULTITHREADING
		std::atomic<bool> m_isThreadTradeOngoing; // non blocking
#endif
		// Config for strategy, we will use it to setup strategy parameters
		std::unique_ptr<tinyxml2::XMLDocument> m_strategyCfgXml;
	};
};