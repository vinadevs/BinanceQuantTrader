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
#include "../PortfolioManager/PortfolioInvestmentBinance.h"
#include "../RiskManagement/RiskManager.h"
#include "../OrderManagement/PositionManager.h"
#include "../IndicatorNSignals/TradingHints.h"
#include "../LibraryUtils/Logger.h"
#include "../KernelTrading/double_defaults.h"

#include "OrderParammeterGenerator.h"

using namespace QuantitativeModel;
using namespace IndicatorNSignals;
using namespace ComplianceNRegulatory;

OrderParammeterGenerator::OrderParammeterGenerator(
	const BinanceTradingRules* tradingRules,
	const PortfolioManager::PortfolioInvestmentBinance* portfolio,
	const RiskManagement::RiskManager* riskManager,
	const OrderManagement::PositionManager* positionManager,
	LibraryUtils::Logger* logger)
	: m_tradingRules(tradingRules)
	, m_portfolio(portfolio)
	, m_riskManager(riskManager)
	, m_positionManager(positionManager)
	, m_logger(logger)
{
}

OrderParammeterGenerator::~OrderParammeterGenerator() {}

QuantOrderParammeter OrderParammeterGenerator::Generate(
	const TradingHints* hints)
{
	QuantOrderParammeter orderParammeter;

	// Access the exchange profile for the given symbol from the trading rules
	const auto* symbolProfile 
		= m_tradingRules->GetExchangeProfileMgr()->AccessRemoteExchangeProfile(hints->symbol);
	if (symbolProfile)
	{
		// Retrieve exchange information for the specific symbol
		const auto& symbolExchangeInfo = symbolProfile->get_by_symbol(hints->symbol);

		// Check if the market trend is upward
		if (hints->isUpTrend)
		{
			m_logger->Info("Market's upTrend detected, calculating order parameters...");

			// Set order parameters for a buy order
			orderParammeter.m_symbol = hints->symbol;
			orderParammeter.m_side = binapi::e_side::buy;
			orderParammeter.m_type = binapi::e_type::limit;
			orderParammeter.m_time = hints->timeInForce;

			// Retrieve price filters for the symbol
			const auto& percentPriceBySideFilter = symbolExchangeInfo.get_filter_percent_price_by_side();
			auto weightedAveragePrice 
				= m_positionManager->GetWeightedAveragePrice(hints->symbol, binapi::e_side::buy);
			if (weightedAveragePrice == INVALID_PRICE)
			{
				// If the weighted average price is invalid, use the window best ask price
				weightedAveragePrice = hints->windowBestAskPrice;
			}
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
				orderParammeter.m_amount = notationFilter.maxNotional.convert_to<double>() / orderParammeter.m_price;
			}
			else if (notationValue < notationFilter.minNotional)
			{
				orderParammeter.m_amount = notationFilter.minNotional.convert_to<double>() / orderParammeter.m_price;
			}
		}
		// Check if the market trend is downward
		else if (hints->isDownTrend)
		{
			m_logger->Info("Market's downTrend detected, calculating order parameters...");

			// Set order parameters for a sell order
			orderParammeter.m_symbol = hints->symbol;
			orderParammeter.m_side = binapi::e_side::sell;
			orderParammeter.m_type = binapi::e_type::limit;
			orderParammeter.m_time = hints->timeInForce;

			// Retrieve price filters for the symbol
			const auto& percentPriceBySideFilter = symbolExchangeInfo.get_filter_percent_price_by_side();
			auto weightedAveragePrice 
				= m_positionManager->GetWeightedAveragePrice(hints->symbol, binapi::e_side::sell);
			if (weightedAveragePrice == INVALID_PRICE)
			{
				// If the weighted average price is invalid, use the window best bid price
				weightedAveragePrice = hints->windowBestBidPrice;
			}
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
				orderParammeter.m_amount = notationFilter.maxNotional.convert_to<double>() / orderParammeter.m_price;
			}
			else if (notationValue < notationFilter.minNotional)
			{
				orderParammeter.m_amount = notationFilter.minNotional.convert_to<double>() / orderParammeter.m_price;
			}
		}
		else
		{
			// Throw an exception if the exchange profile for the symbol cannot be found
			throw std::runtime_error("invalid trend, could not generate order parameters for symbol=" + hints->symbol);
		}
	}
	else
	{
		// Throw an exception if the exchange profile for the symbol cannot be found
		throw std::runtime_error("could not lookup Exchange Profile for symbol=" + hints->symbol);
	}

	// Log and return the generated order parameters
	m_logger->Info(orderParammeter.AsString());
	return orderParammeter;
}

