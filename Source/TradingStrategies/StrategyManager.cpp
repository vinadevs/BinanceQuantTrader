/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "StrategyManager.h"
#include "TradingStrategyBase.h"

void TradingStrategies::StrategyManager::AddStrategy(TradingStrategyBase* strategy)
{
	m_strategies[strategy->GetStrategyID()] = strategy;
}

TradingStrategies::TradingStrategyBase* TradingStrategies::StrategyManager::GetStrategyByID(const std::string& strategyID)
{
	auto it = m_strategies.find(strategyID);
	if (it != m_strategies.end())
	{
		return it->second;
	}
	return nullptr;
}

bool TradingStrategies::StrategyManager::RemoveStrategyByID(const std::string& strategyID)
{
	auto it = m_strategies.find(strategyID);
	if (it != m_strategies.end())
	{
		m_strategies.erase(it);
		return true;
	}
	return false;
}
