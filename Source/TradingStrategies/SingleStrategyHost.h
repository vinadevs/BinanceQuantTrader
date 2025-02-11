/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include <thread>
#include <mutex>
#include <memory>

class TradingStrategyBase;
class StrategyControler;

namespace LibraryUtils {
    class Logger;
};

// This class will manage worker thread
// for trading strategy/algo

namespace TradingStrategies {
    class DLL_CLASS_TRADING_TRATEGIES_EXPORTS SingleStrategyHost {
    public:
        SingleStrategyHost();
        ~SingleStrategyHost();

        void StartStrategyThread(TradingStrategyBase* strategy);
        void StopStrategyThread();
    private:
        bool m_threadStarted {false};
        std::mutex m_threadSafeMutex;
        std::thread m_strategyThread;
        std::unique_ptr<StrategyControler> m_strategyControler;
        std::unique_ptr<LibraryUtils::Logger> m_logger;
    };
};