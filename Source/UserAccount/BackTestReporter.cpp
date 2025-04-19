/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include "../RestAPI/ApiKeyInfoManager.h"
#include "../LibraryUtils/Logger.h"
#include "../LibraryUtils/PathUtils.h"
#include "../LibraryUtils/StringUtils.h"
#include "../SettingNConfig/tinyxml2.h"
#include "../PortfolioManager/PortfolioInvestmentBinance.h"
#include "../ComplianceNRegulatory/BinanceExchangeProfile.h"
#include "../ExchangeConnectivity/ExchangeSimulatorConnectivity.h"

#include "ReportAPIs.h"
#include "BackTestReporter.h"

#include <filesystem>

using namespace UserAccount;
using namespace PortfolioManager;

BackTestReporter::BackTestReporter(
	const tinyxml2::XMLElement* reportConfigXml,
	binapi::rest::account_info_t* accountInfo,
	ComplianceNRegulatory::BinanceExchangeProfileMgr* exchangeProfileMgr,
	PortfolioInvestmentBinance* portfolio)
	: ExchangeReporter(accountInfo, exchangeProfileMgr, portfolio)
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
}

void BackTestReporter::UpdateRemoteData(const std::string& symbol)
{
	std::string errorMessage;
	if (ExchangeSimulatorGateWay->GetUserAccountInfo(
		ApiKeyInfoMgr->GetApiKeyInfo().m_userID, m_accountInfo, errorMessage))
	{
		m_logger->Info("updating account info finished.");
	}
	else
	{
		m_logger->Error("account_info: emsg=" + errorMessage);
	}
	if (ExchangeSimulatorGateWay->GetExchangeInfo(
		symbol, m_exchangeProfileMgr->AccessRemoteExchangeProfile(symbol), errorMessage))
	{
		m_logger->Info("updating exchange info finished.");
	}
	else
	{
		m_logger->Error("account_info: emsg=" + errorMessage);
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

void BackTestReporter::UpdateRemoteReportExchangerPriceForOrders(const std::string& symbol)
{
}

void BackTestReporter::UpdateRemoteReportCalculateLossForOrders(const std::string& symbol)
{
}

void BackTestReporter::DoRemoteExecutionReport(const std::string& symbol)
{
}

void BackTestReporter::DoLocalExecutionReport(const std::string& symbol)
{
}

void BackTestReporter::DoTradeExecutionReport(const std::string& symbol)
{
	if (MergeLocalAndRemmoteReport())
	{
		UpdateRemoteData(symbol);
		DoRemoteExecutionReport(symbol);
		binapi::rest::account_info_t::write_account_info_to_file(
			m_reportToFilePath, *m_accountInfo);
	}
}

bool BackTestReporter::MergeLocalAndRemmoteReport()
{
	return true;
}
