/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "../LibraryUtils/AlarmSystem.h"

namespace LibraryUtils {
	class Logger;
};

namespace ComplianceNRegulatory {
	class BinanceTradingRules;
}

namespace TradingStrategies {
	
	// This class checks the number of orders placed within a 10-second interval
	class OrdersPerTenSecondsChecker final
		: public LibraryUtils::AlarmSystem
	{
	public:
		OrdersPerTenSecondsChecker(
			ComplianceNRegulatory::BinanceTradingRules* tradingRules,
			LibraryUtils::Logger* logger,
			const AlarmSystem::AlarmMode mode);

		virtual ~OrdersPerTenSecondsChecker();

		void OnAlarmTriggered(const int passToDerived = 0) override;
	private:
		ComplianceNRegulatory::BinanceTradingRules* m_tradingRules{ nullptr };
		LibraryUtils::Logger* m_logger{ nullptr };
	};

	///////////////////////////////////////////////////////////////

	// This class monitors the request weight per minute to ensure compliance with API limits
	class RequestWeightPerMinuteChecker final 
		: public LibraryUtils::AlarmSystem
	{
	public:
		RequestWeightPerMinuteChecker(
			ComplianceNRegulatory::BinanceTradingRules* tradingRules,
			LibraryUtils::Logger* logger,
			const LibraryUtils::AlarmSystem::AlarmMode mode);

		virtual ~RequestWeightPerMinuteChecker();

		void OnAlarmTriggered(const int passToDerived = 0) override;
	private:
		ComplianceNRegulatory::BinanceTradingRules* m_tradingRules{ nullptr };
		LibraryUtils::Logger* m_logger{ nullptr };
	};

	///////////////////////////////////////////////////////////////

	// This class checks the number of orders placed within a 24-hour period
	class OrdersPerTwentyFourHoursChecker final 
		: public LibraryUtils::AlarmSystem
	{
	public:
		OrdersPerTwentyFourHoursChecker(
			ComplianceNRegulatory::BinanceTradingRules* tradingRules,
			LibraryUtils::Logger* logger,
			const LibraryUtils::AlarmSystem::AlarmMode mode);

		virtual ~OrdersPerTwentyFourHoursChecker();

		void OnAlarmTriggered(const int passToDerived = 0) override;
	private:
		ComplianceNRegulatory::BinanceTradingRules* m_tradingRules{ nullptr };
		LibraryUtils::Logger* m_logger{ nullptr };
	};

	///////////////////////////////////////////////////////////////

	// This class manages and starts alarms for various trading rule checks
	class CompilanceChecker final
	{
	public:
		CompilanceChecker();
		~CompilanceChecker();
		void StartAlarmOnTradingRules(
			ComplianceNRegulatory::BinanceTradingRules* tradingRules,
			const LibraryUtils::AlarmSystem::AlarmMode mode,
			const bool enablePerTwentyFourHoursChecker);
	private:
		std::unique_ptr<OrdersPerTenSecondsChecker> m_ordersPerTenSecondsChecker;
		std::unique_ptr<RequestWeightPerMinuteChecker> m_requestWeightPerMinuteChecker;
		std::unique_ptr<OrdersPerTwentyFourHoursChecker> m_ordersPerTwentyFourHoursChecker;
		std::unique_ptr<LibraryUtils::Logger> m_logger;
	};
};