/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "FutureRiskEngine.h"

#include "../LibraryUtils/Logger.h"
#include "../PortfolioManager/PortfolioInvestmentBinance.h"
#include "../RiskManagement/RiskManager.h"
#include "../KernelTrading/user_future_account.h"

using namespace RiskManagement;

FutureRiskEngine::FutureRiskEngine(
	PortfolioManager::PortfolioInvestmentBinance* portfolio,
	RiskManagement::RiskManager* riskManager,
	KernelTrading::UserFutureAccount* userFutureAccount,
	LibraryUtils::Logger* logger)
	: m_logger{ logger },
	m_portfolio{ portfolio },
	m_riskManager{ riskManager },
	m_userFutureAccount{ userFutureAccount }
{
}

FutureRiskEngine::~FutureRiskEngine()
{
}

RiskReport FutureRiskEngine::AssessTradingRisk(const OrderManagement::Order* order)
{
	return RiskReport();
}
