/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "../RestAPI/RestAPI.h"
#include "../KernelTrading/user_future_account.h"

#include "BaseReporter.h"

namespace tinyxml2 {
	class XMLElement;
};

namespace LibraryUtils {
	class Logger;
};

namespace ComplianceNRegulatory {
	class BinanceExchangeProfileMgr;
}

namespace OrderManagement {
	class PositionManager;
}

namespace KernelTrading {
	class UserFutureAccount;
}

namespace UserAccount {

	// The FutureTradeReporter class is responsible for generating reports on future trading activities.
	class FutureTradeReporter : public BaseReporter
	{
	public:
		FutureTradeReporter(
			KernelTrading::UserFutureAccount* accountInfo,
			ComplianceNRegulatory::BinanceExchangeProfileMgr* exchangeProfileMgr,
			OrderManagement::PositionManager* positionManager)
			: m_accountInfo(accountInfo),
			m_exchangeProfileMgr(exchangeProfileMgr),
			m_positionManager(positionManager) {
		}

		virtual ~FutureTradeReporter() {};

	protected:
		ComplianceNRegulatory::BinanceExchangeProfileMgr* m_exchangeProfileMgr{ nullptr };
		OrderManagement::PositionManager* m_positionManager{ nullptr };

		// future account info
		KernelTrading::UserFutureAccount* m_accountInfo{ nullptr };
	};
};

