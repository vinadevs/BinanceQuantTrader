/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "SpotRiskEngine.h"

#include "../LibraryUtils/Logger.h"
#include "../PortfolioManager/PortfolioInvestmentBinance.h"
#include "../RiskManagement/RiskManager.h"
#include "../KernelTrading/user_future_account.h"

using namespace RiskManagement;

SpotRiskEngine::SpotRiskEngine(
	PortfolioManager::PortfolioInvestmentBinance* portfolio,
	RiskManagement::RiskManager* riskManager,
	binapi::rest::account_info_t* userSpotAccount,
	LibraryUtils::Logger* logger)
	: m_logger{ logger },
	m_portfolio{ portfolio },
	m_riskManager{ riskManager },
	m_userSpotAccount{ userSpotAccount }
{
}

SpotRiskEngine::~SpotRiskEngine()
{
}

RiskReport SpotRiskEngine::AssessTradingRisk(const OrderManagement::Order* order)
{
	return RiskReport();
}
