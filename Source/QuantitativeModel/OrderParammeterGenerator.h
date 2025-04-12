/********************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"

#include "QuantOrderParammeter.h"

namespace IndicatorNSignals {
    class TradingHints;
};

namespace ComplianceNRegulatory {
    class BinanceTradingRules;
};

namespace QuantitativeModel {

    class DLL_CLASS_QUANTITATIVEMODEL_EXPORTS
        OrderParammeterGenerator final {
    public:
        OrderParammeterGenerator(
            const ComplianceNRegulatory::BinanceTradingRules* tradingRules,
            LibraryUtils::Logger* logger);
        ~OrderParammeterGenerator();
		// Generate order parammeter based on the trading hints and binance exchange rules
        // https://developers.binance.com/docs/binance-spot-api-docs/filters
        QuantOrderParammeter Generate(
            const IndicatorNSignals::TradingHints* hints);
    private:
        const ComplianceNRegulatory::BinanceTradingRules* m_tradingRules{ nullptr };
		LibraryUtils::Logger* m_logger{ nullptr };
    };
};
