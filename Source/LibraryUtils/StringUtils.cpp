/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "StringUtils.h"

#include <regex>

std::vector<std::string> 
StringUtils::SplitAndTrimString(const std::string& str, char separator) {
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

// a function to parse string format to get list of pair of data 
// sample: {"A", "B"}; {"C", "D"};
std::vector<std::pair<std::string, std::string>>
StringUtils::ParseStringPairs(const std::string& input) {
    std::vector<std::pair<std::string, std::string>> result;
    const std::regex pairRegex(R"(\{([^,]+), ([^}]+)\})");
    std::sregex_iterator iter(input.begin(), input.end(), pairRegex);
    const std::sregex_iterator end;
    while (iter != end) {
        result.emplace_back((*iter)[1].str(), (*iter)[2].str());
        ++iter;
    }
    return result;
}