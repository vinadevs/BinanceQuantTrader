/********************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"
#include "QuantOrderParammeter.h"

namespace QuantitativeModel {

	/**
	 * @brief Pre-trade validation & normalization engine
	 *
	 * Ensures all QuantOrderParammeter orders comply with exchange rules
	 *  - Price filter
	 *  - Lot size filter
	 *  - Min notional filter
	 */
	class DLL_CLASS_QUANTITATIVEMODEL_EXPORTS TradingRulesValidationEngine final
	{
	public:
		TradingRulesValidationEngine() = default;
		~TradingRulesValidationEngine() = default;

		/**
		 * @brief Price filter constraints
		 */
		struct PriceFilter final
		{
			double MinPrice;
			double MaxPrice;
			double TickSize;
		};

		/**
		 * @brief Quantity (lot size) constraints
		 */
		struct LotSizeFilter final	
		{
			double MinQty;
			double MaxQty;
			double StepSize;
		};

		/**
		 * @brief Notional constraint (price * quantity)
		 */
		struct MinNotionalFilter final
		{
			double Value;
		};

		/**
		 * @brief Aggregated symbol rules
		 */
		struct SymbolRules final
		{
			PriceFilter		  Price;
			LotSizeFilter	  LotSize;
			MinNotionalFilter MinNotional;
		};

	public:
		/**
		 * @brief Validate order against all rules
		 */
		bool Validate(
				const QuantOrderParammeter& order,
				const SymbolRules& rules) const noexcept;

		/**
		 * @brief Normalize order to nearest valid values
		 */
		void Normalize(
				QuantOrderParammeter& order,
				const SymbolRules&	  rules) const noexcept;

	private:
		// Individual validation functions for each rule
		bool ValidatePrice(
				const QuantOrderParammeter& order,
				const SymbolRules&			rules) const noexcept;

		// Validate quantity against lot size rules
		bool ValidateQuantity(
				const QuantOrderParammeter& order,
				const SymbolRules&			rules) const noexcept;

		// Validate notional value against min notional rules
		bool ValidateNotional(
				const QuantOrderParammeter& order,
				const SymbolRules& rules) const noexcept;

		// Normalization functions to adjust price and quantity to valid values
		double NormalizePrice(
				double			   price,
				const PriceFilter& filter) const noexcept;

		// Normalize quantity to nearest valid step size and within min/max limits
		double NormalizeQuantity(
				double				 qty,
				const LotSizeFilter& filter) const noexcept;
	};
};
