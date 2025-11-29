/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include "ExchangeReporter.h"
#include "Trader.h"

#include <string>
#include <vector>
#include <memory>

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
		BinanceTrader final : public Trader
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
	
		std::unique_ptr<binapi::rest::account_info_t> m_binanceAccountInfo; // spot account info
		std::unique_ptr<ExchangeReporter> m_exchangeReporter;// trade result reporter
	};
};