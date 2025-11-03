/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "StrategyControler.h"
#include "SingleStrategyHost.h"

#include "../LibraryUtils/Logger.h"

using namespace TradingStrategies;

SingleStrategyHost::SingleStrategyHost()
    : m_logger{ std::make_unique<LibraryUtils::Logger>("SingleStrategyHost") } {}

SingleStrategyHost::~SingleStrategyHost()
{
    StopStrategyThread();
}

void SingleStrategyHost::StartStrategyThread(TradingStrategyBase* strategy)
{
    std::unique_lock<std::mutex> lock(m_threadSafeMutex);
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

void SingleStrategyHost::StopStrategyThread()
{
    std::unique_lock<std::mutex> lock(m_threadSafeMutex);
    if (!m_threadStarted)
    {
        m_logger->Info("Strategy thread has not started yet.");
        return;
    }
    m_strategyControler->StopStrategy();
    if (m_strategyThread.joinable()) 
    {
        m_strategyThread.join();
    }
    m_threadStarted = false;
    m_logger->Info("Strategy thread stopped.");
}
