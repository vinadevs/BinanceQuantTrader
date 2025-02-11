/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

namespace TradingStrategies {
    class TradingStrategyBase;
};

// This class will control worker thread
// for trading strategy/algo

namespace TradingStrategies {
    class StrategyControler {
    public:
        StrategyControler(TradingStrategyBase* strategy);
        ~StrategyControler();

        void StartStrategy();

        void StopStrategy();

    private:
        TradingStrategyBase* m_strategy{ nullptr };
    };
};