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

#include <memory>
#include <filesystem>
#include <fstream>

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

namespace UserAccount {

	// The FutureTradeReporter class is responsible for generating reports on trading activities.
	class FutureTradeReporter
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

		virtual void SetupReporter(const tinyxml2::XMLElement* reportCfg) = 0;
		virtual void UpdateRemoteReportTrades(const std::string& symbol) = 0;
		virtual void UpdateRemoteReportOpenOrders(const std::string& symbol) = 0;
		virtual void UpdateRemoteReportAccountBalance(const std::string& symbol) = 0;
		virtual void UpdateRemoteReportExchangerPriceForOrders(const std::string& symbol) = 0;
		virtual void UpdateRemoteReportCalculateLossForOrders(const std::string& symbol) = 0;
		virtual void DoRemoteExecutionReport(const std::string& symbol) = 0;
		virtual void DoLocalExecutionReport(const std::string& symbol) = 0;
		virtual void DoTradeExecutionReport(const std::string& symbol) = 0;
		virtual void UpdateRemoteData(const std::string& symbol) = 0;

	protected:
		virtual bool MergeLocalAndRemmoteReport() = 0;

		ComplianceNRegulatory::BinanceExchangeProfileMgr* m_exchangeProfileMgr{ nullptr };
		OrderManagement::PositionManager* m_positionManager{ nullptr };
		std::unique_ptr<LibraryUtils::Logger> m_logger;

		bool m_enableLastDayTradeReporter{ false };
		bool m_enableOpenOrderReporter{ false };
		bool m_enableBalanceReporter{ false };
		bool m_enableExchangerPriceForOrdersReporter{ false };
		bool m_enableCalculateLossForOrdersReporter{ false };

		// future account info
		KernelTrading::UserFutureAccount* m_accountInfo{ nullptr };
		// external file to store report
		std::string m_reportToFilePath;
	};
};

