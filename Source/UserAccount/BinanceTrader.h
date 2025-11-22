/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include "../OrderManagement/PositionManager.h"

#include "ExchangeReporter.h"
#include "Trader.h"

#include <string>
#include <vector>
#include <memory>

namespace PortfolioManager {
	class PortfolioInvestmentBinance;
}

namespace RiskManagement {
	class RiskManager;
}

namespace ComplianceNRegulatory {
	class BinanceTradingRules;
}

namespace tinyxml2 {
	class XMLElement;
};

namespace TradingStrategies {
	class TradingStrategyBase;
};

namespace UserAccount {

	// This class will manage binance spot trade activities like buy/sell/report...
	// It will also manage the portfolio, trading rules, and risk management.
	class DLL_CLASS_USERACCOUNT_EXPORTS
		BinanceTrader : public Trader
	{
	public:
		BinanceTrader(const tinyxml2::XMLElement* reportCfg, 
			          PortfolioManager::PortfolioInvestmentBinance* portfolio,
					  ComplianceNRegulatory::BinanceTradingRules* tradingRules,
					  RiskManagement::RiskManager* riskManager);

		////////////// UPSTREAM PROCESSING /////////////////////////////

		void UseThisStrategyToTrade(TradingStrategies::TradingStrategyBase* strategy) override;

		WorkedOrderIdentification CreateNewPosition(const QuantitativeModel::QuantOrderParammeter& param) override;

		bool CancelAllOpenPositions(const std::string& symbol) override;

		WorkedOrderIdentification CancelOpenPosition(const std::string& clientOrderId) override;

		void UpdateAccountInfo();

		void ReportTradeResults(const std::string& symbol);

		void CreatePortfolioManagement(const std::vector<std::string>& targetTradeSymbols) override;

		PortfolioManager::PortfolioInvestmentBinance* GetPortfolio() const { return m_portfolio; }
		
		OrderManagement::PositionManager* GetPositionManager() const { return m_positionManager.get(); }

		ComplianceNRegulatory::BinanceTradingRules* GetTradingRules() const { return m_tradingRules; }

		RiskManagement::RiskManager* GetRiskManager() const { return m_riskManager; }

		binapi::rest::account_info_t* GetBinanceAccountInfo() const { return m_binanceAccountInfo.get(); }

		std::vector<double> GetOrderExecutedPrices(const std::string& symbol) const;

		std::vector<double> GetOrderExecutedSlippagePrices(const std::string& symbol) const;

		std::vector<double> GetPnLSeries(const std::string& symbol) const;

		////////////// DOWNSTREAM PROCESSING /////////////////////////////
#if USE_BACK_TEST_TRADING  
		void HandleDownstreamAckMessage(const MiddlewareMQ::BqtJsonMessage& message) override;
#endif
	private:
		double CalculateTradeValue(
			const double quality,
			const double refPrice);

		PortfolioManager::PortfolioInvestmentBinance* m_portfolio{ nullptr }; // list of assets to trade
		ComplianceNRegulatory::BinanceTradingRules* m_tradingRules{ nullptr }; // exchange compliance and regulatory
		RiskManagement::RiskManager* m_riskManager{ nullptr };  // stop loss
		std::unique_ptr<binapi::rest::account_info_t> m_binanceAccountInfo;
		std::unique_ptr<OrderManagement::PositionManager> m_positionManager;
		std::unique_ptr<ExchangeReporter> m_exchangeReporter;
	};
};