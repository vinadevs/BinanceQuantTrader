/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include "../LibraryUtils/MacroUtils.h"

#include <string>

/**
 * @class ApiKeyInfoManager
 * @brief Manages binance user account information, including sensitive keys and user IDs.
 * https://www.binance.com/en-JP/my/dashboard
 * The `ApiKeyInfoManager` class provides an interface to store, retrieve, and manage
 * user account information, such as user IDs, secret keys (SK), and public keys (PK).
 * This information is essential for authentication and interaction with the exchange simulator.
 *
 * @note This class handles sensitive data (secret keys), so ensure proper security measures
 * are in place when using it in production environments.
 */

namespace RestAPI {

	struct DLL_CLASS_RESTAPI_EXPORTS ApiKeyInfo final
	{
		std::string m_userID;
		std::string m_sk;
		std::string m_pk;

		ApiKeyInfo() = default;

		ApiKeyInfo(
			const std::string& userID,
			const std::string& sk,
			const std::string& pk)
			: m_userID(userID), m_sk(sk), m_pk(pk) {}
	};

	class DLL_CLASS_RESTAPI_EXPORTS ApiKeyInfoManager final
	{
	public:
		ApiKeyInfoManager() = default;
		~ApiKeyInfoManager();

		DISABLE_COPY_AND_MOVE(ApiKeyInfoManager);

		static ApiKeyInfoManager* GetInstance();

		void InitApiKeyInfo(
			const std::string& userID,
			const std::string& sk,
			const std::string& pk);
		const ApiKeyInfo& GetApiKeyInfo() const;
	private:
		ApiKeyInfo m_ApiKeyInfo;
	};
};
// Lets shorten the code line!
#define ApiKeyInfoMgr RestAPI::ApiKeyInfoManager::GetInstance()