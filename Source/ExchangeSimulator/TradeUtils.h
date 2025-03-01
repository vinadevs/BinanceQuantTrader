#pragma once

namespace ExchangeSimulator {

	namespace Finance {
		inline double CalculateTradeValue(
			const double quality,
			const double refPrice)
		{
			return quality * refPrice;
		}
	};
};