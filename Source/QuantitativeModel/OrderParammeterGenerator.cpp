/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "../ComplianceNRegulatory/BinanceTradingRules.h"
#include "../ComplianceNRegulatory/BinanceExchangeProfile.h"
#include "../IndicatorNSignals/TradingHints.h"
#include "../LibraryUtils/Logger.h"

#include "OrderParammeterGenerator.h"

using namespace QuantitativeModel;
using namespace IndicatorNSignals;
using namespace ComplianceNRegulatory;

OrderParammeterGenerator::OrderParammeterGenerator(
	const BinanceTradingRules* tradingRules,
	LibraryUtils::Logger* logger)
	: m_tradingRules(tradingRules)
	, m_logger(logger)
{
}

OrderParammeterGenerator::~OrderParammeterGenerator() {}

QuantOrderParammeter OrderParammeterGenerator::Generate(
	const TradingHints* hints)
{
	QuantOrderParammeter orderParammeter;

	// Access the exchange profile for the given symbol from the trading rules
	const auto* symbolProfile = m_tradingRules->GetExchangeProfileMgr()->AccessRemoteExchangeProfile(hints->symbol);
	if (symbolProfile)
	{
		// Retrieve exchange information for the specific symbol
		const auto& symbolExchangeInfo = symbolProfile->get_by_symbol(hints->symbol);

		// Check if the market trend is upward
		if (hints->isUpTrend)
		{
			// Set order parameters for a buy order
			orderParammeter.m_symbol = hints->symbol;
			orderParammeter.m_side = binapi::e_side::buy;
			orderParammeter.m_type = binapi::e_type::limit;
			orderParammeter.m_time = binapi::e_time::GTC;

			// Retrieve price filters for the symbol
			const auto& percentPriceBySideFilter = symbolExchangeInfo.get_filter_percent_price_by_side();
			const auto weightedAveragePrice = hints->windowBestAskPrice;
			const auto limitPrice = weightedAveragePrice;

			// Adjust the price based on the ask price multiplier filters
			if (limitPrice > weightedAveragePrice * percentPriceBySideFilter.askMultiplierUp)
			{
				orderParammeter.m_price = (weightedAveragePrice * percentPriceBySideFilter.askMultiplierUp).convert_to<double>();
			}
			else if (limitPrice < weightedAveragePrice * percentPriceBySideFilter.askMultiplierDown)
			{
				orderParammeter.m_price = (weightedAveragePrice * percentPriceBySideFilter.askMultiplierDown).convert_to<double>();
			}
			else
			{
				orderParammeter.m_price = limitPrice.convert_to<double>();
			}

			// Set the order amount based on the lot size filter
			const auto& lotSizeFilter = symbolExchangeInfo.get_filter_lot_size();
			orderParammeter.m_amount = lotSizeFilter.minQty.convert_to<double>() * 10;

			// Validate the order's notional value against the notional filter
			const auto& notationFilter = symbolExchangeInfo.get_filter_notional();
			const double notationValue = orderParammeter.m_price * orderParammeter.m_amount;
			if (notationValue > notationFilter.maxNotional)
			{
				orderParammeter.m_price = notationFilter.maxNotional.convert_to<double>() / orderParammeter.m_amount;
			}
			else if (notationValue < notationFilter.minNotional)
			{
				orderParammeter.m_price = notationFilter.minNotional.convert_to<double>() / orderParammeter.m_amount;
			}
		}
		// Check if the market trend is downward
		else if (hints->isDownTrend)
		{
			// Set order parameters for a sell order
			orderParammeter.m_symbol = hints->symbol;
			orderParammeter.m_side = binapi::e_side::sell;
			orderParammeter.m_type = binapi::e_type::limit;
			orderParammeter.m_time = binapi::e_time::GTC;

			// Retrieve price filters for the symbol
			const auto& percentPriceBySideFilter = symbolExchangeInfo.get_filter_percent_price_by_side();
			const auto weightedAveragePrice = hints->windowBestAskPrice;
			const auto limitPrice = weightedAveragePrice;

			// Adjust the price based on the bid price multiplier filters
			if (limitPrice > weightedAveragePrice * percentPriceBySideFilter.bidMultiplierUp)
			{
				orderParammeter.m_price = (weightedAveragePrice * percentPriceBySideFilter.bidMultiplierUp).convert_to<double>();
			}
			else if (limitPrice < weightedAveragePrice * percentPriceBySideFilter.bidMultiplierDown)
			{
				orderParammeter.m_price = (weightedAveragePrice * percentPriceBySideFilter.bidMultiplierDown).convert_to<double>();
			}
			else
			{
				orderParammeter.m_price = limitPrice.convert_to<double>();
			}

			// Set the order amount based on the lot size filter
			const auto& lotSizeFilter = symbolExchangeInfo.get_filter_lot_size();
			orderParammeter.m_amount = lotSizeFilter.minQty.convert_to<double>() * 10;

			// Validate the order's notional value against the notional filter
			const auto& notationFilter = symbolExchangeInfo.get_filter_notional();
			const double notationValue = orderParammeter.m_price * orderParammeter.m_amount;
			if (notationValue > notationFilter.maxNotional)
			{
				orderParammeter.m_price = notationFilter.maxNotional.convert_to<double>() / orderParammeter.m_amount;
			}
			else if (notationValue < notationFilter.minNotional)
			{
				orderParammeter.m_price = notationFilter.minNotional.convert_to<double>() / orderParammeter.m_amount;
			}
		}
	}
	else
	{
		// Throw an exception if the exchange profile for the symbol cannot be found
		throw std::runtime_error("could not lookup Exchange Profile for symbol=" + hints->symbol);
	}

	// Return the generated order parameters
	return orderParammeter;
}

