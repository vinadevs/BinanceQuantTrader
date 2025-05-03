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
#include <vector>

namespace CurlAPI {
	std::vector<std::string> DLL_CLASS_CURLAPI_EXPORTS
		GetBinanceListingSymbols(const std::string& symbolStatus);
} // namespace RestAPI
