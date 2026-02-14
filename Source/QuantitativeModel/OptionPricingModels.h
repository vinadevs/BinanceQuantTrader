/********************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"

#include <vector>

namespace QuantitativeModel {

   // ------------------------------------------------
   // 1. Black-Scholes Model
   // ------------------------------------------------
    class DLL_CLASS_QUANTITATIVEMODEL_EXPORTS BlackScholesOption final
    {
    public:
        enum class OptionType : unsigned { Call, Put };

        double CalculatePrice(double S, double K, double r, double sigma, double T, OptionType type);

    private:
        double N(double x);
    };

    // ------------------------------------------------
    // 2. Black Model for Futures
    // ------------------------------------------------
    class DLL_CLASS_QUANTITATIVEMODEL_EXPORTS BlackFuturesOption final
    {
    public:
        enum class OptionType : unsigned { Call, Put };

        double CalculatePrice(double F, double K, double r, double sigma, double T, OptionType type);

    private:
        double N(double x);
    };

    // ------------------------------------------------
    // 3. Monte Carlo Simulation
    // ------------------------------------------------
    class DLL_CLASS_QUANTITATIVEMODEL_EXPORTS MonteCarloOption final
    {
    public:
        enum class OptionType : unsigned { Call, Put };

        double SimulatePrice(double S, double K, double r, double sigma, double T, OptionType type,
            int numSimulations = 10000, int seed = 42);

    private:
        double Payoff(double ST, double K, OptionType type);
    };

    // ------------------------------------------------
    // Risk-free Rate and Volatility Estimation
    // ------------------------------------------------
    double EstimateRiskFreeRate(const std::vector<double>& rates);
    double EstimateVolatility(const std::vector<double>& logReturns);
}; // namespace QuantitativeModel
