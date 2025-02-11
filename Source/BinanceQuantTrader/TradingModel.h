/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include <memory>

#include "../LibraryUtils/MacroUtils.h"

namespace tinyxml2 {
	class XMLDocument;
};

namespace LibraryUtils {
	class Logger;
};

namespace PortfolioManager {
	class PortfolioInvestmentBinance;
};

namespace MarketData {
	class RealTimeMarketData;
};

namespace UserAccount {
	class BinanceTrader;
};

namespace ComplianceNRegulatory {
	class BinanceTradingRules;
}

namespace RiskManagement {
	class RiskManager;
}

namespace TradingStrategies {
	class SingleStrategyHost;
	class TradingStrategyBase;
#if USE_TEST_TRADING
	class StrategyMessageServer;
#endif
};

namespace BinanceQuantTrader {

	// A TradingModel typically refers to a class or structure that encapsulates
	// the logic behind a trading strategy. This class might include PortfolioInvestment
	// RealTimeMarketData, Trader, TradingStrategy,...

	class TradingModel final
	{
	public:
		TradingModel(const tinyxml2::XMLDocument* configBQTXml,
					 const tinyxml2::XMLDocument* configAccessKeyXml);
		~TradingModel();

		// Only single model is allowed to create...
		DISABLE_COPY_AND_ENABLE_MOVE(TradingModel);

		// After this function called, Strategy/Algo will start listenning on
		// indicators/signals events (market data, macro data,...)
		// This is infinitive wait loop, TODO: so need to implement a way to shut down system...
		void RunModel();
	private:
		// -All trading components will be initiated in this function
		// -When we add new modules, it should be initiated here
		void PrepareTradingComponents(
			const tinyxml2::XMLDocument* configBQTXml,
			const tinyxml2::XMLDocument* configAccessKeyXml);

		// Trading components
		std::unique_ptr<PortfolioManager::PortfolioInvestmentBinance> m_portfolio;
		std::unique_ptr<MarketData::RealTimeMarketData> m_marketData;
		std::unique_ptr<UserAccount::BinanceTrader> m_trader;
		std::unique_ptr<ComplianceNRegulatory::BinanceTradingRules> m_tradingRules;
		std::unique_ptr<RiskManagement::RiskManager> m_riskManager;
		std::unique_ptr<TradingStrategies::TradingStrategyBase> m_strategy;
		std::unique_ptr<TradingStrategies::SingleStrategyHost> m_strategyHost;
#if USE_TEST_TRADING
		std::unique_ptr<TradingStrategies::StrategyMessageServer> m_strategyMessageServer;
#endif
		std::unique_ptr<LibraryUtils::Logger> m_logger;
	};
};