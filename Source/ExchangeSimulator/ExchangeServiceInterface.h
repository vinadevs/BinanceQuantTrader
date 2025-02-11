/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

namespace ExchangeSimulator {

    /**
     * @class ServiceInterface
     * @brief An interface for all exchange component services 
     */

    class ExchangeServiceInterface
    {
    public:
        virtual void Start() = 0;
        virtual void Stop() = 0;
    };
};

