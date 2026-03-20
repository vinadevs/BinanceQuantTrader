/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"

namespace QuantitativeModel {

	// TradeFlowImbalance (TFI) measures the imbalance between buy and sell
	// trades (filled already) over a specific time window.
	class DLL_CLASS_QUANTITATIVEMODEL_EXPORTS TradeFlowImbalance final
	{
		double m_buyVolume = 0;
		double m_sellVolume = 0;

	public:
		inline void onTradeUpdate(double qty, bool isBuy)
		{
			if (isBuy)
				m_buyVolume += qty;
			else
				m_sellVolume += qty;
		}

		// TFI = \frac{BuyVolume - SellVolume}{BuyVolume + SellVolume}
		/* -1  -> all sell → sell pressure
		    0  -> balanced trade flow
		   +1  -> all buy → buy pressure */
		inline double CalculateValue() const noexcept
		{
			double total = m_buyVolume + m_sellVolume;

			if (total == 0)
				return 0;

			return (m_buyVolume - m_sellVolume) / total;
		}

		inline void reset()
		{
			m_buyVolume = 0;
			m_sellVolume = 0;
		}
	};

};	// namespace QuantitativeModel