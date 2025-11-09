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
	
	/**
	 * @class UserTradeProfile
	 * @brief Represents a user's trade profile in the exchange simulator.
	 *
	 * This class encapsulates user-specific trading information such as account ID and leverage rate.
	 * It can be extended to include more user-specific trading parameters in the future.
	 */
	class UserTradeProfile final
	{
	public:
		UserTradeProfile(const std::string& userAccountId);
		~UserTradeProfile();

		const std::string& GetUserAccountId() const { return m_userAccountId; }

		double GetLeverageRate() const { return m_leverageRate; }

		void SetLeverageRate(double leverageRate) {
			m_leverageRate = leverageRate;
		}
	private:
		std::string m_userAccountId;
		double m_leverageRate{ 1.0 }; // Default leverage rate
	};
}