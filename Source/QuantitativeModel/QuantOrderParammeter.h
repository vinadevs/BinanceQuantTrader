/********************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be useddistributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"

#include "../RestAPI/RestAPI.h"
#include "../RestAPI/BinanceAPI.h"

#include <string>

namespace QuantitativeModel {

    struct DLL_CLASS_QUANTITATIVEMODEL_EXPORTS
           QuantOrderParammeter final {
           std::string m_symbol;
           binapi::e_side m_side;
           binapi::e_type m_type;
           binapi::e_time m_time;
           double m_amount{ 0 };
           double m_price{ 0 };
           double m_stopPrice{ 0 };

           QuantOrderParammeter() = default;

           // Constructor
           QuantOrderParammeter(
               const std::string& symbol,
               binapi::e_side side,
               binapi::e_type type,
               binapi::e_time time,
               const double amount,
               const double price,
               const double stopPrice
           )
               : m_symbol(symbol),
                 m_side(side),
                 m_type(type),
                 m_time(time),
                 m_amount(amount),
                 m_price(price),
                 m_stopPrice(stopPrice) {}
       };
};
