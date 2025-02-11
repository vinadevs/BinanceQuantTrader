/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "../KernelTrading/double_type.h"
#include "../KernelTrading/types.h"
#include "../KernelTrading/enums.h"

#include <string>
#include <sstream>
#include <type_traits>
#include <exception>

namespace OrderManagement {
    // This helper will convert order paramter types to std::string
	namespace TypeToStringUtils {
        template <typename T>
        std::string ToString(const T& val) {
            if constexpr (std::is_same_v<T, binapi::e_side>) {
                return binapi::e_side_to_string(val);
            }
            else if constexpr (std::is_same_v<T, binapi::e_type>) {
                return binapi::e_type_to_string(val);
            }
            else if constexpr (std::is_same_v<T, binapi::e_time>) {
                return binapi::e_time_to_string(val);
            }
            else if constexpr (std::is_same_v<T, binapi::e_trade_resp_type>) {
                return binapi::e_trade_resp_type_to_string(val);
            }
            if constexpr (std::is_arithmetic_v<T>) {
                // For arithmetic types (int, float, double, etc.)
                return std::to_string(val);
            }
            else {
                throw std::runtime_error("Unsupported type.");
            }
        }
	};
};