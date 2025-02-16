/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "StringUtils.h"

std::vector<std::string> StringUtils::SplitAndTrimString(const std::string& str, char separator) {
    std::vector<std::string> result;
    std::string temp;

    for (const char ch : str) {
        if (ch == separator) {
            if (!temp.empty()) {
                boost::algorithm::trim(temp);
                result.push_back(temp);
                temp.clear();
            }
        }
        else {
            temp += ch;
        }
    }

    if (!temp.empty()) {
        boost::algorithm::trim(temp);
        result.push_back(temp);
    }

    return result;
}