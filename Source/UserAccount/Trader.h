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
#include "../LibraryUtils/Logger.h"

#include <string>
#include <memory>

namespace tinyxml2 {
	class XMLElement;
};

namespace UserAccount {

	// -In the context of financial markets, a trader is an individual 
	// or entity that buys and sells financial instruments,
	// such as stocks, bonds, commodities, currencies, or derivatives,
	// with the goal of making a profit.

	// This class will manage trade activities like buy/sell/report...
	class DLL_CLASS_USERACCOUNT_EXPORTS Trader
	{
	public:
		Trader() {};
		virtual ~Trader();

		virtual bool Buy(const std::string& symbol,
			const binapi::double_type quality,
			const binapi::double_type refPrice) = 0;

		virtual bool Sell(const std::string& symbol,
			const binapi::double_type quality,
			const binapi::double_type refPrice) = 0;

		virtual void ReportTradeData(const std::string& symbol) = 0;
	protected:
		virtual void SetupReporter(const tinyxml2::XMLElement* reportCfg) = 0;
		std::unique_ptr<LibraryUtils::Logger> m_logger;
	};
};