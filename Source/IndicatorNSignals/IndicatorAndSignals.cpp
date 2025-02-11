/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "IndicatorAndSignals.h"

using namespace IndicatorNSignals;

IndicatorAndSignals::IndicatorAndSignals(const std::string& m_symbol)
    : m_tradingHints { std::make_unique<TradingHints>(m_symbol) } {}

IndicatorAndSignals::~IndicatorAndSignals() {}

const std::string& IndicatorAndSignals::GetTargetSymbol() const
{
    return m_tradingHints->symbol;
}

void IndicatorAndSignals::RegisterTradingHintsListener(TradingHintsListener* listener)
{
    if (listener)
    {
        if (m_tradingHintsListeners.end() == std::find(m_tradingHintsListeners.begin(),
            m_tradingHintsListeners.end(), listener))
        {
            m_tradingHintsListeners.emplace_back(listener);
        }
    }
}

void IndicatorAndSignals::UnregisterTradingHintsListener(TradingHintsListener* listener)
{
    if (listener)
    {
        m_tradingHintsListeners.remove(listener);
    }
}

int IndicatorAndSignals::NotifyTradingHintsToListeners()
{
    int updated{ 0 };
    for (auto obverver : m_tradingHintsListeners)
    {
        if (obverver->OnReceivedTradingHints(m_tradingHints.get()))
        {
            updated++;
        }
    }
    return updated;
}
