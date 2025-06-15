/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "UserSpotAccount.h"
#include "UserFutureAccount.h"

#include <memory>
#include <mutex>
#include <map>
#include <string>

namespace LibraryUtils {
    class Logger;
};

namespace tinyxml2 {
    class XMLElement;
};

namespace ExchangeSimulator {

	/**
	 * @class UserAccountManager
	 * @brief Manages user accounts in the trading system.
	 *
	 * This class is responsible for handling user login information, managing cash balances,
	 * tracking cryptocurrency quantities, and providing methods to interact with user account data.
	 * It ensures secure access to account information and performs necessary validations for
	 * account-related operations within the trading system.
	 *
	 * Key Responsibilities:
	 * - Authenticate users during login.
	 * - Store and manage user cash balances.
	 * - Maintain records of cryptocurrency holdings per user.
	 * - Provide methods to deposit, withdraw, or update account balances and crypto quantities.
	 * - Support secure and efficient account operations.
	 */

	 // Spot user accounts are stored in UserSpotAccount
    using SpotAccounts = std::map<std::string, std::unique_ptr<UserSpotAccount>>;
	// Future user accounts are stored in UserFutureAccount
	using FutureAccounts = std::map<std::string, std::unique_ptr<UserFutureAccount>>;

    class UserAccountManager final
    {
    public:
        UserAccountManager(const tinyxml2::XMLElement* userAccountManagerCfg);
        ~UserAccountManager();

        void AddNewSpotUserAccount(
            const std::string& userId,
            const std::string& userConfigPath,
            const std::string& accountInfoJsonFile);

		void AddNewFutureUserAccount(
			const std::string& userId,
			const std::string& userConfigPath,
			const std::string& accountInfoJsonFile);

        void RemoveSpotUserAccount(const std::string& userId);

		void RemoveFutureUserAccount(const std::string& userId);

        UserSpotAccount* LookupSpotUserAccount(const std::string& userId);

		UserFutureAccount* LookupFutureUserAccount(const std::string& userId);

        UserSpotAccount* OpenEditSessionForSpotUserAccount(const std::string& userId);

		UserFutureAccount* OpenEditSessionForFutureUserAccount(const std::string& userId);

        void ClearAll();

        const SpotAccounts& GetUserSpotAccounts();

		const FutureAccounts& GetUserFutureAccounts();
    protected:
        std::unique_ptr<LibraryUtils::Logger> m_logger;
		SpotAccounts m_spotAccounts;
		FutureAccounts m_futureAccounts;
        std::mutex m_mutex;
        std::size_t m_maxAccount{ 0 };
    };
};

