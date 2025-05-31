/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include <string>

namespace ExchangeSimulator {

	class UserTradeProfile;

	class FutureTradeManager final 
	{
	public:
		FutureTradeManager();
		~FutureTradeManager();
		UserTradeProfile* LookupUserTradeProfile(const std::string& userAccountId) const;
	};
}