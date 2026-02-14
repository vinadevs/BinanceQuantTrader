/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"
#include "../LibraryUtils/Logger.h"
#include "../QuantitativeModel/QuantOrderParammeter.h"
#include "../OrderManagement/PositionManager.h"

#include <string>
#include <memory>
#include <vector>
#include <utility>

#if USE_BACK_TEST_TRADING
#include "../MiddlewareMQ/BqtJsonMessage.h"
#endif

#include "TraderAndStrategy.h"

namespace tinyxml2 {
	class XMLElement;
};

namespace TradingStrategies {
	class TradingStrategyBase;
};

namespace PortfolioManager {
	class PortfolioInvestmentBinance;
}

namespace RiskManagement {
	class RiskManager;
}

namespace ComplianceNRegulatory {
	class BinanceTradingRules;
}

namespace UserAccount {

	// -In the context of financial markets, a trader is an individual 
	// or entity that buys and sells financial instruments,
	// such as stocks, bonds, commodities, currencies, or derivatives,
	// with the goal of making a profit.

	// This class will manage trade activities like buy/sell/report...

	// WorkedOrderIdentification is a pair where:
	// - first element is a boolean indicating success or failure of the order creating operation
	// - second element is a string that can be an client order ID
	using WorkedOrderIdentification = std::pair<bool, std::string>;

	class DLL_CLASS_USERACCOUNT_EXPORTS Trader
	{
	public:
		Trader(PortfolioManager::PortfolioInvestmentBinance* portfolio,
			ComplianceNRegulatory::BinanceTradingRules* tradingRules,
			RiskManagement::RiskManager* riskManager)
			: m_portfolio(portfolio),
			m_tradingRules(tradingRules),
			m_riskManager(riskManager)
		{
			m_positionManager = std::make_unique<OrderManagement::PositionManager>();
		}

		virtual ~Trader();

		// Create new position based on the provided quantitative parameters.
		virtual WorkedOrderIdentification CreateNewPosition(
			const QuantitativeModel::QuantOrderParammeter& param) {
			return WorkedOrderIdentification();
		}

		// Cancel all open positions for the given symbol.
		virtual bool CancelAllOpenPositions(
			const std::string& symbol) { return false; }

		// Cancel an open position by its client order ID.
		virtual WorkedOrderIdentification CancelOpenPosition(
			const std::string& clientOrderId) {
			return WorkedOrderIdentification();
		}

		// Update account information, such as balances and positions.
		virtual void CreatePortfolioManagement(
			const std::vector<std::string>& targetTradeSymbols) {}

		// Create portfolio management for a specific symbol.
		virtual void CreatePortfolioManagement(
			const std::string& symbol) {}

		// Set up the trading strategy to be used for trading.
		// This method has to be called at strategy initialization time.
		virtual void UseThisStrategyToTrade(TradingStrategies::TradingStrategyBase* strategy) {}

		PortfolioManager::PortfolioInvestmentBinance* GetPortfolio() const { return m_portfolio; }

		OrderManagement::PositionManager* GetPositionManager() const { return m_positionManager.get(); }

		ComplianceNRegulatory::BinanceTradingRules* GetTradingRules() const { return m_tradingRules; }

		RiskManagement::RiskManager* GetRiskManager() const { return m_riskManager; }

		TraderType GetTraderType() const { return m_traderType; }

		TraderAndStrategy& GetTraderAndStrategyMapping() { return m_traderAndStrategy; }
#if USE_BACK_TEST_TRADING  
		// Handle downstream acknowledgment messages from the simulator.
		virtual void HandleDownstreamAckMessage(
			const MiddlewareMQ::BqtJsonMessage& message) {}
#endif
	protected:
		TraderType m_traderType{ TraderType::UNDEFINED_TRADER }; // type of trader
		std::unique_ptr<LibraryUtils::Logger> m_logger; // log message
		TradingStrategies::TradingStrategyBase* m_tradingStrategy{ nullptr }; // trading strategy to use
		PortfolioManager::PortfolioInvestmentBinance* m_portfolio{ nullptr }; // list of assets to trade
		ComplianceNRegulatory::BinanceTradingRules* m_tradingRules{ nullptr }; // exchange compliance and regulatory
		RiskManagement::RiskManager* m_riskManager{ nullptr };  // manage trading risks
		std::unique_ptr<OrderManagement::PositionManager> m_positionManager; // manage worked trading positions
		// trader and strategy mapping
		TraderAndStrategy m_traderAndStrategy;
	};
};