/********************************************************************************
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

namespace ComplianceNRegulatory {

	struct HardTradingLimits;
	class BinanceExchangeProfileMgr;

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

		BinanceExchangeProfileMgr* GetExchangeProfileMgr() const {
			return m_exchangeProfileMgr.get();
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
		std::unique_ptr<BinanceExchangeProfileMgr> m_exchangeProfileMgr;
	};
};

