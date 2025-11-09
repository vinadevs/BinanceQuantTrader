#include "../RiskManagement/VWAPOrderExecutionRiskMetrics.h"

#define BOOST_TEST_MODULE OrderExecutionRiskMetricsTest
#include <boost/test/included/unit_test.hpp>

#include <vector>
#include <cmath>

using namespace RiskManagement;

/**
 * @brief Test average slippage calculation.
 *
 * Checks a simple case where execution prices are exactly 1 unit higher than VWAP prices.
 * The expected average slippage should be 1.0.
 */
BOOST_AUTO_TEST_CASE(test_computeAverageSlippage) {
    std::vector<double> exec = { 101, 102, 103 };
    std::vector<double> vwap = { 100, 101, 102 };
    BOOST_CHECK_CLOSE(VWAPOrderExecutionRiskMetrics::computeAverageSlippage(exec, vwap), 1.0, 1e-9);
}

/**
 * @brief Test standard deviation of slippage.
 *
 * In this test, all execution prices exceed VWAP prices by exactly 1 unit,
 * so the slippage values are constant and standard deviation should be zero.
 */
BOOST_AUTO_TEST_CASE(test_computeStdDevSlippage) {
    std::vector<double> exec = { 101, 102, 103 };
    std::vector<double> vwap = { 100, 101, 102 };
    BOOST_CHECK_CLOSE(VWAPOrderExecutionRiskMetrics::computeStdDevSlippage(exec, vwap), 0.0, 1e-9);
}

/**
 * @brief Test maximum drawdown calculation.
 *
 * This test case has a PnL series with a peak at 120 and a trough at 90,
 * so the maximum drawdown should be 30 (difference between peak and trough).
 */
BOOST_AUTO_TEST_CASE(test_computeMaxDrawdown) {
    std::vector<double> pnl = { 100, 110, 105, 120, 115, 90 };
    BOOST_CHECK_CLOSE(VWAPOrderExecutionRiskMetrics::computeMaxDrawdown(pnl), 30.0, 1e-9);
}

/**
 * @brief Test skewness calculation.
 *
 * The input data is symmetric (1, 2, 3, 4, 5),
 * so the skewness should be approximately zero.
 */
BOOST_AUTO_TEST_CASE(test_computeSkewness) {
    std::vector<double> data = { 1, 2, 3, 4, 5 };
    double skew = VWAPOrderExecutionRiskMetrics::computeSkewness(data);
    BOOST_CHECK_SMALL(skew, 1e-9);
}
