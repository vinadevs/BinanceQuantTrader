/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#ifndef VWAP_ORDER_EXECUTION_RISK_METRICS_H
#define VWAP_ORDER_EXECUTION_RISK_METRICS_H

#include "dlldefine.h"

#include <vector>

namespace RiskManagement {

    /**
     * @brief Class for calculating execution risk metrics for VWAP-based trading strategies.
     *
     * This class provides statistical risk metrics such as:
     * - Average Slippage
     * - Standard Deviation of Slippage
     * - Maximum Drawdown
     * - Skewness
     */
    class DLL_CLASS_RISKMANAGEMENT_EXPORTS VWAPOrderExecutionRiskMetrics final {
    public:
        /**
         * @brief Computes the average slippage between executed prices and VWAP.
         * @param executedPrices Vector of executed trade prices.
         * @param vwapPrices Vector of VWAP prices for each corresponding trade.
         * @return Average slippage value.
         * @throws std::invalid_argument if input vectors have different sizes or are empty.
         */
        static double computeAverageSlippage(const std::vector<double>& executedPrices,
            const std::vector<double>& vwapPrices);

        /**
         * @brief Computes the standard deviation of slippage values.
         * @param executedPrices Vector of executed trade prices.
         * @param vwapPrices Vector of VWAP prices for each corresponding trade.
         * @return Standard deviation of slippage.
         * @throws std::invalid_argument if input vectors have different sizes or are empty.
         */
        static double computeStdDevSlippage(const std::vector<double>& executedPrices,
            const std::vector<double>& vwapPrices);

        /**
         * @brief Computes the maximum drawdown of cumulative PnL.
         * @param pnlSeries Vector of PnL values over time.
         * @return Maximum drawdown value.
         * @throws std::invalid_argument if pnlSeries is empty.
         */
        static double computeMaxDrawdown(const std::vector<double>& pnlSeries);

        /**
         * @brief Computes the skewness of a data series.
         * @param data Vector of numeric values.
         * @return Skewness value.
         * @throws std::invalid_argument if data is empty.
         */
        static double computeSkewness(const std::vector<double>& data);
    };

} // namespace RiskManagement

#endif // VWAP_ORDER_EXECUTION_RISK_METRICS_H

