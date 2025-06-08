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
#include "../LibraryUtils/StringUtils.h"
#include "../LibraryUtils/PathUtils.h"

#include "UserAccountManager.h"

#include <cassert>
#include <filesystem>

using namespace ExchangeSimulator;

UserAccountManager::UserAccountManager(const tinyxml2::XMLElement* userAccountManagerCfg)
{
    assert(userAccountManagerCfg);
    const auto* generalSettingXml = userAccountManagerCfg->FirstChildElement("GeneralSetting");
    assert(generalSettingXml);
    m_maxAccount = generalSettingXml->Unsigned64Attribute("MaximumAccount");

    const auto* accountInfoXml = userAccountManagerCfg->FirstChildElement("AccountInfo");
    assert(accountInfoXml);
    std::string accountInfoJsonFile(accountInfoXml->Attribute("File"));
    PathUtils::ReplaceSubString(accountInfoJsonFile, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());

    const auto* testUserAccountXml = userAccountManagerCfg->FirstChildElement("TestUserAccount");
    assert(testUserAccountXml);
    const auto* listUserIDStr = testUserAccountXml->Attribute("ListUserID");
    auto listUserID = StringUtils::ParseStringPairs(listUserIDStr);
    for (auto& userPair : listUserID) 
    {
        PathUtils::ReplaceSubString(userPair.second, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());
        if (std::filesystem::exists(userPair.second))
        {
            AddNewUserAccount(userPair.first, userPair.second, accountInfoJsonFile);
        }
        else
        {
            std::cout << "Path does not exist=" << userPair.second << "\n";
        }
    }
}

UserAccountManager::~UserAccountManager()
{
}

void UserAccountManager::AddNewUserAccount(
    const std::string& userId,
    const std::string& userConfigPath,
    const std::string& accountInfoJsonFile)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    if (m_accounts.size() <= m_maxAccount)
    {
        const auto result = m_accounts.try_emplace(userId, std::make_unique<UserAccount>(userConfigPath, accountInfoJsonFile));
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

UserAccount* UserAccountManager::LookupUserAccount(const std::string& userId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    const auto it = m_accounts.find(userId);
    if (it != m_accounts.end()) 
    {
        return it->second.get();
    }
    throw std::runtime_error("No UserAccount found with userId '" + userId + "'.");
}

UserAccount* UserAccountManager::OpenEditSessionForUserAccount(const std::string& userId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_accounts.find(userId);
    if (it != m_accounts.end())
    {
        return it->second.get();
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
