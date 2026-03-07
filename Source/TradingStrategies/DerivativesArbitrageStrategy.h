/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"

#include "../MarketData/SynchronousMarketData.h"
#include "../MarketData/MarketDataObserver.h"
#include "../KernelTrading/bqt_symbol.h"

#include "TradingStrategyBase.h"
#include "InstrumentQuoter.h"

#include <string>
#include <memory>
#include <vector>
#include <mutex>

namespace UserAccount {
	class Trader;
}

namespace ComplianceNRegulatory {
	class BinanceTradingRules;
}

namespace MarketData {
	class RealTimeMarketData;
	class MarketDataSubject;
}

namespace QuantitativeModel {
	class MarketDataAnalyzer;
	class CostOfCarryFuturesPricer;
}

namespace RiskManagement {
	class DerivativesRiskModel;
}

// The DerivativesArbitrageStrategy class is an automated trading strategy designed to exploit future market arbitrage opportunities.
// The strategy uses model Cash & Carry arbitrage principles to identify and execute trades that capitalize
// on price discrepancies between spot and futures markets. A kind of low-risk, market-neutral strategy
// that involves buy/sell the underlying asset in the spot market while simultaneously long/short a futures contract on that asset
// with a zero delta hedging to lock in a risk-free profit.

// Flow Models:
// MarketData
// ↓
// FairValueModel(pricing)
// ↓
// GreeksModel(delta / gamma / theta)
// ↓
// RiskModel(limits / kill switch)
// ↓
// CashCarryStrategy(quote + hedge)

namespace TradingStrategies {

    enum class ArbitrageType : unsigned
	{
		// Buy the spot and sell the future when the future is overpriced, or sell the spot
		// and buy the future when the future is underpriced
		QUOTE_SPOT_AND_HEDGE_FUTURE = 0,
		// Sell the future and buy the spot when the future is overpriced, or buy the future 
		// and sell the spot when the future is underpriced
		QUOTE_FUTURE_AND_HEDGE_SPOT = 1,
		// Dynamically choose to quote and hedge based on market conditions, 
		// such as liquidity, volatility, and order book depth
		DYNAMIC_QUOTE_AND_HEDGE = 2,
	};

	struct MarketDataSnapshot final
	{
		std::string m_symbol;
		double m_spotBestBidPrice{ 0.0 };
		double m_spotBestAskPrice{ 0.0 };
		double m_spotLastPrice{ 0.0 };
		double m_spotLastTradeVolume{ 0.0 };
		double m_futureBestBidPrice{ 0.0 };
		double m_futureBestAskPrice{ 0.0 };
		double m_futureLastPrice{ 0.0 };
		double m_futureLastTradeVolume{ 0.0 };
		double m_fundingRate{ 0.0 };
		double m_marketSpotCummulativeVolume{ 0.0 };
		double m_marketFutureCummulativeVolume{ 0.0 };
		double m_timeToExpiry{ 0.0 };
	};

	struct SymbolMononitorInfo final
	{
		std::string m_symbol;
		bool m_isSpotSymbol{ false };
		bool m_isFutureSymbol{ false };
	};

	class DLL_CLASS_TRADING_TRATEGIES_EXPORTS
		DerivativesArbitrageStrategy :
		public TradingStrategyBase, // strategy core
		public MarketData::MarketDataObserver, // market data update
		public LibraryUtils::AlarmSystem // schedule trading
	{
	public:
		DerivativesArbitrageStrategy(const std::string& strategyCfgPath,
			MarketData::RealTimeMarketData* marketData,
			UserAccount::Trader* trader,
			ComplianceNRegulatory::BinanceTradingRules* tradingRules);

		virtual ~DerivativesArbitrageStrategy();

		/* Override MarketDataObserver interface for receiving market data updates */
		// Spot order book ticker update event
		bool OnIndividualBookTickerChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		// Spot trade data update event
		bool OnTradeChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		// Future order book ticker update event
		bool OnBookDataFutureChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		// Future trade data update event
		bool OnTradeDataFutureChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		// Future funding rate update event
		bool OnFutureFundingDataChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		
		// when an order is opening and exchange sends back an ack message
		virtual void OnOrderOpeningPositionAck(const OrderManagement::BinanceNewOrder* openingOrder) override;
		// when an order is closed and exchange sends back an ack message
		virtual void OnOrderClosedPositionAck(const OrderManagement::BinanceNewOrder* closedOrder) override;
		// when an order is liquidated and exchange sends back an ack message
		virtual void OnOrderLiquidatedPositionAck(const OrderManagement::BinanceNewOrder* liquidatedOrder) override;
		// when an order is margin called and exchange sends back an ack message
		virtual void OnOrderMarginCalledPositionAck(const OrderManagement::BinanceNewOrder* marginCalledOrder) override;

		void ReportTradeResults(const std::string& symbol) override;

		void InitializeParameters(const std::string& strategyCfgPath) override;

		void StartTrade() override;

		void StopTrade() override;

		// - This function is called when the alarm is triggered, which is used to send orders
		void OnAlarmTriggered(const int passToDerived = 0) override;
	private:
		void InitializeMarketDataAnalyzer();
		void InitializeMarketDataSnapshots();
		void InitializeInstrumentQuoters();
		void SetupOrderScheduler();
		void CreateBinanceExchangeProfile();
		void CreatePortfolioManagement();
		void CreateRiskManagementEngines();
		void CreatePricingModels();
		void PrepareTargetMonitorSymbols();
		void SubscribeTargetSymbols();
		void UnsubscribeTargetSymbols();

		KernelTrading::BqtSymbol& GetSymbolInfo(const std::string& symbol);

		// The minimum price difference between the futures and the spot to enter a trade
		// This threshold should be greater than the transaction costs (including trading fees,
		// funding fees, and slippage) to ensure profitability
		double m_entryThresholdDiff{ 0.0 };
		// The offset for quoting, which is the minimum distance between our theoretical price and the bid/offer prices of our orders,
		double m_quoteOffset{0.0};
		// The tick width of the trading instrument, which is the minimum price increment allowed by the exchange, used for aligning our order prices to valid levels
		double m_tickWidth{0.0};
		// How to quote and hedge, either quote the spot and hedge with future, or quote the future and hedge with spot, 
		// or dynamically choose to quote and hedge based on market conditions
		ArbitrageType m_arbitrageType{ ArbitrageType::QUOTE_SPOT_AND_HEDGE_FUTURE };
		std::vector<std::string> m_targetTradeSymbols;
		std::unordered_map<std::string, KernelTrading::BqtSymbol> m_symbolMonitorInfos; // symbol info for monitoring and trading, including whether it's spot or future symbol
		std::unordered_map<std::string, std::unique_ptr<MarketDataSnapshot>> m_marketDataSnapshots; // latest market data snapshot for each symbol
		std::unordered_map<std::string, InstrumentQuoter> m_instrumentQuoters;	 // instrument quoter for each symbol, responsible for quoting for the symbol
		std::unique_ptr<QuantitativeModel::MarketDataAnalyzer> m_marketDataAnalyzer;
		std::unique_ptr <RiskManagement::DerivativesRiskModel> m_riskModel;
		std::unique_ptr<QuantitativeModel::CostOfCarryFuturesPricer> m_fairValueModel;
		std::mutex m_marketDataMutex; // mutex to protect market data snapshots when accessed by multiple threads
	};
};