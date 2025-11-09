/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "CompilanceChecker.h"

#include "../ComplianceNRegulatory/BinanceTradingRules.h"
#include "../LibraryUtils/Logger.h"

using namespace TradingStrategies;
using namespace ComplianceNRegulatory;
using namespace LibraryUtils;

static constexpr long tenSecondMs = 10000;
static constexpr long safeThreholdMs = 2000; // to make sure we will not be banned!

OrdersPerTenSecondsChecker::OrdersPerTenSecondsChecker(
	BinanceTradingRules* tradingRules,
	Logger* logger,
	const AlarmSystem::AlarmMode mode)
	: m_tradingRules(tradingRules), m_logger(logger), AlarmSystem(tenSecondMs - safeThreholdMs, mode) {}

OrdersPerTenSecondsChecker::~OrdersPerTenSecondsChecker() {}

void OrdersPerTenSecondsChecker::OnAlarmTriggered(const int passToDerived/*=0*/)
{
	// NOTE: PLEASE DO NOT CALL REST API UPDATES MANY TIMES/SECONDS
	// AS BINANCE WILL BAN THE LOCAL IP FOR THAT SPAM
	// PLEASE CHECK IN ComplianceNRegulatory CODE
	// REMEMBER ALWAY IMPLEMENT A CHECKER BEFORE CALL REST API
	m_tradingRules->ResetOrdersPerTenSecondsCounter(false);
	m_logger->Info("Reset limitation of orders per ten seconds.");
}

///////////////////////////////////////////////////////////////

static constexpr long minuteMs = 60000;
static constexpr long safeThreholdMin = 10000;

RequestWeightPerMinuteChecker::RequestWeightPerMinuteChecker(
	BinanceTradingRules* tradingRules,
	Logger* logger,
	const AlarmSystem::AlarmMode mode) 
	: m_tradingRules(tradingRules), m_logger(logger), AlarmSystem(minuteMs, mode) {}

RequestWeightPerMinuteChecker::~RequestWeightPerMinuteChecker() {}

void RequestWeightPerMinuteChecker::OnAlarmTriggered(const int passToDerived)
{
	// NOTE: PLEASE DO NOT CALL REST API UPDATES MANY TIMES/SECONDS
	// AS BINANCE WILL BAN THE LOCAL IP FOR THAT SPAM
	// PLEASE CHECK IN ComplianceNRegulatory CODE
	// REMEMBER ALWAY IMPLEMENT A CHECKER BEFORE CALL REST API
	m_tradingRules->ResetRequestWeightPerMinuteCounter(false);
	m_logger->Info("Reset limitation of requests per minute.");
}

///////////////////////////////////////////////////////////////

static constexpr long twentyFourHoursMs = 86400000;
static constexpr long safeThreholdMinDay = 8100000;

OrdersPerTwentyFourHoursChecker::OrdersPerTwentyFourHoursChecker(
	BinanceTradingRules* tradingRules,
	Logger* logger,
	const AlarmSystem::AlarmMode mode)
	: m_tradingRules(tradingRules), m_logger(logger), AlarmSystem(twentyFourHoursMs, mode) {}

OrdersPerTwentyFourHoursChecker::~OrdersPerTwentyFourHoursChecker() {}

void OrdersPerTwentyFourHoursChecker::OnAlarmTriggered(const int passToDerived)
{
	// NOTE: PLEASE DO NOT CALL REST API UPDATES MANY TIMES/SECONDS
	// AS BINANCE WILL BAN THE LOCAL IP FOR THAT SPAM
	// PLEASE CHECK IN ComplianceNRegulatory CODE
	// REMEMBER ALWAY IMPLEMENT A CHECKER BEFORE CALL REST API
	m_tradingRules->ResetOrdersPerTwentyFourHoursCounter(false);
	m_logger->Info("Reset limitation of orders per 24 hours.");
}

///////////////////////////////////////////////////////////////

CompilanceChecker::CompilanceChecker()
	: m_logger{ std::make_unique<Logger>("CompilanceChecker") } {}

CompilanceChecker::~CompilanceChecker() {}

void CompilanceChecker::StartAlarmOnTradingRules(
	BinanceTradingRules* tradingRules,
	const AlarmSystem::AlarmMode mode,
	const bool enablePerTwentyFourHoursChecker)
{
	m_ordersPerTenSecondsChecker 
		= std::make_unique<OrdersPerTenSecondsChecker>(tradingRules, m_logger.get(), mode);
	m_logger->Info("OrdersPerTenSecondsChecker started.");
	m_ordersPerTenSecondsChecker->Start();
	m_requestWeightPerMinuteChecker 
		= std::make_unique<RequestWeightPerMinuteChecker>(tradingRules, m_logger.get(), mode);
	m_requestWeightPerMinuteChecker->Start();
	m_logger->Info("RequestWeightPerMinuteChecker started.");
	if (enablePerTwentyFourHoursChecker) {
		m_ordersPerTwentyFourHoursChecker
			= std::make_unique<OrdersPerTwentyFourHoursChecker>(tradingRules, m_logger.get(), mode);
		m_ordersPerTwentyFourHoursChecker->Start();
		m_logger->Info("OrdersPerTwentyFourHoursChecker started.");
	}
}
