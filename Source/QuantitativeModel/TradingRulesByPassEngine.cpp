/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "TradingRulesByPassEngine.h"

#include <cmath>

using namespace QuantitativeModel;

bool TradingRulesValidationEngine::Validate(
		const QuantOrderParammeter& order,
		const SymbolRules&			rules) const noexcept
{
	return ValidatePrice(order, rules) && ValidateQuantity(order, rules) && ValidateNotional(order, rules);
}

void TradingRulesValidationEngine::Normalize(
		QuantOrderParammeter& order,
		const SymbolRules&	  rules) const noexcept
{
	order.m_price = NormalizePrice(order.m_price, rules.Price);
	order.m_amount = NormalizeQuantity(order.m_amount, rules.LotSize);
}

bool TradingRulesValidationEngine::ValidatePrice(
		const QuantOrderParammeter& order,
		const SymbolRules&			rules) const noexcept
{
	const auto& f = rules.Price;

	if (order.m_price < f.MinPrice) return false;
	if (order.m_price > f.MaxPrice) return false;

	const double steps = order.m_price / f.TickSize;
	return std::fabs(steps - std::round(steps)) < 1e-9;
}

bool TradingRulesValidationEngine::ValidateQuantity(
		const QuantOrderParammeter& order,
		const SymbolRules&			rules) const noexcept
{
	const auto& f = rules.LotSize;

	if (order.m_amount < f.MinQty) return false;
	if (order.m_amount > f.MaxQty) return false;

	const double steps = order.m_amount / f.StepSize;
	return std::fabs(steps - std::round(steps)) < 1e-9;
}

bool TradingRulesValidationEngine::ValidateNotional(
		const QuantOrderParammeter& order,
		const SymbolRules& rules) const noexcept
{
	return (order.m_price * order.m_amount) >= rules.MinNotional.Value;
}

double TradingRulesValidationEngine::NormalizePrice(
		double			   price,
		const PriceFilter& filter) const noexcept
{
	if (price < filter.MinPrice) return filter.MinPrice;
	if (price > filter.MaxPrice) return filter.MaxPrice;

	const double steps = std::floor(price / filter.TickSize);
	return steps * filter.TickSize;
}

double TradingRulesValidationEngine::NormalizeQuantity(
		double				 qty,
		const LotSizeFilter& filter) const noexcept
{
	if (qty < filter.MinQty) return filter.MinQty;
	if (qty > filter.MaxQty) return filter.MaxQty;

	const double steps = std::floor(qty / filter.StepSize);
	return steps * filter.StepSize;
}
