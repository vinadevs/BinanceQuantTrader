/********************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"

// https://www.binance.com/en/support/faq/frequently-asked-questions-on-api-360004492232
// Binance Hard - Limits
// 6,000 request weight per minute(keep in mind that this is not necessarily the same as 6, 000 requests)
// 50 orders per 10 seconds
// 160,000 orders per 24 hours

namespace ComplianceNRegulatory {
	struct DLL_CLASS_COMPLIANCENREGULATORY_EXPORTS HardTradingLimits final
	{
		HardTradingLimits() = default;
		HardTradingLimits(
			const size_t requestWeightPerMinute,
			const size_t ordersPerTenSeconds,
			const size_t ordersPerTwentyFourHours) :
			m_requestWeightPerMinute(requestWeightPerMinute),
			m_ordersPerTenSeconds(ordersPerTenSeconds),
			m_ordersPerTwentyFourHours(ordersPerTwentyFourHours) {
		}
		size_t m_requestWeightPerMinute{ 6000 };
		size_t m_ordersPerTenSeconds{ 50 };
		size_t m_ordersPerTwentyFourHours{ 160000 };
	};
};

