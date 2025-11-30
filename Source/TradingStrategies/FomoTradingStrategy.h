/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include "../LibraryUtils/BQTQueue.h"
#include "../IndicatorNSignals/TradingSignalService.h"

#include "TradingStrategyBase.h"

#include <memory>
#include <string>
#include <vector>
#if USE_MULTITHREADING
#include <queue>
#include <mutex>
#include <condition_variable>
#endif

namespace MarketData {
	class RealTimeMarketData;
}

namespace UserAccount {
	class Trader;
}

namespace ComplianceNRegulatory {
	class BinanceTradingRules;
}

namespace QuantitativeModel {
	class OrderParammeterGenerator;
}

namespace tinyxml2 {
	class XMLDocument;
}

namespace RiskManagement {
	class SpotRiskEngine;
}

// The fear of missing out, or FOMO, refers to the feeling or 
// perception that others are having more fun, living better lives,
// or experiencing better things than you are

// THIS IS AN AUTO STRATEGY

namespace TradingStrategies {
	class DLL_CLASS_TRADING_TRATEGIES_EXPORTS
		FomoTradingStrategy 
		: public TradingStrategyBase,
		  public IndicatorNSignals::TradingHintsListener
	{
	public: 
		FomoTradingStrategy(const std::string& strategyCfgPath,
							MarketData::RealTimeMarketData* marketData,
							UserAccount::Trader* trader,
							ComplianceNRegulatory::BinanceTradingRules* tradingRules);

		virtual ~FomoTradingStrategy();

		bool OnReceivedTradingHints(const IndicatorNSignals::TradingHints* hints) override;
		
		// when an order is fully filled and exchange sends back an ack message
		void OnOrderFilledAck(const OrderManagement::BinanceNewOrder* filledOrder);
		// when an order partially filled and exchange sends back an ack message
		void OnOrderPartiallyFilledAck(const OrderManagement::BinanceNewOrder* partiallyFilledOrder);
		// when an order is rejected and exchange sends back an ack message
		void OnOrderRejectedAck(const OrderManagement::BinanceNewOrder* rejectedOrder);

		void ReportTradeResults(const std::string& symbol) override;

		void InitializeParameters(const std::string& strategyCfgPath) override;

		void StartTrade() override;

		void StopTrade() override;
	private:
		void CreateTradingSignalServices();
		void SubscribeTargetSymbols();
		void UnsubscribeTargetSymbols();
		void CreatePortfolioManagement();
		void CreateOrderParameterGenerator();
		void CreateBinanceExchangeProfile();
		void CreateRiskManagementEngine();
		bool TradeAsHints(const IndicatorNSignals::TradingHints* hints);

#if USE_MULTITHREADING
		void TradingLoop();

		std::unique_ptr<tinyxml2::XMLDocument> m_strategyCfgXml;
		std::unique_ptr<QuantitativeModel::OrderParammeterGenerator> m_orderParammeterGenerator;
		std::vector<std::string> m_targetTradeSymbols;
		std::mutex m_tradingHintsMutex;
		std::condition_variable m_tradingHintCond; // avoid polling thread
		std::atomic<bool> m_hasNewTradingHint{ false }; // lock free thread
		// customize queue with thread safe protection
		MultipleThreads::BQTQueue<const IndicatorNSignals::TradingHints*> m_tradingHintsQueue;
#endif
		std::unique_ptr<IndicatorNSignals::TradingSignalService> m_tradingSignalService;
		std::unique_ptr < RiskManagement::SpotRiskEngine> m_spotRiskEngine;
	};
};