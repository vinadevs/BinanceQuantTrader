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

#include <stdexcept>

using namespace ExchangeSimulator;

UserTradeProfileManager::UserTradeProfileManager(const tinyxml2::XMLElement* exchangeInfoManagerCfg)
{
}

UserTradeProfileManager::~UserTradeProfileManager()
{
}

bool UserTradeProfileManager::AddUserTradeProfile(const UserTradeProfile& userTradeProfile)
{
	return m_userTradeProfiles.try_emplace("307109623", UserTradeProfile("307109623")).second;
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
