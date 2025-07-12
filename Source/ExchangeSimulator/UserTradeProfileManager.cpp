/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "UserTradeProfileManager.h"
#include "UserTradeProfile.h"

#include "../SettingNConfig/tinyxml2.h"

#include <cassert>
#include <stdexcept>

using namespace ExchangeSimulator;

UserTradeProfileManager::UserTradeProfileManager(
	const tinyxml2::XMLElement* userTradeProfileManagerCfg,
	const std::vector<std::pair<std::string, std::string>>& userIds)
{
	assert(userTradeProfileManagerCfg);
	const auto* userTradeProfileManagerXml = userTradeProfileManagerCfg->FirstChildElement("FutureTradeSettings");
	assert(userTradeProfileManagerXml);
	const double leverageRate = userTradeProfileManagerXml->DoubleAttribute("LeverageRate");
	if (leverageRate <= 0.0 || leverageRate > 100.0)
	{
		throw std::runtime_error("Invalid leverage rate in UserTradeProfileManager configuration.");
	}
	for (const auto& userPair : userIds)
	{
		UserTradeProfile userTradeProfile(userPair.first);
		userTradeProfile.SetLeverageRate(leverageRate);
		if (!AddUserTradeProfile(userTradeProfile))
		{
			throw std::runtime_error("Failed to add user trade profile for user ID: " + userPair.first);
		}
	}
}

UserTradeProfileManager::~UserTradeProfileManager()
{
}

bool UserTradeProfileManager::AddUserTradeProfile(const UserTradeProfile& userTradeProfile)
{
	return m_userTradeProfiles.try_emplace(userTradeProfile.GetUserAccountId(), userTradeProfile).second;
}

UserTradeProfile& UserTradeProfileManager::LookupUserTradeProfile(const std::string& userAccountId)
{
	auto it = m_userTradeProfiles.find(userAccountId);
	if (it != m_userTradeProfiles.end())
	{
		return it->second;
	}
	throw std::runtime_error("User trade profile not found for user account ID: " + userAccountId);
}
