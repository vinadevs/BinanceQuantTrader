/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include "HybridTrader.h"

UserAccount::HybridTrader::HybridTrader(const tinyxml2::XMLElement* reportCfg, PortfolioManager::PortfolioInvestmentBinance* portfolio, ComplianceNRegulatory::BinanceTradingRules* tradingRules, RiskManagement::RiskManager* riskManager)
	: Trader(portfolio, tradingRules, riskManager)
{
	if (!reportCfg || !portfolio || !tradingRules || !riskManager)
	{
		throw std::runtime_error("HybridTrader: invalid input parameters");
	}
	m_traderType = TraderType::HYBRID_TRADER;
	m_spotTrader = std::make_unique<UserAccount::BinanceTrader>(reportCfg, portfolio, tradingRules, riskManager);
	m_futureTrader = std::make_unique<UserAccount::FutureTrader>(reportCfg, portfolio, tradingRules, riskManager);
}

bool UserAccount::HybridTrader::CreateSpotTrader(const tinyxml2::XMLElement* reportCfg, PortfolioManager::PortfolioInvestmentBinance* portfolio, ComplianceNRegulatory::BinanceTradingRules* tradingRules, RiskManagement::RiskManager* riskManager)
{
	if (!reportCfg || !portfolio || !tradingRules || !riskManager)
	{
		return false;
	}
	m_spotTrader = std::make_unique<UserAccount::BinanceTrader>(reportCfg, portfolio, tradingRules, riskManager);
	return true;
}

bool UserAccount::HybridTrader::CreateFutureTrader(const tinyxml2::XMLElement* reportCfg, PortfolioManager::PortfolioInvestmentBinance* portfolio, ComplianceNRegulatory::BinanceTradingRules* tradingRules, RiskManagement::RiskManager* riskManager)
{
	if (!reportCfg || !portfolio || !tradingRules || !riskManager)
	{
		return false;
	}
	m_futureTrader = std::make_unique<UserAccount::FutureTrader>(reportCfg, portfolio, tradingRules, riskManager);
	return true;
}

UserAccount::BinanceTrader* UserAccount::HybridTrader::GetSpotTrader() const
{
	return m_spotTrader ? m_spotTrader.get() : nullptr;
}

UserAccount::FutureTrader* UserAccount::HybridTrader::GetFutureTrader() const
{
	return m_spotTrader ? m_futureTrader.get() : nullptr;
}

void UserAccount::HybridTrader::ReportTradeResults(const std::string& symbol)
{
	if (m_spotTrader)
	{
		m_spotTrader->ReportTradeResults(symbol);
	}
	if (m_futureTrader)
	{
		m_futureTrader->ReportTradeResults(symbol);
	}
}
