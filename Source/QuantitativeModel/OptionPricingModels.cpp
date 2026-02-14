/********************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include "OptionPricingModels.h"

#include <cmath>
#include <random>
#include <algorithm>
#include <vector>
#include <numeric>

#undef max
#undef min

using namespace QuantitativeModel;

constexpr double PI = 3.14159265358979323846;

// ------------------------------------------------
// 1. Black-Scholes Model
// ------------------------------------------------

double BlackScholesOption::CalculatePrice(double S, double K, double r, double sigma, double T, OptionType type)
{
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);
    if (type == OptionType::Call)
        return S * N(d1) - K * std::exp(-r * T) * N(d2);
    else
        return K * std::exp(-r * T) * N(-d2) - S * N(-d1);
}

double BlackScholesOption::N(double x)
{
    return 0.5 * std::erfc(-x / std::sqrt(2));
}

// ------------------------------------------------
// 2. Black Model for Futures
// ------------------------------------------------

double BlackFuturesOption::CalculatePrice(double F, double K, double r, double sigma, double T, OptionType type)
{
    double d1 = (std::log(F / K) + 0.5 * sigma * sigma * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);
    if (type == OptionType::Call)
        return std::exp(-r * T) * (F * N(d1) - K * N(d2));
    else
        return std::exp(-r * T) * (K * N(-d2) - F * N(-d1));
}

double BlackFuturesOption::N(double x)
{
    return 0.5 * std::erfc(-x / std::sqrt(2));
}

// ------------------------------------------------
// 3. Monte Carlo Simulation
// ------------------------------------------------

double MonteCarloOption::SimulatePrice(double S, double K, double r, double sigma, double T, OptionType type,
    int numSimulations, int seed) 
{
    std::mt19937 gen(seed);
    std::normal_distribution<> dist(0.0, 1.0);

    double sumPayoff = 0.0;
    for (int i = 0; i < numSimulations; ++i) 
    {
        double Z = dist(gen);
        double ST = S * std::exp((r - 0.5 * sigma * sigma) * T + sigma * std::sqrt(T) * Z);
        sumPayoff += Payoff(ST, K, type);
    }
    return std::exp(-r * T) * (sumPayoff / numSimulations);
}

double MonteCarloOption::Payoff(double ST, double K, OptionType type)
{
    if (type == OptionType::Call)
        return std::max(ST - K, 0.0);
    else
        return std::max(K - ST, 0.0);
}

// ------------------------------------------------
// Risk-free Rate and Volatility Estimation
// ------------------------------------------------
double QuantitativeModel::EstimateRiskFreeRate(const std::vector<double>& rates)
{
    return std::accumulate(rates.begin(), rates.end(), 0.0) / rates.size();
}

double QuantitativeModel::EstimateVolatility(const std::vector<double>& logReturns)
{
    double mean = std::accumulate(logReturns.begin(), logReturns.end(), 0.0) / logReturns.size();
    double sumSquares = 0.0;
    for (double r : logReturns)
        sumSquares += (r - mean) * (r - mean);
    return std::sqrt(sumSquares / (logReturns.size() - 1)) * std::sqrt(252); // annualized
}
