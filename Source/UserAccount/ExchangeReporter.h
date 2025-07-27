/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "../RestAPI/RestAPI.h"

#include "BaseReporter.h"

#include <memory>
#include <filesystem>
#include <fstream>

namespace tinyxml2 {
	class XMLElement;
};

namespace ComplianceNRegulatory {
	class BinanceExchangeProfileMgr;
}

namespace OrderManagement {
	class PositionManager;
}

namespace UserAccount {
	
	// The ExchangeReporter class is responsible for generating reports on trading activities.
	class ExchangeReporter : public BaseReporter
	{
	public:
		ExchangeReporter(
			binapi::rest::account_info_t* accountInfo,
			ComplianceNRegulatory::BinanceExchangeProfileMgr* exchangeProfileMgr,
			OrderManagement::PositionManager* positionManager)
			: m_accountInfo(accountInfo),
			m_exchangeProfileMgr(exchangeProfileMgr),
			m_positionManager(positionManager) {}

		virtual ~ExchangeReporter() {};

		virtual void UpdateRemoteReportTrades(const std::string& symbol) {};
		virtual void UpdateRemoteReportOpenOrders(const std::string& symbol) {};
		virtual void UpdateRemoteReportAccountBalance(const std::string& symbol) {};
		virtual void UpdateRemoteReportExchangerPriceForOrders(const std::string& symbol) {};
		virtual void UpdateRemoteReportCalculateLossForOrders(const std::string& symbol) {};
		virtual void DoRemoteExecutionReport(const std::string& symbol) {};
		virtual void DoLocalExecutionReport(const std::string& symbol) {};

	protected:
		virtual bool MergeLocalAndRemmoteReport() { return true; };

		ComplianceNRegulatory::BinanceExchangeProfileMgr* m_exchangeProfileMgr{ nullptr };
		OrderManagement::PositionManager* m_positionManager{ nullptr };

		bool m_enableLastDayTradeReporter{ false };
		bool m_enableOpenOrderReporter{ false };
		bool m_enableBalanceReporter{ false };
		bool m_enableExchangerPriceForOrdersReporter{ false };
		bool m_enableCalculateLossForOrdersReporter{ false };

		// restAPI data
		binapi::rest::account_info_t* m_accountInfo;
	};
};

