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
#include "../LibraryUtils/FileUtils.h"

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

	// Load user accounts from XML configuration
    const auto* testUserAccountXml = userAccountManagerCfg->FirstChildElement("TestUserAccount");
    assert(testUserAccountXml);
    const auto* listUserIDStr = testUserAccountXml->Attribute("ListUserID");
    m_userAccountIds = StringUtils::ParseStringPairs(listUserIDStr);

    const auto* spotAccountInfoXml = userAccountManagerCfg->FirstChildElement("SpotAccountInfo");
    assert(spotAccountInfoXml);
    std::string spotAccountInfoJsonFile(spotAccountInfoXml->Attribute("File"));
    PathUtils::ReplaceSubString(spotAccountInfoJsonFile, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());

    for (auto& userPair : m_userAccountIds)
    {
        PathUtils::ReplaceSubString(userPair.second, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());
        if (std::filesystem::exists(userPair.second))
        {
            AddNewSpotUserAccount(userPair.first, userPair.second, spotAccountInfoJsonFile);
        }
        else
        {
            std::cout << "Path does not exist=" << userPair.second << "\n";
        }
    }

    const auto* futureAccountInfoXml = userAccountManagerCfg->FirstChildElement("FutureAccountInfo");
    assert(futureAccountInfoXml);
    std::string futureAccountInfoJsonFile(futureAccountInfoXml->Attribute("File"));
    PathUtils::ReplaceSubString(futureAccountInfoJsonFile, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());

    for (auto& userPair : m_userAccountIds)
	{
		PathUtils::ReplaceSubString(userPair.second, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());
		if (std::filesystem::exists(userPair.second))
		{
			AddNewFutureUserAccount(userPair.first, userPair.second, futureAccountInfoJsonFile);
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
    if (m_spotAccounts.size() <= m_maxAccount)
    {
        const auto result = m_spotAccounts.try_emplace(
            userId, std::make_unique<UserSpotAccount>(userConfigPath, accountInfoJsonFile));
        if (!result.second)
        {
            LOG_WARNING_STREAM(m_logger, "UserSpotAccount with userId '" << userId << "' already exists.");
        }
    }
    else
    {
        LOG_ERROR_STREAM(m_logger, "Exceeded maximum account setting.");
    }
}

void UserAccountManager::AddNewFutureUserAccount(
    const std::string& userId,
    const std::string& userConfigPath,
    const std::string& accountInfoJsonFile)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	if (m_futureAccounts.size() <= m_maxAccount)
	{
		const nlohmann::json jsonData = nlohmann::json::parse(FileUtils::ReadFileContent(accountInfoJsonFile));
		const auto result = m_futureAccounts.try_emplace(
            userId, std::make_unique<KernelTrading::UserFutureAccount>(userId, userConfigPath, jsonData));
		if (!result.second)
		{
			LOG_WARNING_STREAM(m_logger, "UserFutureAccount with userId '" << userId << "' already exists.");
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
    const auto it = m_spotAccounts.find(userId);
    if (it != m_spotAccounts.end())
    {
        m_spotAccounts.erase(it);
        LOG_INFO_STREAM(m_logger, "UserSpotAccount with userId '" << userId << "' removed successfully.");
    }
    else
    {
        throw std::runtime_error("No UserSpotAccount found with userId '" + userId + "'.");
    }
}

void UserAccountManager::RemoveFutureUserAccount(const std::string& userId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	const auto it = m_futureAccounts.find(userId);
	if (it != m_futureAccounts.end())
	{
		m_futureAccounts.erase(it);
		LOG_INFO_STREAM(m_logger, "UserFutureAccount with userId '" << userId << "' removed successfully.");
	}
	else
	{
		throw std::runtime_error("No UserFutureAccount found with userId '" + userId + "'.");
	}
}

const UserSpotAccount* UserAccountManager::LookupSpotUserAccount(const std::string& userId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    const auto it = m_spotAccounts.find(userId);
    if (it != m_spotAccounts.end()) 
    {
        return it->second.get();
    }
    throw std::runtime_error("No UserSpotAccount found with userId '" + userId + "'.");
}

const KernelTrading::UserFutureAccount* UserAccountManager::LookupFutureUserAccount(const std::string& userId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	auto it = m_futureAccounts.find(userId);
	if (it != m_futureAccounts.end())
	{
		return it->second.get();
	}
	throw std::runtime_error("No UserFutureAccount found with userId '" + userId + "'.");
}

UserSpotAccount* UserAccountManager::OpenEditSessionForSpotUserAccount(const std::string& userId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_spotAccounts.find(userId);
    if (it != m_spotAccounts.end())
    {
        return it->second.get();
    }
    throw std::runtime_error("No UserSpotAccount found with userId '" + userId + "'.");
}

KernelTrading::UserFutureAccount* UserAccountManager::OpenEditSessionForFutureUserAccount(const std::string& userId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	auto it = m_futureAccounts.find(userId);
	if (it != m_futureAccounts.end())
	{
		return it->second.get();
	}
	throw std::runtime_error("No UserFutureAccount found with userId '" + userId + "'.");
}

void UserAccountManager::ClearAll()
{
    std::lock_guard<std::mutex> lock(m_mutex);
	m_spotAccounts.clear();
}

const SpotAccounts& UserAccountManager::GetUserSpotAccounts()
{
	return m_spotAccounts;
}

const FutureAccounts& UserAccountManager::GetUserFutureAccounts()
{
	return m_futureAccounts;
}
