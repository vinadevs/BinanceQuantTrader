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
	RiskReport report;
	// 1. Check if the order exceeds the max position size limit
	//double currentPositionSize = m_portfolio->GetSpotPositionSize(order->GetSymbol());
	//double newPositionSize = currentPositionSize + order->GetQuantity();
	//double maxPositionSize = m_riskManager->GetMaxPositionSize();
	//if (std::abs(newPositionSize) > maxPositionSize)
	//{
	//	report.m_isRisky = true;
	//	report.m_riskReason = "Exceeds max position size limit";
	//	m_logger->Warn("Risk assessment failed for order: " + order->GetOrderId() + ". Reason: " + report.m_riskReason);
	//	return report;
	//}
	//// 2. Check if the order would cause the account to exceed risk limits
	//double accountBalance = m_riskManager->GetAccountBalance();
	//double riskPerTradePercent = m_riskManager->GetRiskPerTradePercent();
	//double riskAmount = accountBalance * riskPerTradePercent / 100.0;
	//double potentialLoss = std::abs(order->GetQuantity() * order->GetPrice() - currentPositionSize * order->GetPrice());
	//if (potentialLoss > riskAmount)
	//{
	//	report.m_isRisky = true;
	//	report.m_riskReason = "Potential loss exceeds risk per trade limit";
	//	m_logger->Warn("Risk assessment failed for order: " + order->GetOrderId() + ". Reason: " + report.m_riskReason);
	//	return report;
	//}
	//// If all checks pass, the order is not considered risky
	//report.m_isRisky = false;
	return report;
}
