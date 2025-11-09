#include "pch.h"

#include <cmath>
#include <random>
#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>

#undef max
#undef min

constexpr double PI = 3.14159265358979323846;

// ------------------------
// 1. Black-Scholes Model
// ------------------------
class BlackScholesOption {
public:
    enum class OptionType { Call, Put };

    static double CalculatePrice(double S, double K, double r, double sigma, double T, OptionType type) {
        double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
        double d2 = d1 - sigma * std::sqrt(T);
        if (type == OptionType::Call)
            return S * N(d1) - K * std::exp(-r * T) * N(d2);
        else
            return K * std::exp(-r * T) * N(-d2) - S * N(-d1);
    }

private:
    static double N(double x) {
        return 0.5 * std::erfc(-x / std::sqrt(2));
    }
};

// ------------------------
// 2. Black Model for Futures
// ------------------------
class BlackFuturesOption {
public:
    enum class OptionType { Call, Put };

    static double CalculatePrice(double F, double K, double r, double sigma, double T, OptionType type) {
        double d1 = (std::log(F / K) + 0.5 * sigma * sigma * T) / (sigma * std::sqrt(T));
        double d2 = d1 - sigma * std::sqrt(T);
        if (type == OptionType::Call)
            return std::exp(-r * T) * (F * N(d1) - K * N(d2));
        else
            return std::exp(-r * T) * (K * N(-d2) - F * N(-d1));
    }

private:
    static double N(double x) {
        return 0.5 * std::erfc(-x / std::sqrt(2));
    }
};

// ------------------------
// 3. Monte Carlo Simulation
// ------------------------
class MonteCarloOption {
public:
    enum class OptionType { Call, Put };

    static double SimulatePrice(double S, double K, double r, double sigma, double T, OptionType type,
        int numSimulations = 10000, int seed = 42) {
        std::mt19937 gen(seed);
        std::normal_distribution<> dist(0.0, 1.0);

        double sumPayoff = 0.0;
        for (int i = 0; i < numSimulations; ++i) {
            double Z = dist(gen);
            double ST = S * std::exp((r - 0.5 * sigma * sigma) * T + sigma * std::sqrt(T) * Z);
            sumPayoff += Payoff(ST, K, type);
        }
        return std::exp(-r * T) * (sumPayoff / numSimulations);
    }

private:
    static double Payoff(double ST, double K, OptionType type) {
        if (type == OptionType::Call)
            return std::max(ST - K, 0.0);
        else
            return std::max(K - ST, 0.0);
    }
};

// ------------------------
// Risk-free Rate and Volatility Estimation
// ------------------------
double EstimateRiskFreeRate() {
    // Ví dụ giả định: Tính trung bình lãi suất trái phiếu ngắn hạn
    std::vector<double> rates = { 0.048, 0.052, 0.051 }; // dữ liệu 3 tháng gần nhất
    return std::accumulate(rates.begin(), rates.end(), 0.0) / rates.size();
}

double EstimateVolatility(const std::vector<double>& logReturns) {
    double mean = std::accumulate(logReturns.begin(), logReturns.end(), 0.0) / logReturns.size();
    double sumSquares = 0.0;
    for (double r : logReturns)
        sumSquares += (r - mean) * (r - mean);
    return std::sqrt(sumSquares / (logReturns.size() - 1)) * std::sqrt(252); // annualized
}

// ------------------------
// Example Usage
// ------------------------
int main() {
    double S = 30000;     // Spot price
    double K = 32000;     // Strike price
    double T = 0.25;      // 3 months to expiry

    // Tính lãi suất phi rủi ro
    double r = EstimateRiskFreeRate();

    // Tính log-returns giả định (dữ liệu bạn nên lấy từ BTC historical)
    std::vector<double> returns = { 0.01, -0.005, 0.003, -0.002, 0.006 };
    double sigma = EstimateVolatility(returns);

    // Black-Scholes on spot BTC
    double bsCall = BlackScholesOption::CalculatePrice(S, K, r, sigma, T, BlackScholesOption::OptionType::Call);
    double bsPut = BlackScholesOption::CalculatePrice(S, K, r, sigma, T, BlackScholesOption::OptionType::Put);

    std::cout << "Risk-free rate:     " << r << "\n";
    std::cout << "Volatility (sigma): " << sigma << "\n";
    std::cout << "Black-Scholes Call: " << bsCall << "\n";
    std::cout << "Black-Scholes Put:  " << bsPut << "\n";

    // Black model on futures price F
    double F = 31000;
    double bfCall = BlackFuturesOption::CalculatePrice(F, K, r, sigma, T, BlackFuturesOption::OptionType::Call);
    std::cout << "Black Futures Call: " << bfCall << "\n";

    // Monte Carlo simulation
    double mcCall = MonteCarloOption::SimulatePrice(S, K, r, sigma, T, MonteCarloOption::OptionType::Call);
    std::cout << "Monte Carlo Call:   " << mcCall << "\n";

    return 0;
}
