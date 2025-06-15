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
#include "../OrderManagement/PositionManager.h"
#include "../ComplianceNRegulatory/BinanceExchangeProfile.h"
#include "../ExchangeConnectivity/ExchangeSimulatorConnectivity.h"

#include "ReportAPIs.h"
#include "BackTestReporter.h"



using namespace UserSpotAccount;
using namespace OrderManagement;

BackTestReporter::BackTestReporter(
	const tinyxml2::XMLElement* reportConfigXml,
	binapi::rest::account_info_t* accountInfo,
	ComplianceNRegulatory::BinanceExchangeProfileMgr* exchangeProfileMgr,
	PositionManager* positionManager)
	: ExchangeReporter(accountInfo, exchangeProfileMgr, positionManager)
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

	const std::string reportChannel(reportCfg->Attribute("ReportChannel"));
	m_reportChannel = FromReportChannelTextToEnum(reportChannel);
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
		if (m_reportChannel == ReportChannel::CONSOLE)
		{
		}
		else if (m_reportChannel == ReportChannel::EXTERNAL_FILE)
		{
			std::ofstream externalFileStream(m_reportToFilePath);
			if (!externalFileStream.is_open()) {
				throw std::runtime_error("BackTestReporter: could not open report file="
					+ m_reportToFilePath);
			}
			externalFileStream << "-- User Account Balance -------------------------" << std::endl;
			binapi::rest::account_info_t::write_account_info_to_file(
				externalFileStream, *m_accountInfo);
			externalFileStream << std::endl;
			const auto* orderManager = m_positionManager->GetWorkedOrderManager();
			if (orderManager)
			{
				externalFileStream << "-- Traded Order List -------------------------" << std::endl;
				
				const auto& listOrder = orderManager->GetOrders();
				externalFileStream << "Total new order=" << listOrder.size() << std::endl;
				for (const auto& order : listOrder)
				{
					externalFileStream << order.second.get()->ToStringAck() << std::endl;
				}

				const auto& listCancelOrder = orderManager->GetCancelOrders();
				externalFileStream << "Total cancel order=" << listCancelOrder.size() << std::endl;
				for (const auto& order : listCancelOrder)
				{
					externalFileStream << order.second.get()->ToStringAck() << std::endl;
				}
			}
		}
		else if (m_reportChannel == ReportChannel::GUI_APP)
		{
		}
	}
}

bool BackTestReporter::MergeLocalAndRemmoteReport()
{
	return true;
}
