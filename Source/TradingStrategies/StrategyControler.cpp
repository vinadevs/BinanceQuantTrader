/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "TradingStrategyBase.h"
#include "StrategyControler.h"

using namespace TradingStrategies;

StrategyControler::StrategyControler(TradingStrategyBase* strategy)
	: m_strategy(strategy) {}

StrategyControler::~StrategyControler() {}

void StrategyControler::StartStrategy()
{
	if (m_strategy)
	{
		m_strategy->StartLive();
	}
}

void StrategyControler::StopStrategy()
{
	if (m_strategy)
	{
		m_strategy->StopLive();
	}
}
