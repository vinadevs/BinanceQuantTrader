#pragma once

#include "dlldefine.h"

namespace QuantitativeModel {

	// MiddleBestPrice calculates the middle price between the best bid and best ask prices.
	struct DLL_CLASS_QUANTITATIVEMODEL_EXPORTS MiddleBestPrice final
	{
		double m_bestBidPrice = 0;
		double m_bestAskPrice = 0;

		inline double CalculateValue() const noexcept
		{
			const double total = m_bestBidPrice + m_bestAskPrice;
			if (total == 0) return 0;
			return (m_bestBidPrice - m_bestAskPrice) / total;
		}
	};

	// MircoPrice calculates the micro price imbalance based on the bid and ask prices and sizes.
	//	Short - term price prediction
	//	    IF	MicroPrice
	//			> MidPrice
	//→ bullish pressure
	//		IF	MicroPrice
	//			< MidPrice
	//→ bearish pressure
	struct DLL_CLASS_QUANTITATIVEMODEL_EXPORTS MircoPrice final
	{
		double m_bidPrice;
		double m_askPrice;
		double m_bidSize;
		double m_askSize;

		inline double CalculateValue() const noexcept
		{
			double total = m_bidSize + m_askSize;
			return (m_askPrice * m_bidSize + m_bidPrice * m_askSize) / total;
		}
	};

};	// namespace QuantitativeModel