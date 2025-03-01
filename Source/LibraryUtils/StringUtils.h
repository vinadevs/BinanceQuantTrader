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

	std::vector<std::string> DLL_CLASS_LIBRARYUTILS_EXPORTS 
		SplitAndTrimString(const std::string& str, char separator);
	std::vector<std::pair<std::string, std::string>> DLL_CLASS_LIBRARYUTILS_EXPORTS
		ParseStringPairs(const std::string& input);
};
