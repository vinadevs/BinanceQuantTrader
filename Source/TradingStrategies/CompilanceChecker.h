/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "../LibraryUtils/AlarmSystem.h"

namespace TradingStrategies {
	static constexpr long tenSecondMs = 10000;
	static constexpr long safeThreholdMs = 2000; // to make sure we will not be banned!
	class OrdersPerTenSecondsChecker : public LibraryUtils::AlarmSystem
	{
	public:
		OrdersPerTenSecondsChecker(const AlarmSystem::AlarmMode mode)
			: LibraryUtils::AlarmSystem(tenSecondMs - safeThreholdMs, mode) {}
	};

	static constexpr long twentyFourHoursMs = 86400000;
	static constexpr long safeThreholdMinDay = 8100000;
	class OrdersPerTwentyFourHoursChecker : public LibraryUtils::AlarmSystem
	{
	public:
		OrdersPerTwentyFourHoursChecker(const AlarmSystem::AlarmMode mode)
			: LibraryUtils::AlarmSystem(twentyFourHoursMs, mode) {}
	};

	static constexpr long minuteMs = 60000;
	static constexpr long safeThreholdMin = 10000;
	class RequestWeightPerMinuteChecker : public LibraryUtils::AlarmSystem
	{
	public:
		RequestWeightPerMinuteChecker(const AlarmSystem::AlarmMode mode)
			: LibraryUtils::AlarmSystem(minuteMs, mode) {}
	};
};