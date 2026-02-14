/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#ifndef _DTF_HPP_
#define _DTF_HPP_

#include <string>

#include <cstdint>

#include "dlldefine.h"

namespace dtf {

    /*************************************************************************************************/

    // always in nanoseconds resolution
    std::uint64_t DLL_CLASS timestamp(int offset = 0 /*in hours*/);

    struct DLL_CLASS flags {
        enum : std::size_t {
            yyyy_mm_dd = 1u << 0u
            , dd_mm_yyyy = 1u << 1u
            , sep1 = 1u << 2u // 2018-12-11 13:58:56
            , sep2 = 1u << 3u // 2018.12.11-13.58.59
            , sep3 = 1u << 4u // 2018.12.11-13:58:59
            , secs = 1u << 5u // seconds resolution
            , msecs = 1u << 6u // milliseconds resolution
            , usecs = 1u << 7u // microseconds resolution
            , nsecs = 1u << 8u // nanoseconds resolution
        };
    };

    enum { bufsize = 32 };

    // format as number in c-string representation
    std::size_t DLL_CLASS timestamp_to_chars(
        char* buf
        , std::uint64_t ts
        , std::size_t f = flags::msecs
    );

    std::string DLL_CLASS timestamp_to_str(
        std::uint64_t ts
        , std::size_t f = flags::msecs
    );

    std::string DLL_CLASS timestamp_str(std::size_t f = flags::msecs, int offset = 0);

    // formats as date-time string
    // returns the num of bytes placed
    std::size_t DLL_CLASS timestamp_to_dt_chars(
        char* ptr // dst buf with at least 'bufsize' bytes
        , std::uint64_t ts
        , std::size_t f = flags::yyyy_mm_dd | flags::sep1 | flags::msecs
    );

    std::string DLL_CLASS timestamp_to_dt_str(
        std::uint64_t ts
        , std::size_t f = flags::yyyy_mm_dd | flags::sep1 | flags::msecs
    );

    std::string DLL_CLASS timestamp_dt_str(
        std::size_t f = flags::yyyy_mm_dd | flags::sep1 | flags::msecs
        , int offset = 0
    );

    /*************************************************************************************************/

    // gets the respective flags for date-time string
    std::size_t DLL_CLASS dt_str_flags(const char* buf, std::size_t n);

    /*************************************************************************************************/

} // ns dtf

#ifdef DTF_HEADER_ONLY
#   include "dtf.cpp"
#endif // DTF_HEADER_ONLY

#endif // _DTF_HPP_
