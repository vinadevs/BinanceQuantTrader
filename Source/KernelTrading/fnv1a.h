/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#ifndef _FNV1A_HPP_
#define _FNV1A_HPP_

#include <cstdint>
#include <type_traits>

namespace binapi {

    /*************************************************************************************************/

    template<typename CharT>
    constexpr std::size_t ct_strlen(const CharT* s) {
        const CharT* b = s;
        for (; *s; ++s)
            ;

        return s - b;
    }

    /*************************************************************************************************/

    template<typename CharT>
    constexpr std::uint32_t fnv1a(const CharT* s, std::size_t len) {
        std::uint32_t seed = 0x811c9dc5;
        for (; len; --len, ++s) {
            seed = static_cast<std::uint32_t>(
                (seed ^ static_cast<std::uint32_t>(*s)) * static_cast<std::uint64_t>(0x01000193)
                );
        }

        return seed;
    }

    template<
        typename ConstCharPtr
        , typename = typename std::enable_if<
        std::is_same<ConstCharPtr, const char*>::value
        >::type
    >
    constexpr std::uint32_t fnv1a(ConstCharPtr s) {
        return fnv1a(s, ct_strlen(s));
    }

    template<typename CharT, std::size_t N>
    constexpr std::uint32_t fnv1a(const CharT(&s)[N]) {
        return fnv1a(s, N - 1);
    }

    /*************************************************************************************************/

} // ns binapi

#endif // _FNV1A_HPP_
