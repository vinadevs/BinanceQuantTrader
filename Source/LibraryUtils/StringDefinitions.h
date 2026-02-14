/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include <string>

namespace StringDefinitions {
	const std::string BinanceExchangeCancelledStatus = "CANCELED";
	// spot trading order client order id prefixes
	const std::string BQTNewBuyOrder = "BQTNewBuyOrder";
	const std::string BQTNewSellOrder = "BQTNewSellOrder";
	// future trading order client order id prefixes
	const std::string BQTNewLongOrder = "BQTNewLongOrder";
	const std::string BQTNewShortOrder = "BQTNewShortOrder";
	const std::string BQTCancelOrder = "BQTCancelOrder";
};
