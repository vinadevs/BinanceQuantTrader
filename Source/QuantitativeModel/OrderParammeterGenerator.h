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
    struct TradingHints;
};

namespace ComplianceNRegulatory {
    class BinanceTradingRules;
};

namespace PortfolioManager {
    class PortfolioInvestmentBinance;
}

namespace OrderManagement {
    class PositionManager;
}

namespace RiskManagement {
    class RiskManager;
}

namespace QuantitativeModel {

    // The OrderParammeterGenerator class is responsible for generating order parameters 
    // based on trading hints and Binance exchange rules. It ensures compliance with 
    // Binance's trading filters and regulations while utilizing trading hints to 
    // create efficient and compliant order parameters. This class is intended for use 
    // in quantitative trading models and integrates with BinanceTradingRules for 
    // regulatory adherence and Logger for detailed logging and debugging purposes.

    class DLL_CLASS_QUANTITATIVEMODEL_EXPORTS
        OrderParammeterGenerator final {
    public:
        OrderParammeterGenerator(
            const ComplianceNRegulatory::BinanceTradingRules* tradingRules,
			const PortfolioManager::PortfolioInvestmentBinance* portfolio,
			const RiskManagement::RiskManager* riskManager,
			const OrderManagement::PositionManager* positionManager,
            LibraryUtils::Logger* logger);
        ~OrderParammeterGenerator();
		// Generate order parammeter based on the trading hints and binance exchange rules
        // https://developers.binance.com/docs/binance-spot-api-docs/filters
        QuantOrderParammeter Generate(
            const IndicatorNSignals::TradingHints* hints);
    private:
        const ComplianceNRegulatory::BinanceTradingRules* m_tradingRules{ nullptr };
        const PortfolioManager::PortfolioInvestmentBinance* m_portfolio{ nullptr };
        const RiskManagement::RiskManager* m_riskManager{ nullptr };
        const OrderManagement::PositionManager* m_positionManager{ nullptr };
		LibraryUtils::Logger* m_logger{ nullptr };
    };
};
