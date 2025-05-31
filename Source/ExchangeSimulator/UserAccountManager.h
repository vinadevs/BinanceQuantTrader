/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "UserAccount.h"

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

    using Accounts = std::map<std::string, std::unique_ptr<UserAccount>>;

    class UserAccountManager
    {
    public:
        UserAccountManager(const tinyxml2::XMLElement* userAccountManagerCfg);
        ~UserAccountManager();

        void AddNewUserAccount(
            const std::string& userId,
            const std::string& userConfigPath,
            const std::string& accountInfoJsonFile);

        void RemoveUserAccount(const std::string& userId);

        const UserAccount* LookupUserAccount(const std::string& userId);

        UserAccount* OpenEditSessionForUserAccount(const std::string& userId);

        void ClearAll();

        const Accounts& GetUserAccounts();

		bool IsAccountHavingSufficientMargin(
			const std::string& symbol,
			const double requiredMarginCash) const;
    protected:
        std::unique_ptr<LibraryUtils::Logger> m_logger;
        Accounts m_accounts;
        std::mutex m_mutex;
        std::size_t m_maxAccount{ 0 };
    };
};

