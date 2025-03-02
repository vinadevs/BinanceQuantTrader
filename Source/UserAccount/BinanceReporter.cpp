/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../RestAPI/RestAPI.h"
#include "../RestAPI/BinanceAPI.h"
#include "../LibraryUtils/Logger.h"
#include "../SettingNConfig/tinyxml2.h"
#include "../PortfolioManager/PortfolioInvestmentBinance.h"

#include "ReportAPIs.h"
#include "BinanceReporter.h"

#include <iostream>
#include "BinanceTrader.h"

using namespace UserAccount;
using namespace PortfolioManager;
using namespace RestAPI;

BinanceReporter::BinanceReporter(
	const tinyxml2::XMLElement* reportConfigXml,
	PortfolioInvestmentBinance* portfolio)
	: ExchangeReporter(portfolio)
{
	m_logger = std::make_unique<LibraryUtils::Logger>("BinanceReporter");
	SetupReporter(reportConfigXml);
	m_logger->Info("setting up trading reporter finished.");
}

BinanceReporter::~BinanceReporter() {}

void BinanceReporter::SetupReporter(const tinyxml2::XMLElement* reportCfg)
{
	if (reportCfg->BoolAttribute("TradeReport"))
	{
		m_enableTradeReporter = true;
	}
	if (reportCfg->BoolAttribute("OrderReport"))
	{
		m_enableOpenOrderReporter = true;
	}
	if (reportCfg->BoolAttribute("BalanceReport"))
	{
		m_enableBalanceReporter = true;
	}
}
void BinanceReporter::UpdateRemoteReportTrades(const std::string& symbol)
{
	auto accountIInfo = BinanceApiGateWay->account_info();
	if (!accountIInfo)
	{
		LOG_ERROR_STREAM(m_logger, "get account info error: " << accountIInfo.errmsg);
		return;
	}
	auto exchangeIInfo = BinanceApiGateWay->exchange_info(symbol);
	if (!exchangeIInfo)
	{
		LOG_ERROR_STREAM(m_logger, "exchange_info error: " << exchangeIInfo.errmsg);
		return;
	}
	static const auto trades_report_cb = [&](const binapi::rest::order_info_t& o)
	{
		LOG_INFO_STREAM(m_logger, o.symbol << " - " << o.orderId);
	};
	LOG_INFO_STREAM(m_logger, "********************* TRADES REPORT *********************************");
	binapi::make_trades_report_for_last_day(std::cout, *BinanceApiGateWay, accountIInfo.v, exchangeIInfo.v, { symbol }, trades_report_cb);
}

void BinanceReporter::UpdateRemoteReportOpenOrders(const std::string& symbol)
{
	auto accountIInfo = BinanceApiGateWay->account_info();
	if (!accountIInfo) 
	{
		LOG_ERROR_STREAM(m_logger, "get account info error: " << accountIInfo.errmsg);
		return;
	}
	auto exchangeIInfo = BinanceApiGateWay->exchange_info(symbol);
	if (!exchangeIInfo) 
	{
		LOG_ERROR_STREAM(m_logger, "exchange_info error: " << exchangeIInfo.errmsg);
		return;
	}
	LOG_INFO_STREAM(m_logger, "******************* OPEN ORDERS REPORT ******************************");
	binapi::make_open_orders_report(std::cout, *BinanceApiGateWay, exchangeIInfo.v, {});
}

void BinanceReporter::UpdateRemoteReportAccountBalance(const std::string& symbol)
{
	auto accountIInfo = BinanceApiGateWay->account_info();
	if (!accountIInfo)
	{
		LOG_ERROR_STREAM(m_logger, "get account info error: " << accountIInfo.errmsg);
		return;
	}
	auto exchangeIInfo = BinanceApiGateWay->exchange_info(symbol);
	if (!exchangeIInfo)
	{
		LOG_ERROR_STREAM(m_logger, "exchange_info error: " << exchangeIInfo.errmsg);
		return;
	}
	LOG_INFO_STREAM(m_logger, "******************** BALANCE REPORT *********************************");
	binapi::make_balance_report(std::cout, *BinanceApiGateWay, accountIInfo.v, exchangeIInfo.v);
}

void BinanceReporter::DoRemoteExecutionReport(const std::string& symbol)
{
	if (m_enableTradeReporter)
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
}

void BinanceReporter::DoLocalExecutionReport(const std::string& symbol)
{
}

void BinanceReporter::DoTradeExecutionReport()
{
	MergeLocalAndRemmoteReport();
	LOG_INFO_STREAM(m_logger, "BinanceAccountInfo=" << *m_portfolio->GetBinanceAccountInfo());
}

bool BinanceReporter::MergeLocalAndRemmoteReport()
{
	return false;
}
