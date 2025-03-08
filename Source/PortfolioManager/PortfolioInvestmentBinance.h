/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include "../KernelTrading/double_type.h"
#include "../KernelTrading/types.h"

#include "PortfolioInvestment.h"
#include "BinanceTradingPair.h"

#include <string>
#include <vector>
#include <mutex>
#include <unordered_set>

namespace tinyxml2 {
	class XMLElement;
};

namespace MarketData {
	class RealTimeMarketData;
}

namespace PortfolioManager {

	using BinanceBalances = std::map<std::string, BinanceBalance>;
	using BinanceTradingPairMap = std::unordered_map<std::string, std::unique_ptr<BinanceTradingPair>>;
	class DLL_CLASS_PORTFOLIOMANAGER_EXPORTS BinanceTradingPairManager final
	{
	public:
		BinanceTradingPairManager() = default;
		// Thread safe methods
		bool CreateNewTradingPair(const std::string& tradingPairPair, const MarketData::RealTimeMarketData* marketData, const BinanceBalance& balance);
		bool RemoveTradingPair(const std::string& tradingPairPair);
		BinanceTradingPair* GetTradingPair(const std::string& asset);
		const BinanceTradingPairMap& GetTradingPairs() const;
	private:
		std::mutex m_threadSafeMutex;
		BinanceTradingPairMap m_assets;
	};

	////////////////////////////////////////////////////////////////

	// On Binance, portfolio investment refers to managing 
	// a collection of various crypto assets, including tokens,
	// stablecoins, and other cryptocurrencies, to achieve financial
	// goals such as growth, income, or risk mitigation

	class DLL_CLASS_PORTFOLIOMANAGER_EXPORTS
		PortfolioInvestmentBinance : public PortfolioInvestment
	{
	public:
		PortfolioInvestmentBinance(const tinyxml2::XMLElement* portfolioCfg,
								   const MarketData::RealTimeMarketData* marketData);
		~PortfolioInvestmentBinance() override;
		
		void SetUserAccountInfo(binapi::rest::account_info_t* account);

		// NOTE: PLEASE DO NOT CALL UPDATES MANY TIMES/SECONDS
		// AS BINANCE WILL BAN THE LOCAL IP FOR THAT SPAM
		// PLEASE CHECK IN ComplianceNRegulatory CODE
		void UpdateBinanceAccountInfo();
		void UpdateBinanceTradingPairs();
		binapi::rest::account_info_t* GetBinanceAccountInfo();
		BinanceTradingPairManager& GetBinanceTradingPairManager(bool updateNewData = false);
		BinanceTradingPair* GetBinanceTradingPair(const std::string& asset, bool updateNewData = false);
		
		static std::string CreateTradingPairSymbol(const std::string& tartgetSymbol);

	private:
		const BinanceBalances& GetAllBinanceBalances(bool updateNewData = false);
		BinanceBalances GetTradableBinanceBalances(bool updateNewData = false);
		const BinanceBalance& GetBinanceBalance(const std::string& asset, bool updateNewData = false);

		bool IsCryptoAssetAbleToTrade(const BinanceBalance& balance) const;
		bool HasCryptoAssetBalance(const BinanceBalance& balance) const;
		bool IsCryptoAssetHasMarketData(const std::string& asset) const;

		binapi::rest::account_info_t* m_binanceAccountInfo {nullptr};
		BinanceTradingPairManager m_binanceTradingPairMgr;
		const MarketData::RealTimeMarketData* m_marketData{nullptr};
	};
};