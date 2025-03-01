/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include "../MarketData/MarketDataObserver.h"

#include "IndicatorAndSignals.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace MarketData {
    class MarketDataSubject;
}

namespace tinyxml2 {
    class XMLElement;
};

namespace LibraryUtils {
    class Logger;
};

namespace PortfolioManager {
    class PortfolioInvestmentBinance;
}

namespace IndicatorNSignals {
    class TradingTrendDetector;
    class IndicatorAndSignalManager
    {
    public:
        IndicatorAndSignalManager();
        ~IndicatorAndSignalManager();
        void RegisterTradingHintsListener(TradingHintsListener* listener);
        void UnregisterTradingHintsListener(TradingHintsListener* listener);

        bool CreateNewTrendingAnalyzer(const std::string& symbol, const tinyxml2::XMLElement* parametersXml);
        bool RemoveTrendingAnalyzer(const std::string& symbol);
        TradingTrendDetector* GetTrendindAnalyzer(const std::string& symbol);
    private:
        std::unordered_map<std::string, std::unique_ptr<TradingTrendDetector>> m_trendingSignalAnalyzers;
    };

    // -This Service will manage signals and indicators for trading trend
    // -It will receive data feed from market data handlers and dispatch
    // data to trend detector...

    class DLL_CLASS_INDICATORNSIGNALS_EXPORTS
        TradingSignalService : public MarketData::MarketDataObserver
    {
    public:
        TradingSignalService(
            PortfolioManager::PortfolioInvestmentBinance* portfolio,
            const tinyxml2::XMLElement* signalXml);

        void RegisterTradingHintsListener(TradingHintsListener* listener);
        void UnregisterTradingHintsListener(TradingHintsListener* listener);

        bool OnIndividualBookTickerChange(
            MarketData::MarketDataSubject* marketData,
            const std::string& symbol) override;
    private:
        bool IsInvestmentPortfolioAssetEmpty() const;

        std::unique_ptr<IndicatorAndSignalManager> m_signalMgr;
        PortfolioManager::PortfolioInvestmentBinance* m_portfolio{ nullptr };
        std::unique_ptr<LibraryUtils::Logger> m_logger;
    };
};