/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "TradingStrategyBase.h"
#include "StrategyController.h"

using namespace TradingStrategies;

StrategyController::StrategyController(TradingStrategyBase* strategy)
	: m_strategy(strategy) {}

StrategyController::~StrategyController() {}

void StrategyController::StartStrategy()
{
	if (m_strategy)
	{
		m_strategy->StartTrade();
	}
}

void StrategyController::StopStrategy()
{
	if (m_strategy)
	{
		m_strategy->StopTrade();
	}
}
