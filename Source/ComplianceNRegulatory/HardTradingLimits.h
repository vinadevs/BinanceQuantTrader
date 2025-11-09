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
// 100 orders per 10 seconds
// 200,000 orders per 24 hours

namespace ComplianceNRegulatory {
	struct DLL_CLASS_COMPLIANCENREGULATORY_EXPORTS HardTradingLimits final
	{
		HardTradingLimits() = default;
		HardTradingLimits(
			const size_t requestWeightPerMinute,
			const size_t ordersPerTenSeconds,
			const size_t ordersPerTwentyFourHours) :
			m_maxRequestWeightPerMinute(requestWeightPerMinute),
			m_maxOrdersPerTenSeconds(ordersPerTenSeconds),
			m_maxOrdersPerTwentyFourHours(ordersPerTwentyFourHours) {
		}
		size_t m_maxRequestWeightPerMinute{ 3000 };
		size_t m_maxOrdersPerTenSeconds{ 100 };
		size_t m_maxOrdersPerTwentyFourHours{ 200000 };
	};
};

