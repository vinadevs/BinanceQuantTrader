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
		class account_info_t;
	};
};

namespace BinanceAccountUtils {
	bool QueryBinanceAccount(
		binapi::rest::account_info_t* account,
		LibraryUtils::Logger* logger = nullptr);
};
