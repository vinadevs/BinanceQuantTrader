/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"

#include "../LibraryUtils/MacroUtils.h"
#include "../CurlAPI/BinanceMarginRateInfo.h"

#include <string>
#include <unordered_map>

// We can use this SINGLETON class to store exchange simulator rules/settings also as it is singleton...

namespace tinyxml2 {
	class XMLElement;
};

// https://www.binance.com/it/support/faq/binance-futures-fee-structure-fee-calculations-360033544231
// https://www.binance.com/en/support/faq/what-are-market-makers-and-takers-360007720071

// For example, a regular user's maker commission: 0.02%; taker commission: 0.05%
// As for buyer/seller commission fields. They’re usually = 0.

namespace ExchangeSimulator {

	using RateCommission = double;

	class ExchangeRuleAndCompliance
	{
	public:
		ExchangeRuleAndCompliance() = default;
		~ExchangeRuleAndCompliance();

		DISABLE_COPY_AND_MOVE(ExchangeRuleAndCompliance);

		static ExchangeRuleAndCompliance* GetInstance();

		void SetRuleAndCompliance(const tinyxml2::XMLElement* exchangeRuleConfigXml);

		/**
		 * @brief Get the spot maker/taker/buyer/seller commission rates.
		 * @return The commission rates as RateCommission.
		 */
		RateCommission GetMakerCommission() const;
		RateCommission GetTakerCommission() const;
		RateCommission GetBuyerCommission() const;
		RateCommission GetSellerCommission() const;
		/**
		 * @brief Get the future maker/taker commission rates.
		 * @return The future commission rates as RateCommission.
		 */
		RateCommission GetFutureMakerCommission() const;
		RateCommission GetFutureTakerCommission() const;
		/**
		 * @brief Get the margin rate info for a specific symbol.
		 * @param symbol The trading symbol (e.g., "BTCUSDT").
		 * @return The SymbolMarginRateInfo for the given symbol.
		 */
		const CurlAPI::SymbolMarginRateInfo GetFutureMarginRateInfo(const std::string& symbol) const;
		/**
		 * @brief Get the leverage bracket for a specific symbol and position notional.
		 * @param symbol The trading symbol (e.g., "BTCUSDT").
		 * @param postitionNotional The notional value of the position.
		 * @return The LeverageBracket for the given symbol and notional.
		 */
		const CurlAPI::LeverageBracket& GetFutureLeverageBracketByNotional(const std::string& symbol, const double postitionNotional);

		/**
		 * @brief Get the leverage bracket for a specific symbol and tier.
		 * @param symbol The trading symbol (e.g., "BTCUSDT").
		 * @param tier The leverage tier.
		 * @return The LeverageBracket for the given symbol and tier.
		 */
		const CurlAPI::LeverageBracket& GetFutureLeverageBracketByTier(const std::string& symbol, const double tier);

	private:
		/**
		 * @brief Load leverage brackets from a JSON file.
		 * @param filename Path to the JSON file.
		 * @return A map from symbol to its SymbolMarginRateInfo.
		 */
		void LoadLeverageBracketsFromFile(const std::string& filename);

		RateCommission m_makerCommission{ 0 }; // fee commission from binance
		RateCommission m_takerCommission{ 0 }; // fee commission from binance
		RateCommission m_buyerCommission{ 0 }; // fee commission from buy side
		RateCommission m_sellerCommission{ 0 };// fee commission from sell side
		RateCommission m_futureMakerCommission{ 0 }; // fee commission from binance for future market
		RateCommission m_futureTakerCommission{ 0 }; // fee commission from binance for future market
		std::unordered_map<std::string, CurlAPI::SymbolMarginRateInfo> m_symbolMarginRateInfos; // symbol to maintenance margin rate map
	};
};
// Lets shorten the code line!
#define ExchangeRuleMgr ExchangeSimulator::ExchangeRuleAndCompliance::GetInstance()