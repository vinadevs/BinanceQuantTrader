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
#include "FutureBackTestReporter.h"

using namespace UserAccount;
using namespace OrderManagement;

FutureBackTestReporter::FutureBackTestReporter(
	const tinyxml2::XMLElement* reportConfigXml,
	KernelTrading::UserFutureAccount* accountInfo,
	ComplianceNRegulatory::BinanceExchangeProfileMgr* exchangeProfileMgr,
	PositionManager* positionManager)
	: FutureTradeReporter(accountInfo, exchangeProfileMgr, positionManager)
{
	m_logger = std::make_unique<LibraryUtils::Logger>("FutureBackTestReporter");
	SetupReporter(reportConfigXml);
	m_logger->Info("setting up trading reporter finished.");
}

FutureBackTestReporter::~FutureBackTestReporter() {}

void FutureBackTestReporter::SetupReporter(const tinyxml2::XMLElement* reportCfg)
{
	std::string reportToFilePath(reportCfg->Attribute("ReportToFilePath"));
	PathUtils::ReplaceSubString(reportToFilePath, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());
	m_reportToFilePath = reportToFilePath;

	const std::string reportChannel(reportCfg->Attribute("ReportChannel"));
	m_reportChannel = FromReportChannelTextToEnum(reportChannel);
}

void FutureBackTestReporter::UpdateRemoteData(const std::string& symbol)
{
	std::string errorMessage;
	if (ExchangeSimulatorGateWay->GetUserFutureAccountInfo(
		ApiKeyInfoMgr->GetApiKeyInfo().m_userID, m_accountInfo, errorMessage))
	{
		m_logger->Info("updating future account info finished.");
	}
	else
	{
		m_logger->Error("account_info: emsg=" + errorMessage);
	}
}

void FutureBackTestReporter::DoTradeExecutionReport(const std::string& symbol)
{
	UpdateRemoteData(symbol);
	if (m_reportChannel == ReportChannel::CONSOLE)
	{
	}
	else if (m_reportChannel == ReportChannel::EXTERNAL_FILE)
	{
		std::ofstream externalFileStream(m_reportToFilePath);
		if (!externalFileStream.is_open()) {
			throw std::runtime_error("FutureBackTestReporter: could not open report file="
				+ m_reportToFilePath);
		}
		externalFileStream << "-- User Future Account Balance -------------------------" << std::endl;
		KernelTrading::UserFutureAccount::write_account_info_to_file(
			externalFileStream, *m_accountInfo);
		externalFileStream << std::endl;
		/*const auto* orderManager = m_positionManager->GetWorkedOrderManager();
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
		}*/
	}
	else if (m_reportChannel == ReportChannel::GUI_APP)
	{
	}
}
