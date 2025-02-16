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

#include <string>

// We can use this SINGLETON class to store exchange simulator rules/settings also as it is singleton...

namespace tinyxml2 {
	class XMLElement;
};

// https://www.binance.com/it/support/faq/binance-futures-fee-structure-fee-calculations-360033544231
// https://www.binance.com/en/support/faq/what-are-market-makers-and-takers-360007720071

// For example, a regular user's maker commission: 0.02%; taker commission: 0.05%
// As for buyer/seller commission fields. They’re usually = 0.

namespace ExchangeSimulator {

	// NOTE: 
	using RateCommission = double;

	class ExchangeRuleAndCompliance
	{
	public:
		ExchangeRuleAndCompliance() = default;
		~ExchangeRuleAndCompliance();

		DISABLE_COPY_AND_MOVE(ExchangeRuleAndCompliance);

		static ExchangeRuleAndCompliance* GetInstance();

		void SetRuleAndCompliance(const tinyxml2::XMLElement* exchangeRuleConfigXml);

		RateCommission GetMakerCommission() const;
		RateCommission GetTakerCommission() const;
		RateCommission GetBuyerCommission() const;
		RateCommission GetSellerCommission() const;
	private:
		RateCommission m_makerCommission{ 0 }; // fee commission from binance
		RateCommission m_takerCommission{ 0 }; // fee commission from binance
		RateCommission m_buyerCommission{ 0 }; // fee commission from buy side
		RateCommission m_sellerCommission{ 0 };// fee commission from sell side
	};
};
// Lets shorten the code line!
#define ExchangeRuleMgr ExchangeSimulator::ExchangeRuleAndCompliance::GetInstance()