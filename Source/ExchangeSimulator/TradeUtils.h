#pragma once

namespace ExchangeSimulator {

	namespace Finance {
		inline double CalculateTradeValue(
			const double quality,
			const double refPrice)
		{
			return quality * refPrice;
		}

		inline bool IsLeverageRatioValid(const double leverageRatio) {
			return leverageRatio > 0.0 && leverageRatio <= 125.0; // Binance max leverage is 125x
		}
	};
};