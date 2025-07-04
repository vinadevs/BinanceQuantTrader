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

#include <string>
#include <memory>
#include <vector>

#if USE_BACK_TEST_TRADING
#include "../MiddlewareMQ/BqtJsonMessage.h"
#endif

namespace tinyxml2 {
	class XMLElement;
};

namespace UserAccount {

	// -In the context of financial markets, a trader is an individual 
	// or entity that buys and sells financial instruments,
	// such as stocks, bonds, commodities, currencies, or derivatives,
	// with the goal of making a profit.

	// This class will manage trade activities like buy/sell/report...
	class DLL_CLASS_USERACCOUNT_EXPORTS Trader
	{
	public:
		Trader() {};
		virtual ~Trader();

		// Create new position based on the provided quantitative parameters.
		virtual bool CreateNewPosition(
			const QuantitativeModel::QuantOrderParammeter& param) = 0;

		// Cancel all open positions for the given symbol.
		virtual bool CancelAllOpenPositions(
			const std::string& symbol) = 0;

		// Update account information, such as balances and positions.
		virtual void CreatePortfolioManagement(
			const std::vector<std::string>& targetTradeSymbols) = 0;
#if USE_BACK_TEST_TRADING  
		// Handle downstream acknowledgment messages from the simulator.
		virtual void HandleDownstreamAckMessage(
			const MiddlewareMQ::BqtJsonMessage& message) = 0;
#endif
	protected:
		std::unique_ptr<LibraryUtils::Logger> m_logger;
	};
};