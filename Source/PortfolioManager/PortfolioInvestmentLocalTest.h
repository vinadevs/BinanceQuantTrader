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
#include "CryptoAssetLocalTest.h"

#include <string>
#include <vector>

namespace tinyxml2 {
	class XMLElement;
};

// This class for local test only

namespace PortfolioManager {
	class DLL_CLASS_PORTFOLIOMANAGER_EXPORTS
		PortfolioInvestmentLocalTest : public PortfolioInvestment
	{
	public:
		PortfolioInvestmentLocalTest(const tinyxml2::XMLElement* portfolioCfg);
		~PortfolioInvestmentLocalTest() override;

		void UpdateInvestmentAsset(const std::string& symbol,
								   const binapi::double_type newAssetQty,
								   const binapi::double_type newAssetCashBalance);

		void RemoveInvestmentAsset(const std::string& symbol);

		bool IsAlreadyHold(const std::string& symbol);

		const CryptoAssetLocalTest&  GetCryptoAsset(const std::string& symbol);

		const std::vector<CryptoAssetLocalTest>& GetHoldingAssetList() const;

		// For Binance Account
		bool UpdateBinanceAssets(const binapi::rest::account_info_t& accountInfo);

	private:
		void AddInvestmentAsset(const std::string& symbol,
			const binapi::double_type assetQty,
			const binapi::double_type assetCashBalance,
			const binapi::double_type originalAssetCashBalance);

		void LoadInvestmentAssets(const char* filePath);

		std::vector<CryptoAssetLocalTest> m_cryptoAssets;
	};
};