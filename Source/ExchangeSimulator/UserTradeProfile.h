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
	
	class UserTradeProfile final
	{
	public:
		UserTradeProfile(const std::string& userAccountId);
		~UserTradeProfile();

		const std::string& GetUserAccountId() const { return m_userAccountId; }
		double GetLeverageRate() const { return 1.0; } // Placeholder for leverage rate, can be modified later
	private:
		std::string m_userAccountId;
	};
}