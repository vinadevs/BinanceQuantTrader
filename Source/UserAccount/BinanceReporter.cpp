/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../KernelTrading/errors.h"
#include "../RestAPI/RestAPI.h"
#include "../RestAPI/BinanceSpotApiGateWay.h"
#include "../LibraryUtils/Logger.h"
#include "../LibraryUtils/PathUtils.h"
#include "../LibraryUtils/StringUtils.h"
#include "../SettingNConfig/tinyxml2.h"
#include "../OrderManagement/PositionManager.h"
#include "../ComplianceNRegulatory/BinanceExchangeProfile.h"

#include "ReportAPIs.h"
#include "BinanceReporter.h"
#include "BinanceTrader.h"

#include <filesystem>

using namespace UserAccount;
using namespace OrderManagement;
using namespace RestAPI;

BinanceReporter::BinanceReporter(
	const tinyxml2::XMLElement* reportConfigXml,
	binapi::rest::account_info_t* accountInfo,
	ComplianceNRegulatory::BinanceExchangeProfileMgr* exchangeProfileMgr,
	PositionManager* positionManager)
	: ExchangeReporter(accountInfo, exchangeProfileMgr, positionManager)
{
	m_logger = std::make_unique<LibraryUtils::Logger>("BinanceReporter");
	SetupReporter(reportConfigXml);
	m_logger->Info("setting up trading reporter finished.");
}

BinanceReporter::~BinanceReporter() {}

void BinanceReporter::UpdateRemoteData(const std::string& symbol)
{
	const auto accountInfoResult = BinanceSpotApiGateWayMgr->account_info();
	if (!accountInfoResult)
	{
		LOG_ERROR_STREAM(m_logger, "account_info: ec=" << std::to_string(accountInfoResult.ec)
			<< ", ename=" << binapi::rest::e_error_to_string(accountInfoResult.ec)
			<< ", emsg=" << accountInfoResult.errmsg);
	}
	else
	{
		m_logger->Info("updating account info finished.");
		DEREF_V(m_accountInfo) = accountInfoResult.v;
	}
	const auto exchangeInfoResult = BinanceSpotApiGateWayMgr->exchange_info(symbol);
	if (!exchangeInfoResult)
	{
		LOG_ERROR_STREAM(m_logger, "exchange_info: ec=" << std::to_string(exchangeInfoResult.ec)
			<< ", ename=" << binapi::rest::e_error_to_string(exchangeInfoResult.ec)
			<< ", emsg=" << exchangeInfoResult.errmsg);
	}
	else
	{
		m_logger->Info("updating exchange info finished.");
		DEREF_V(m_exchangeProfileMgr->AccessRemoteExchangeProfile(symbol)) = exchangeInfoResult.v;
	}
}

void BinanceReporter::SetupReporter(const tinyxml2::XMLElement* reportCfg)
{
	if (reportCfg->BoolAttribute("TradeReport"))
	{
		m_enableLastDayTradeReporter = true;
	}
	if (reportCfg->BoolAttribute("OrderReport"))
	{
		m_enableOpenOrderReporter = true;
	}
	if (reportCfg->BoolAttribute("BalanceReport"))
	{
		m_enableBalanceReporter = true;
	}
	if (reportCfg->BoolAttribute("ExchangerPriceForOrdersReport"))
	{
		m_enableExchangerPriceForOrdersReporter = true;
	}
	if (reportCfg->BoolAttribute("CalculateLossForOrdersReport"))
	{
		m_enableCalculateLossForOrdersReporter = true;
	}

	std::string reportToFilePath(reportCfg->Attribute("ReportToFilePath"));
	PathUtils::ReplaceSubString(reportToFilePath, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());
	m_reportToFilePath = reportToFilePath;

	const std::string reportChannel(reportCfg->Attribute("ReportChannel"));
	m_reportChannel = FromReportChannelTextToEnum(reportChannel);
}

