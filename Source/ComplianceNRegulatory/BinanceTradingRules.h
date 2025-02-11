/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include <memory>
#include <mutex>

namespace tinyxml2 {
	class XMLElement;
};

// https://www.binance.com/en/support/faq/frequently-asked-questions-on-api-360004492232
// Binance Hard - Limits
// 6,000 request weight per minute(keep in mind that this is not necessarily the same as 6, 000 requests)
// 50 orders per 10 seconds
// 160,000 orders per 24 hours

namespace ComplianceNRegulatory {
	struct DLL_CLASS_COMPLIANCENREGULATORY_EXPORTS HardTradingLimits final
	{
		HardTradingLimits(
			const size_t requestWeightPerMinute,
			const size_t ordersPerTenSeconds,
			const size_t ordersPerTwentyFourHours) :
			m_requestWeightPerMinute(requestWeightPerMinute),
			m_ordersPerTenSeconds(ordersPerTenSeconds),
			m_ordersPerTwentyFourHours(ordersPerTwentyFourHours) {}
		size_t m_requestWeightPerMinute { 6000 };
		size_t m_ordersPerTenSeconds{ 50 };
		size_t m_ordersPerTwentyFourHours{ 160000 };
	};

	// -Trading Rules from an Exchange are guidelines and
	// rules established by the exchange to ensure fair,
	// orderly, and transparent markets.These rules are essential 
	// for protecting investors, maintaining market stability, and
	// enforcing compliance among participants.Each exchange, 
	// including Binance Exchange, has specific 
	// trading rules that tradersand institutions must follow.

    // NOTE: PLEASE DO NOT CALL REST API UPDATES MANY TIMES/SECONDS
	// AS BINANCE WILL BAN THE LOCAL IP FOR THAT SPAM
	// REMEMBER ALWAY IMPLEMENT A CHECKER BEFORE CALL REST API
	class DLL_CLASS_COMPLIANCENREGULATORY_EXPORTS BinanceTradingRules final
	{
	public:
		BinanceTradingRules(const tinyxml2::XMLElement* tradingRuleConfigXml);
		~BinanceTradingRules();

		HardTradingLimits* GetTradingLimits() const {
			return m_limits.get();
		};

		void IncreaseRequestWeightPerMinute();
		void IncreaseOrdersPerTenSeconds();
		void IncreaseOrdersPerTwentyFourHours();

		void ResetRequestWeightPerMinuteCounter();
		void ResetOrdersPerTenSecondsCounter();
		void ResetOrdersPerTwentyFourHoursCounter();

		bool IsNotExceededRequestWeightPerMinute();
		bool IsNotExceededOrdersPerTenSeconds();
		bool IsNotExceededOrdersPerTwentyFourHours();
	private:
		std::mutex m_mutex; // for thread safe
		std::unique_ptr<HardTradingLimits> m_limits;
		size_t m_requestWeightPerMinuteCounter{ 0 };
		size_t m_ordersPerTenSecondsCounter{ 0 };
		size_t m_ordersPerTwentyFourHoursCounter{ 0 };


	};
};

