/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

// Trading hints is which Indicator/Signals sends to
// Strategy side. A hint will include trading opporntunities to help
// Strategy can take actions in long or short a crypto asset

#include "dlldefine.h"

#include "../KernelTrading/double_type.h"
#include "../KernelTrading/types.h"

#include <string>

namespace IndicatorNSignals {

	struct DLL_CLASS_INDICATORNSIGNALS_EXPORTS TradingHints
	{
		TradingHints(const std::string& s) : symbol(s) {}
		std::string symbol;
		bool isInvertedTrend{ false };
		bool isUpTrend{ false };
		bool isDownTrend{ false };
		bool shouldCancelAllOrder{ false };
		binapi::double_type windowBestBidPrice{ 0 };
		binapi::double_type windowBestAskPrice{ 0 };
		binapi::e_time timeInForce{ binapi::e_time::IOC };
		bool CanTrade() {
			return isUpTrend || isDownTrend;
		}
	};

	class DLL_CLASS_INDICATORNSIGNALS_EXPORTS TradingHintsListener
	{
	public:
		// Override function need to have try catch block
		// as we are throwing some exception when analysis data
		virtual bool OnReceivedTradingHints(const TradingHints* hints) = 0;
	};
};


