/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include <string>

namespace LibraryUtils {
	class Logger;
};

namespace binapi {
	namespace rest {
		struct account_info_t;
	};
};

namespace KernelTrading {
	class UserFutureAccount;
};

namespace BinanceAccountUtils {
	// This function queries the Binance spot account information.
	bool QueryBinanceSpotAccount(
		binapi::rest::account_info_t* account,
		LibraryUtils::Logger* logger = nullptr);

	// This function queries the Binance future account information.
	bool QueryBinanceFutureAccount(
		KernelTrading::UserFutureAccount* account,
		LibraryUtils::Logger* logger = nullptr);
};
