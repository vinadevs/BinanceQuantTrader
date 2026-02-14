/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#ifndef _ENUMS_HPP_
#define _ENUMS_HPP_

#include <cstddef>

#include "dlldefine.h"

namespace binapi {

    /*************************************************************************************************/

    enum class DLL_CLASS e_side : std::size_t {
        buy
        , sell
    };

    e_side DLL_CLASS e_side_from_string(const char* str);
    DLL_CLASS const char* e_side_to_string(e_side side);

    /*************************************************************************************************/

    enum class DLL_CLASS e_type : std::size_t {
        limit
        , market
        , stop_loss
        , stop_loss_limit
        , take_profit
        , take_profit_limit
        , limit_maker
    };

    e_type DLL_CLASS e_type_from_string(const char* str);
    DLL_CLASS const char* e_type_to_string(e_type type);

    /*************************************************************************************************/

    enum class DLL_CLASS e_time : std::size_t {
        GTC
        , IOC
        , FOK
    };

    e_time DLL_CLASS e_time_from_string(const char* str);
    DLL_CLASS const char* e_time_to_string(e_time time);

    /*************************************************************************************************/

    enum class DLL_CLASS e_freq : std::size_t {
        _100ms = 100
        , _1000ms = 1000
    };

    e_freq DLL_CLASS e_freq_from_string(const char* str);
    DLL_CLASS const char* e_freq_to_string(e_freq freq);

    /*************************************************************************************************/

    enum class DLL_CLASS e_levels : std::size_t {
        _5 = 5
        , _10 = 10
        , _20 = 20
    };

    e_levels DLL_CLASS e_levels_from_string(const char* str);
    DLL_CLASS const char* e_levels_to_string(e_levels level);

    /*************************************************************************************************/

    enum class DLL_CLASS e_trade_resp_type : std::size_t {
        ACK
        , RESULT
        , FULL
        , TEST
        , UNKNOWN
    };

    e_trade_resp_type DLL_CLASS e_trade_resp_type_from_string(const char* str);
    DLL_CLASS const char* e_trade_resp_type_to_string(e_trade_resp_type resp);

    /*************************************************************************************************/

    enum class DLL_CLASS e_permissions : std::size_t {
        NONE = 1u << 0
        , SPOT = 1u << 1
        , MARGIN = 1u << 2
        , LEVERAGED = 1u << 3
        , TRD_GRP_002 = 1u << 4
        , TRD_GRP_003 = 1u << 5
        , TRD_GRP_004 = 1u << 6
        , TRD_GRP_005 = 1u << 7
    };

    e_permissions DLL_CLASS e_permissions_from_string(const char* str);
    DLL_CLASS const char* e_permissions_to_string(e_permissions resp);

    /*************************************************************************************************/

} // ns binapi

#endif // _ENUMS_HPP_
