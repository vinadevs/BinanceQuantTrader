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
#include "../RestAPI/BinanceAPI.h"
#include "../LibraryUtils/Logger.h"
#include "../SettingNConfig/tinyxml2.h"
#include "../PortfolioManager/PortfolioInvestmentBinance.h"
#include "../ComplianceNRegulatory/BinanceExchangeProfile.h"

#include "ReportAPIs.h"
#include "BinanceReporter.h"

#include "BinanceTrader.h"

using namespace UserAccount;
using namespace PortfolioManager;
using namespace RestAPI;

BinanceReporter::BinanceReporter(
	const tinyxml2::XMLElement* reportConfigXml,
	binapi::rest::account_info_t* accountInfo,
	ComplianceNRegulatory::BinanceExchangeProfileMgr* exchangeProfileMgr,
	PortfolioInvestmentBinance* portfolio)
	: ExchangeReporter(accountInfo, exchangeProfileMgr, portfolio)
{
	m_logger = std::make_unique<LibraryUtils::Logger>("BinanceReporter");
	SetupReporter(reportConfigXml);
	m_logger->Info("setting up trading reporter finished.");
}

BinanceReporter::~BinanceReporter() {}

void BinanceReporter::UpdateRemoteData(const std::string& symbol)
{
	const auto accountInfoResult = BinanceApiGateWay->account_info();
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
	const auto exchangeInfoResult = BinanceApiGateWay->exchange_info(symbol);
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
}

void BinanceReporter::UpdateRemoteReportTrades(const std::string& symbol)
{
	UpdateRemoteData(symbol);
	static const auto trades_report_cb = [&](const binapi::rest::order_info_t& o)
	{
		LOG_INFO_STREAM(m_logger, o.symbol << " - " << o.orderId);
	};
	LOG_INFO_STREAM(m_logger, "********************* TRADES REPORT *********************************");
	binapi::make_trades_report_for_last_day(std::cout, 
		*BinanceApiGateWay, DEREF_V(m_accountInfo),
		DEREF_V(m_exchangeProfileMgr->AccessRemoteExchangeProfile(symbol)),
		{ symbol }, trades_report_cb);
}

void BinanceReporter::UpdateRemoteReportOpenOrders(const std::string& symbol)
{
	UpdateRemoteData(symbol);
	LOG_INFO_STREAM(m_logger, "******************* OPEN ORDERS REPORT ******************************");
	binapi::make_open_orders_report(std::cout, 
		*BinanceApiGateWay, DEREF_V(m_exchangeProfileMgr->AccessRemoteExchangeProfile(symbol)), { symbol });
}

void BinanceReporter::UpdateRemoteReportAccountBalance(const std::string& symbol)
{
	UpdateRemoteData(symbol);
	LOG_INFO_STREAM(m_logger, "******************** USER BALANCE REPORT *********************************");
	binapi::make_balance_report(std::cout, 
		*BinanceApiGateWay, DEREF_V(m_accountInfo),
		DEREF_V(m_exchangeProfileMgr->AccessRemoteExchangeProfile(symbol)));
}

void BinanceReporter::UpdateRemoteReportExchangerPriceForOrders(const std::string& symbol)
{
	UpdateRemoteData(symbol);
	LOG_INFO_STREAM(m_logger, "******************** EXCHANGE ORDER PRICE REPORT *********************************");
	binapi::show_exchanger_price_for_orders(std::cout,
		*BinanceApiGateWay, DEREF_V(m_exchangeProfileMgr->AccessRemoteExchangeProfile(symbol)), { symbol });
}

void BinanceReporter::UpdateRemoteReportCalculateLossForOrders(const std::string& symbol)
{
	UpdateRemoteData(symbol);
	LOG_INFO_STREAM(m_logger, "******************** CALCULATE ORDER LOSS REPORT *********************************");
	binapi::calc_loss_for_orders(std::cout,
		*BinanceApiGateWay, DEREF_V(m_exchangeProfileMgr->AccessRemoteExchangeProfile(symbol)), { symbol });
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

void BinanceReporter::DoTradeExecutionReport()
{
	if (MergeLocalAndRemmoteReport())
	{
		LOG_INFO_STREAM(m_logger, "BinanceAccountInfo=" << *m_portfolio->GetBinanceAccountInfo());
	}
}

bool BinanceReporter::MergeLocalAndRemmoteReport()
{
	return false;
}
