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

   // CostOfCarryFuturesPricer implements the cost-of-carry futures pricing model.
   // It calculates the theoretical futures price based on the spot price,
   // funding rate, and time to expiry. The model assumes no arbitrage and
   // continuous compounding of the funding rate.

   class DLL_CLASS_QUANTITATIVEMODEL_EXPORTS CostOfCarryFuturesPricer final 
   {
   public:
       struct ModelParams final
       {
           double m_spotMiddlePrice; // 0.5*(spot.bid.price + spot.ask.price);
           double m_fundingRate; // per year
		   double m_timeToExpiry;	 // year fraction
       };

       double Compute(const ModelParams& param) const noexcept;
   };
}; // namespace QuantitativeModel
