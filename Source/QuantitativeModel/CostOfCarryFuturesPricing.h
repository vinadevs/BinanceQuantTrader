/********************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"

#include <cmath>
#include <algorithm>
#include <vector>
#include <numeric>

namespace QuantitativeModel {

   class CostOfCarryFuturesPricer final 
   {
   public:
       struct ModelParams final
       {
           double m_spotMiddlePrice; // 0.5*(spot.bid.price + spot.ask.price);
           double m_fundingRate; // per year
           std::size_t m_timeToExpiry; // year fraction
       };

       double Compute(const ModelParams& param) const noexcept;
   };
}; // namespace QuantitativeModel
