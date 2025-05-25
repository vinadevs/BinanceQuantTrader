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
#include <vector>

namespace CurlAPI {
	std::vector<std::string> DLL_CLASS_CURLAPI_EXPORTS
		GetBinanceListingSymbols(const std::string& symbolStatus);

	binapi::rest::exchange_info_t DLL_CLASS_CURLAPI_EXPORTS
		GetExchangeInfo();

} // namespace RestAPI
