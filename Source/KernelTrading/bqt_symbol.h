/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include <string>

namespace KernelTrading {

	enum class SymbolType : unsigned
	{
		SPOT, // The spot market, where financial instruments are traded for immediate delivery. In the context of Binance, this refers to the trading of cryptocurrencies where the actual asset is bought or sold and delivered immediately.
		FUTURE, // The futures market, where financial contracts obligate the buyer to purchase an asset (or the seller to sell an asset) at a predetermined future date and price. In Binance, this refers to the trading of cryptocurrency futures contracts, which are derivatives that derive their value from the underlying cryptocurrency assets. These contracts allow traders to speculate on the price movements of cryptocurrencies without owning the actual assets.
		OPTION,// The options market, where financial derivatives give the buyer the right, but not the obligation, to buy or sell an asset at a specified price on or before a certain date. In Binance, this refers to the trading of cryptocurrency options contracts, which are derivatives that derive their value from the underlying cryptocurrency assets. These contracts allow traders to speculate on the price movements of cryptocurrencies with limited risk, as the maximum loss is limited to the premium paid for the option.
		UNKNOWN // An unknown symbol type, used as a default value when the symbol type cannot be determined or is not specified. This can help in error handling and debugging by indicating that the symbol type is not recognized.
	};

    // Class BqtSymbol manages all pre and post trade information for a Binance symbol.
    // It stores symbol name, type (SPOT, FUTURE, OPTION, UNKNOWN), total traded volume, and profit/loss (PLN).
    // This class provides methods to set and get these properties, ensuring encapsulation and type safety.

    class BqtSymbol final
	{
	public:
		BqtSymbol() = default;
		BqtSymbol(const std::string& symbol, const SymbolType& symbolType)
			: m_symbol(symbol), m_symbolType(symbolType)
		{
		}

		const std::string& GetSymbol() const noexcept
		{
			return m_symbol;
		}

		const SymbolType& GetSymbolType() const noexcept
		{
			return m_symbolType;
		}

		void SetSymbolType(const SymbolType& symbolType) noexcept
		{
			m_symbolType = symbolType;
		}

		void SetTotalTradedVolume(const double totalTradedVolume) noexcept
		{
			m_totalTradedVolume = totalTradedVolume;
		}

		double GetTotalTradedVolume() const noexcept
		{
			return m_totalTradedVolume;
		}

		void SetPLN(const double pln) noexcept
		{
			m_pln = pln;
		}

		double GetPLN() const noexcept
		{
			return m_pln;
		}

	private:
		std::string m_symbol;
		double m_totalTradedVolume{ 0.0 };
		double m_pln{ 0.0 };
		SymbolType m_symbolType{ SymbolType::UNKNOWN };
	};
}