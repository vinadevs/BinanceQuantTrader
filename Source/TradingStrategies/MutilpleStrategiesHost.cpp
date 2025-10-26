/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "StrategyControler.h"
#include "MutilpleStrategiesHost.h"

#include "../LibraryUtils/Logger.h"

using namespace TradingStrategies;

MutilpleStrategiesHost::MutilpleStrategiesHost()
    : m_logger{ std::make_unique<LibraryUtils::Logger>("MutilpleStrategiesHost") } {}

MutilpleStrategiesHost::~MutilpleStrategiesHost()
{
    StopStrategyThread();
}

void MutilpleStrategiesHost::StartStrategyThread(TradingStrategyBase* strategy)
{
    if (m_threadStarted)
    {
        m_logger->Info("Strategy thread already started.");
        return;
    }
    m_threadStarted = true;
    m_strategyControler = std::make_unique<StrategyControler>(strategy);
    m_strategyThread = std::thread([this]
    {
        m_strategyControler->StartStrategy();
    });
    m_logger->Info("Strategy thread started.");
}

void MutilpleStrategiesHost::StopStrategyThread()
{
    if (!m_threadStarted)
    {
        m_logger->Info("Strategy thread has not started yet.");
        return;
    }
    m_strategyControler->StopStrategy();
    m_logger->Info("Strategy thread stopped.");
}