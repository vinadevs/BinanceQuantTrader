/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include <memory>

namespace ExchangeSimulator {

    /**
     * @class ServiceInterface
     * @brief An interface for all exchange component services 
     */

    class ExchangeServiceInterface
    {
    public:
        virtual ~ExchangeServiceInterface() = default; // deleted via base-class pointers
        virtual void Start() = 0;
        virtual void Stop() = 0;
    };

    using ExchangeService = std::unique_ptr<ExchangeServiceInterface>;
};

