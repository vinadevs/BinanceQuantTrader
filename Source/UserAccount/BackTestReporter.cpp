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
#include "BackTestReporter.h"

#include <iostream>

using namespace UserAccount;
using namespace PortfolioManager;
using namespace RestAPI;

BackTestReporter::BackTestReporter(
	const tinyxml2::XMLElement* reportConfigXml,
	PortfolioInvestmentBinance* portfolio)
	: ExchangeReporter(portfolio)
{
	m_logger = std::make_unique<LibraryUtils::Logger>("BackTestReporter");
	SetupReporter(reportConfigXml);
	m_logger->Info("setting up trading reporter finished.");
}

BackTestReporter::~BackTestReporter() {}

void BackTestReporter::SetupReporter(const tinyxml2::XMLElement* reportCfg)
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

void BackTestReporter::UpdateRemoteReportTrades(const std::string& symbol)
{
}

void BackTestReporter::UpdateRemoteReportOpenOrders(const std::string& symbol)
{	
}

void BackTestReporter::UpdateRemoteReportAccountBalance(const std::string& symbol)
{
}

void BackTestReporter::DoRemoteExecutionReport(const std::string& symbol)
{
}

void BackTestReporter::DoLocalExecutionReport(const std::string& symbol)
{
}

void BackTestReporter::DoTradeExecutionReport()
{
	MergeLocalAndRemmoteReport();
	LOG_INFO_STREAM(m_logger, "BinanceAccountInfo=" << *m_portfolio->GetBinanceAccountInfo());
}

bool BackTestReporter::MergeLocalAndRemmoteReport()
{
	return false;
}
