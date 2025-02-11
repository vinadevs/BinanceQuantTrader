/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

// ----------------------------------------------------------------------------
// Feature updated,bug fixed and codebase ported to windows platform, tested on 
// Visual Studio 2022
// Visual Studio 2019
// Visual Studio 2017
// Developer: vinadevs
// ----------------------------------------------------------------------------

#ifndef __binapi__iofmt_hpp
#define __binapi__iofmt_hpp

#include <iomanip>

namespace binapi {

constexpr std::ios_base::fmtflags iofmt = std::ios_base::fixed|std::ios_base::showpoint|std::ios_base::showbase;

} // ns binapi

#endif // __binapi__iofmt_hpp
