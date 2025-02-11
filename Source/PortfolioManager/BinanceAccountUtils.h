/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include "../KernelTrading/types.h"

#include <string>

namespace LibraryUtils {
	class Logger;
};

namespace BinanceAccountUtils {
	bool QueryBinanceAccount(
		binapi::rest::account_info_t& accountInfo,
		LibraryUtils::Logger* logger = nullptr);
};