void BinanceReporter::UpdateRemoteReportTrades(const std::string& symbol)
{
	static const auto trades_report_cb = [&](const binapi::rest::order_info_t& o)
	{
		LOG_INFO_STREAM(m_logger, o.symbol << " - " << o.orderId);
	};
	if (m_reportChannel == ReportChannel::CONSOLE)
	{
		LOG_INFO_STREAM(m_logger, "********************* TRADES REPORT *********************************");
		binapi::make_trades_report_for_last_day(std::cout,
			*BinanceSpotApiGateWayMgr, DEREF_V(m_accountInfo),
			DEREF_V(m_exchangeProfileMgr->AccessRemoteExchangeProfile(symbol)),
			{ symbol }, trades_report_cb);
	}
	else if (m_reportChannel == ReportChannel::EXTERNAL_FILE)
	{
	}
	else if (m_reportChannel == ReportChannel::GUI_APP)
	{
	}
}

void BinanceReporter::UpdateRemoteReportOpenOrders(const std::string& symbol)
{
	LOG_INFO_STREAM(m_logger, "******************* OPEN ORDERS REPORT ******************************");
	binapi::make_open_orders_report(std::cout, 
		*BinanceSpotApiGateWayMgr, DEREF_V(m_exchangeProfileMgr->AccessRemoteExchangeProfile(symbol)), { symbol });
}

void BinanceReporter::UpdateRemoteReportAccountBalance(const std::string& symbol)
{
	LOG_INFO_STREAM(m_logger, "******************** USER BALANCE REPORT *********************************");
	binapi::make_balance_report(std::cout, 
		*BinanceSpotApiGateWayMgr, DEREF_V(m_accountInfo),
		DEREF_V(m_exchangeProfileMgr->AccessRemoteExchangeProfile(symbol)));
}

void BinanceReporter::UpdateRemoteReportExchangerPriceForOrders(const std::string& symbol)
{
	LOG_INFO_STREAM(m_logger, "******************** EXCHANGE ORDER PRICE REPORT *********************************");
	binapi::show_exchanger_price_for_orders(std::cout,
		*BinanceSpotApiGateWayMgr, DEREF_V(m_exchangeProfileMgr->AccessRemoteExchangeProfile(symbol)), { symbol });
}

void BinanceReporter::UpdateRemoteReportCalculateLossForOrders(const std::string& symbol)
{
	LOG_INFO_STREAM(m_logger, "******************** CALCULATE ORDER LOSS REPORT *********************************");
	binapi::calc_loss_for_orders(std::cout,
		*BinanceSpotApiGateWayMgr, DEREF_V(m_exchangeProfileMgr->AccessRemoteExchangeProfile(symbol)), { symbol });
}

void BinanceReporter::DoRemoteExecutionReport(const std::string& symbol)
{
	if (m_enableLastDayTradeReporter)
	{
		UpdateRemoteReportTrades(symbol);
	}
	if (m_enableOpenOrderReporter)
	{
		UpdateRemoteReportOpenOrders(symbol);
	}
	if (m_enableBalanceReporter)
	{
		UpdateRemoteReportAccountBalance(symbol);
	}
	if (m_enableExchangerPriceForOrdersReporter)
	{
		UpdateRemoteReportExchangerPriceForOrders(symbol);
	}
	if (m_enableCalculateLossForOrdersReporter)
	{
		UpdateRemoteReportCalculateLossForOrders(symbol);
	}
}

void BinanceReporter::DoLocalExecutionReport(const std::string& symbol)
{
}

void BinanceReporter::DoTradeExecutionReport(const std::string& symbol)
{
	if (MergeLocalAndRemmoteReport())
	{
		UpdateRemoteData(symbol);
		DoRemoteExecutionReport(symbol);
		binapi::rest::account_info_t::write_account_info_to_file(
			m_reportToFilePath, *m_accountInfo);
	}
}

bool BinanceReporter::MergeLocalAndRemmoteReport()
{
	return true;
}
