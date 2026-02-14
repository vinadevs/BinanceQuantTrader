/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#ifndef _PAIRSLIST_HPP_
#define _PAIRSLIST_HPP_

#include <string>
#include <set>

#include "dlldefine.h"

namespace binapi {
    namespace rest {

        struct exchange_info_t;

    } // ns rest

    /*************************************************************************************************/

    // create the exchange available pairs list according to black and white lists
    std::set<std::string> DLL_CLASS process_pairs(
        const std::string& whitelist
        , const std::string& blacklist
        , const binapi::rest::exchange_info_t& exinfo
    );

    bool DLL_CLASS pair_in_pairs(const std::set<std::string>& pairs, const std::string& pair);

    void DLL_CLASS test_blackwhite_list();

    /*************************************************************************************************/

} // ns binapi

#endif // _PAIRSLIST_HPP_
