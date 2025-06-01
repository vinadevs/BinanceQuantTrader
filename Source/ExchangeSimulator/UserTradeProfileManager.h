/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include <string>
#include <unordered_map>

namespace tinyxml2 {
	class XMLElement;
}

namespace ExchangeSimulator {

	class UserTradeProfile;

	/**
	 * @class UserTradeProfileManager
	 * @brief Manages user trade profiles for futures trading in the exchange simulator.
	 *
	 * This class is responsible for adding and looking up user trade profiles based on user account IDs.
	 * It uses an unordered map to store user trade profiles for efficient access.
	 */
	class UserTradeProfileManager final 
	{
	public:
		UserTradeProfileManager(const tinyxml2::XMLElement* exchangeInfoManagerCfg);
		~UserTradeProfileManager();
		bool AddUserTradeProfile(const UserTradeProfile& userTradeProfile);
		UserTradeProfile& LookupUserTradeProfile(const std::string& userAccountId);
	private:
		std::unordered_map<std::string, UserTradeProfile> m_userTradeProfiles;
	};
}