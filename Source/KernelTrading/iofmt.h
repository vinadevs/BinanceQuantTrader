/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#ifndef _IOFMT_HPP_
#define _IOFMT_HPP_

#include <iomanip>

namespace binapi {

	constexpr std::ios_base::fmtflags iofmt = std::ios_base::fixed | std::ios_base::showpoint | std::ios_base::showbase;

} // ns binapi

#endif // _IOFMT_HPP_
