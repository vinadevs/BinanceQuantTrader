/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include "VWAPOrderExecutionRiskMetrics.h"

#include <cmath>
#include <stdexcept>
#include <algorithm>

#undef max
#undef min

namespace RiskManagement {

    double VWAPOrderExecutionRiskMetrics::computeAverageSlippage(const std::vector<double>& executedPrices,
        const std::vector<double>& vwapPrices) {
        if (executedPrices.size() != vwapPrices.size() || executedPrices.empty()) {
            throw std::invalid_argument("Invalid input sizes for computeAverageSlippage");
        }

        double totalSlippage = 0.0;
        for (size_t i = 0; i < executedPrices.size(); ++i) {
            totalSlippage += executedPrices[i] - vwapPrices[i];
        }
        return totalSlippage / executedPrices.size();
    }

    double VWAPOrderExecutionRiskMetrics::computeStdDevSlippage(const std::vector<double>& executedPrices,
        const std::vector<double>& vwapPrices) {
        if (executedPrices.size() != vwapPrices.size() || executedPrices.empty()) {
            throw std::invalid_argument("Invalid input sizes for computeStdDevSlippage");
        }

        double avg = computeAverageSlippage(executedPrices, vwapPrices);
        double sumSq = 0.0;
        for (size_t i = 0; i < executedPrices.size(); ++i) {
            double diff = (executedPrices[i] - vwapPrices[i]) - avg;
            sumSq += diff * diff;
        }
        return std::sqrt(sumSq / executedPrices.size());
    }

    double VWAPOrderExecutionRiskMetrics::computeMaxDrawdown(const std::vector<double>& pnlSeries) {
        if (pnlSeries.empty()) {
            throw std::invalid_argument("Empty PnL series for computeMaxDrawdown");
        }

        double peak = pnlSeries[0];
        double maxDrawdown = 0.0;

        for (double pnl : pnlSeries) {
            if (pnl > peak) peak = pnl;
            double drawdown = peak - pnl;
            if (drawdown > maxDrawdown) maxDrawdown = drawdown;
        }
        return maxDrawdown;
    }

    double VWAPOrderExecutionRiskMetrics::computeSkewness(const std::vector<double>& data) {
        if (data.empty()) {
            throw std::invalid_argument("Empty data for computeSkewness");
        }

        double mean = 0.0;
        for (double val : data) mean += val;
        mean /= data.size();

        double m2 = 0.0, m3 = 0.0;
        for (double val : data) {
            double diff = val - mean;
            m2 += diff * diff;
            m3 += diff * diff * diff;
        }
        m2 /= data.size();
        m3 /= data.size();

        double stddev = std::sqrt(m2);
        if (stddev == 0.0) return 0.0;

        return m3 / (stddev * stddev * stddev);
    }

} // namespace RiskManagement
