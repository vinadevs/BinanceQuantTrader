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

	// Load spot user accounts from XML configuration
    const auto* testSpotUserAccountXml = userAccountManagerCfg->FirstChildElement("TestSpotUserAccount");
    assert(testSpotUserAccountXml);
    const auto* listUserIDStr = testSpotUserAccountXml->Attribute("ListUserID");
    auto listUserID = StringUtils::ParseStringPairs(listUserIDStr);
    for (auto& userPair : listUserID) 
    {
        PathUtils::ReplaceSubString(userPair.second, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());
        if (std::filesystem::exists(userPair.second))
        {
            AddNewSpotUserAccount(userPair.first, userPair.second, accountInfoJsonFile);
        }
        else
        {
            std::cout << "Path does not exist=" << userPair.second << "\n";
        }
    }

	// Load future user accounts from XML configuration
    const auto* testFutureUserAccountXml = userAccountManagerCfg->FirstChildElement("TestFutureUserAccount");
	assert(testFutureUserAccountXml);
	const auto* listFutureUserIDStr = testFutureUserAccountXml->Attribute("ListUserID");
	auto listFutureUserID = StringUtils::ParseStringPairs(listFutureUserIDStr);
	for (auto& userPair : listFutureUserID)
	{
		PathUtils::ReplaceSubString(userPair.second, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());
		if (std::filesystem::exists(userPair.second))
		{
			AddNewFutureUserAccount(userPair.first, userPair.second, accountInfoJsonFile);
		}
		else
		{
			std::cout << "Path does not exist=" << userPair.second << "\n";
		}
	}
}

UserAccountManager::~UserAccountManager()
{
	m_logger->Info("UserAccountManager destroyed.");
}

void UserAccountManager::AddNewSpotUserAccount(
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

void UserAccountManager::AddNewFutureUserAccount(const std::string& userId, const std::string& userConfigPath, const std::string& accountInfoJsonFile)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	if (m_futureAccounts.size() <= m_maxAccount)
	{
		const auto result = m_futureAccounts.try_emplace(userId, std::make_unique<UserAccountFuture>(userConfigPath, accountInfoJsonFile));
		if (!result.second)
		{
			LOG_WARNING_STREAM(m_logger, "UserAccountFuture with userId '" << userId << "' already exists.");
		}
	}
	else
	{
		LOG_ERROR_STREAM(m_logger, "Exceeded maximum account setting.");
	}
}

void UserAccountManager::RemoveSpotUserAccount(const std::string& userId)
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

void UserAccountManager::RemoveFutureUserAccount(const std::string& userId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	const auto it = m_futureAccounts.find(userId);
	if (it != m_futureAccounts.end())
	{
		m_futureAccounts.erase(it);
		LOG_INFO_STREAM(m_logger, "UserAccountFuture with userId '" << userId << "' removed successfully.");
	}
	else
	{
		throw std::runtime_error("No UserAccountFuture found with userId '" + userId + "'.");
	}
}

UserAccount* UserAccountManager::LookupSpotUserAccount(const std::string& userId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    const auto it = m_accounts.find(userId);
    if (it != m_accounts.end()) 
    {
        return it->second.get();
    }
    throw std::runtime_error("No UserAccount found with userId '" + userId + "'.");
}

UserAccountFuture* UserAccountManager::LookupFutureUserAccount(const std::string& userId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	auto it = m_futureAccounts.find(userId);
	if (it != m_futureAccounts.end())
	{
		return it->second.get();
	}
	throw std::runtime_error("No UserAccountFuture found with userId '" + userId + "'.");
}

UserAccount* UserAccountManager::OpenEditSessionForSpotUserAccount(const std::string& userId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_accounts.find(userId);
    if (it != m_accounts.end())
    {
        return it->second.get();
    }
    throw std::runtime_error("No UserAccount found with userId '" + userId + "'.");
}

UserAccountFuture* UserAccountManager::OpenEditSessionForFutureUserAccount(const std::string& userId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	auto it = m_futureAccounts.find(userId);
	if (it != m_futureAccounts.end())
	{
		return it->second.get();
	}
	throw std::runtime_error("No UserAccountFuture found with userId '" + userId + "'.");
}

void UserAccountManager::ClearAll()
{
    std::lock_guard<std::mutex> lock(m_mutex);
	m_accounts.clear();
}

const Accounts& UserAccountManager::GetUserSpotAccounts()
{
	return m_accounts;
}

const FutureAccounts& UserAccountManager::GetUserFutureAccounts()
{
	return m_futureAccounts;
}
