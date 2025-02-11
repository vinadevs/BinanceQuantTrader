/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include <string.h>
#include <boost/algorithm/string/trim.hpp>

namespace StringUtils {
	inline bool IsConfigAttributeMatched(const char* left, const char* right)
	{
		return strcmp(left, right) == 0;
	}
	inline void StrimString(std::string& str)
	{
		boost::algorithm::trim(str);
	}
};
