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

namespace ExchangeSimulator {
	class ExchangeRuleAndCompliance
	{
	public:
		ExchangeRuleAndCompliance() = default;
		~ExchangeRuleAndCompliance();

		DISABLE_COPY_AND_MOVE(ExchangeRuleAndCompliance);

		static ExchangeRuleAndCompliance* GetInstance();

		void SetRuleAndCompliance(const tinyxml2::XMLElement* exchangeRuleConfigXml);

		std::size_t GetMakerCommission() const;
		std::size_t GetTakerCommission() const;
		std::size_t GetBuyerCommission() const;
		std::size_t GetSellerCommission() const;
	private:
		std::size_t m_makerCommission{ 0 }; // fee commission from binance
		std::size_t m_takerCommission{ 0 }; // fee commission from binance
		std::size_t m_buyerCommission{ 0 }; // fee commission from buy side
		std::size_t m_sellerCommission{ 0 };// fee commission from sell side
	};
};
// Lets shorten the code line!
#define ExchangeRuleMgr ExchangeSimulator::ExchangeRuleAndCompliance::GetInstance()