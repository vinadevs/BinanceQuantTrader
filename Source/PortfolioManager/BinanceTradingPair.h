/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include "../KernelTrading/double_type.h"
#include "../KernelTrading/types.h"

#include <string>

namespace MarketData {
	class RealTimeMarketData;
}

namespace PortfolioManager {
	using BinanceBalance = binapi::rest::account_info_t::balance_t;
	struct DLL_CLASS_PORTFOLIOMANAGER_EXPORTS BinanceTradingPair
	{
	public:
		BinanceTradingPair(const std::string& tradingPair,
						   const MarketData::RealTimeMarketData* marketData,
						   const BinanceBalance& balance);
		void UpdateTradingPair(const BinanceBalance& balance);
		std::string GetSymbol() const { return m_tradingPairName; }
		binapi::double_type GetCash(const std::string& tradingPair) const;
		binapi::double_type GetQuantity() const;
	private:
		const std::string m_tradingPairName;
		BinanceBalance m_balance;
		const MarketData::RealTimeMarketData* m_marketData {nullptr};
	};
};