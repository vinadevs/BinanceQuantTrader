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