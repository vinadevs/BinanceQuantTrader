/********************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include "CostOfCarryFuturesPricing.h"

using namespace QuantitativeModel;

double CostOfCarryFuturesPricer::Compute(const ModelParams& param) const noexcept
{
    // basisfair​=fundingRate⋅T⋅S
    return param.m_spotMiddlePrice * std::exp(param.m_fundingRate * param.m_timeToExpiry);
}