/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include <type_traits>

// This util use to iterate over the elements of the enum class  
// The class needs to define the last value END_NOT_USING

// Usage: use as below for loop or macro FOR_LOOP_ENUM(iter, enumType)
// for (auto type = EnumBegin<SubscriptionHandleType>(); type != EnumEnd<SubscriptionHandleType>();
//    type = static_cast<SubscriptionHandleType>(static_cast<unsigned>(type) + 1))
// {
//    std::cout << static_cast<unsigned>(type) << '\n';
// }

template<typename Enum>
constexpr auto EnumBegin() { 
	return static_cast<Enum>(0); 
}

template<typename Enum>
constexpr auto EnumEnd() { 
	return static_cast<Enum>(static_cast<std::underlying_type_t<Enum>>(Enum::END_NOT_USING));
}

#define FOR_LOOP_ENUM(iter, enumType)  \
for (auto iter = EnumBegin<enumType>(); \
	iter != EnumEnd<enumType>(); \
	iter = static_cast<enumType>(static_cast<unsigned>(iter) + 1))
