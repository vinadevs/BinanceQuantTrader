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
	const auto accountInfo = BinanceApiGateWay->account_info();
	if (!accountInfo)
	{
		LOG_ERROR_STREAM(m_logger, "get account info error: " << accountInfo.errmsg);
		return;
	}
	const auto exchangeInfo = BinanceApiGateWay->exchange_info(symbol);
	if (!exchangeInfo)
	{
		LOG_ERROR_STREAM(m_logger, "exchange_info error: " << exchangeInfo.errmsg);
		return;
	}
	static const auto trades_report_cb = [&](const binapi::rest::order_info_t& o)
	{
		LOG_INFO_STREAM(m_logger, o.symbol << " - " << o.orderId);
	};
	LOG_INFO_STREAM(m_logger, "********************* TRADES REPORT *********************************");
	binapi::make_trades_report_for_last_day(std::cout, 
		*BinanceApiGateWay, accountInfo.v, exchangeInfo.v,
		{ symbol }, trades_report_cb);
}

void BinanceReporter::UpdateRemoteReportOpenOrders(const std::string& symbol)
{
	const auto accountInfo = BinanceApiGateWay->account_info();
	if (!accountInfo) 
	{
		LOG_ERROR_STREAM(m_logger, "get account info error: " << accountInfo.errmsg);
		return;
	}
	const auto exchangeInfo = BinanceApiGateWay->exchange_info(symbol);
	if (!exchangeInfo) 
	{
		LOG_ERROR_STREAM(m_logger, "exchange_info error: " << exchangeInfo.errmsg);
		return;
	}
	LOG_INFO_STREAM(m_logger, "******************* OPEN ORDERS REPORT ******************************");
	binapi::make_open_orders_report(std::cout, 
		*BinanceApiGateWay, exchangeInfo.v, { symbol });
}

void BinanceReporter::UpdateRemoteReportAccountBalance(const std::string& symbol)
{
	const auto accountInfo = BinanceApiGateWay->account_info();
	if (!accountInfo)
	{
		LOG_ERROR_STREAM(m_logger, "get account info error: " << accountInfo.errmsg);
		return;
	}
	const auto exchangeInfo = BinanceApiGateWay->exchange_info(symbol);
	if (!exchangeInfo)
	{
		LOG_ERROR_STREAM(m_logger, "exchange_info error: " << exchangeInfo.errmsg);
		return;
	}
	LOG_INFO_STREAM(m_logger, "******************** USER BALANCE REPORT *********************************");
	binapi::make_balance_report(std::cout, 
		*BinanceApiGateWay, accountInfo.v, exchangeInfo.v);
}

void BinanceReporter::UpdateRemoteReportExchangerPriceForOrders(const std::string& symbol)
{
	const auto accountInfo = BinanceApiGateWay->account_info();
	if (!accountInfo)
	{
		LOG_ERROR_STREAM(m_logger, "get account info error: " << accountInfo.errmsg);
		return;
	}
	const auto exchangeInfo = BinanceApiGateWay->exchange_info(symbol);
	if (!exchangeInfo)
	{
		LOG_ERROR_STREAM(m_logger, "exchange_info error: " << exchangeInfo.errmsg);
		return;
	}
	LOG_INFO_STREAM(m_logger, "******************** EXCHANGE ORDER PRICE REPORT *********************************");
	binapi::show_exchanger_price_for_orders(std::cout,
		*BinanceApiGateWay, exchangeInfo.v, { symbol });
}

void BinanceReporter::UpdateRemoteReportCalculateLossForOrders(const std::string& symbol)
{
	const auto accountInfo = BinanceApiGateWay->account_info();
	if (!accountInfo)
	{
		LOG_ERROR_STREAM(m_logger, "get account info error: " << accountInfo.errmsg);
		return;
	}
	const auto exchangeInfo = BinanceApiGateWay->exchange_info(symbol);
	if (!exchangeInfo)
	{
		LOG_ERROR_STREAM(m_logger, "exchange_info error: " << exchangeInfo.errmsg);
		return;
	}
	LOG_INFO_STREAM(m_logger, "******************** CALCULATE ORDER LOSS REPORT *********************************");
	binapi::calc_loss_for_orders(std::cout,
		*BinanceApiGateWay, exchangeInfo.v, { symbol });
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
