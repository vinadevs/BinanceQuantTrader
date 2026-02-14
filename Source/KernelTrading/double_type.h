/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#ifndef __TYPES_DOUBLE_TYPE_HPP_
#define __TYPES_DOUBLE_TYPE_HPP_

#include <boost/multiprecision/cpp_dec_float.hpp>

/*************************************************************************************************/

namespace binapi {

    using double_type = boost::multiprecision::number<
        boost::multiprecision::cpp_dec_float<8>
        , boost::multiprecision::et_off
    >;

} // ns binapi

/*************************************************************************************************/

#endif // __TYPES_DOUBLE_TYPE_HPP_
