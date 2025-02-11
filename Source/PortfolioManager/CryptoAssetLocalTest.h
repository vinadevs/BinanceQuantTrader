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
#include "Asset.h"

#include <string>

// This class for local test only

namespace PortfolioManager {
	class DLL_CLASS_PORTFOLIOMANAGER_EXPORTS CryptoAssetLocalTest : public Asset
	{
	public:
		CryptoAssetLocalTest(const std::string& symbol,
					const binapi::double_type assetQty,
					const binapi::double_type assetCashBalance);

		void UpdateAssetQty(const binapi::double_type assetQty);
		void UpdateAssetCashBalance(const binapi::double_type assetCashBalance);

		// only call one time, its fixed value and no reset again!
		void SetOriginalAssetCashBalance(const binapi::double_type assetCashBalance);

		const std::string& GetSymbol() const { return m_symbol; }
		const std::string& GetCurrency() const { return m_currency; }
		binapi::double_type GetAssetQty() const { return m_assetQty; }
		binapi::double_type GetAssetCashBalance() const { return m_assetCashBalance; }
		binapi::double_type GetOriginalAssetCashBalance() const { return m_originalAssetCashBalance; }

	private:
		std::string m_symbol; // Ex: Trading pair: BTC/USDT
		std::string m_currency {" (USD)"}; // USD
		binapi::double_type m_assetQty{ 0.0 };
		binapi::double_type m_assetCashBalance{ 0.0 };
		binapi::double_type m_originalAssetCashBalance{ 0.0 };
	};
};