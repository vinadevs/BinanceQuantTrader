/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/Logger.h"

#include "UserAccountManager.h"

#include <cassert>

using namespace ExchangeSimulator;

UserAccountManager::UserAccountManager(const tinyxml2::XMLElement* userAccountManagerCfg)
{
    assert(userAccountManagerCfg);
    const auto* generalSettingXml = userAccountManagerCfg->FirstChildElement("GeneralSetting");
    assert(generalSettingXml);
    m_maxAccount = generalSettingXml->Unsigned64Attribute("MaximumAccount");
}

UserAccountManager::~UserAccountManager()
{
}

void UserAccountManager::AddNewUserAccount(const std::string& userId, const UserAccount& userAccount)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    if (m_accounts.size() <= m_maxAccount)
    {
        const auto result = m_accounts.try_emplace(userId, userAccount);
        if (!result.second)
        {
            LOG_WARNING_STREAM(m_logger, "UserAccount with userId '" << userId << "' already exists.");
        }
    }
    else
    {
        LOG_ERROR_STREAM(m_logger, "Exceeded maximum account setting.");
    }
}

void UserAccountManager::RemoveUserAccount(const std::string& userId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    const auto it = m_accounts.find(userId);
    if (it != m_accounts.end())
    {
        m_accounts.erase(it);
        LOG_INFO_STREAM(m_logger, "UserAccount with userId '" << userId << "' removed successfully.");
    }
    else
    {
        throw std::runtime_error("No UserAccount found with userId '" + userId + "'.");
    }
}

const UserAccount& UserAccountManager::LookupUserAccount(const std::string& userId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    const auto it = m_accounts.find(userId);
    if (it != m_accounts.end()) 
    {
        LOG_INFO_STREAM(m_logger, "UserAccount with userId '" << userId << "' found successfully.");
        return it->second;
    }
    throw std::runtime_error("No UserAccount found with userId '" + userId + "'.");
}

UserAccount& UserAccountManager::OpenEditSessionForUserAccount(const std::string& userId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_accounts.find(userId);
    if (it != m_accounts.end())
    {
        LOG_INFO_STREAM(m_logger, "UserAccount with userId '" << userId << "' found successfully.");
        return it->second;
    }
    throw std::runtime_error("No UserAccount found with userId '" + userId + "'.");
}

void UserAccountManager::ClearAll()
{
    std::lock_guard<std::mutex> lock(m_mutex);
	m_accounts.clear();
}

const Accounts& UserAccountManager::GetUserAccounts()
{
	return m_accounts;
}
