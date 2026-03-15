/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"

#include <vector>

namespace QuantitativeModel {

	// Order book imbalance is a measure of the relative strength
	// of buy and sell orders in the order book.
	//	if (OBI > 0.6)  -> short term buy signal
	//  if (OBI < -0.6) -> short term sell signal
	//	-1  -> all sell orders
	//  0  -> balanced order book
	//  +1  -> all buy orders
	struct IncrementalOrderBookImbalance final
	{
		double m_bidVolume = 0;
		double m_askVolume = 0;

		inline void AddBidQty(double qty)
		{
			m_bidVolume += qty;
		}

		inline void RemoveBidQty(double qty)
		{
			m_bidVolume -= qty;
		}

		inline void AddAskQty(double qty)
		{
			m_askVolume += qty;
		}

		inline void RemoveAskQty(double qty)
		{
			m_askVolume -= qty;
		}

		// Imbalance = \frac{BidVolume - AskVolume}{BidVolume + AskVolume}
		inline double CalculateValue() const noexcept
		{
			const double total = m_bidVolume + m_askVolume;
			if (total == 0) return 0;

			return (m_bidVolume - m_askVolume) / total;
		}
	};

	// LevelPrice represents a price level in the order book, including the price and 
	// the quantity available at that price level.
	struct LevelPrice final
	{
		double price;
		double quantity;
	};

	// CalculateOrderBookImbalance computes the order book imbalance 
	// based on the bid and ask volumes at a specified depth.
	inline double CalculateOrderBookImbalance(
		const std::vector<LevelPrice>& bids,
		const std::vector<LevelPrice>& asks,
		size_t depth) noexcept
	{
		double bidVolume = 0.0;
		double askVolume = 0.0;

		for (size_t i = 0; i < depth && i < bids.size(); ++i)
			bidVolume += bids[i].quantity;

		for (size_t i = 0; i < depth && i < asks.size(); ++i)
			askVolume += asks[i].quantity;

		if (bidVolume + askVolume == 0)
			return 0.0;

		return (bidVolume - askVolume) / (bidVolume + askVolume);
	}
};	// namespace QuantitativeModel