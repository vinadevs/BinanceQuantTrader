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
#include "../LibraryUtils/Logger.h"

#include <string>
#include <vector>
#include <memory>

// In trading systems, StaticData refers to the non-transactional,
// unchanging information that is essential for the operation of the 
// trading platform. This data typically provides the foundational
// information required for various trading activities but does not
// frequently change in the short term.

// We can use this SINGLETON class to store global settings also as it is singleton...

namespace tinyxml2 {
	class XMLElement;
};

namespace LibraryUtils {
	class Logger;
};

namespace StaticData {
	class DLL_CLASS_TRADING_STATICDATA_EXPORTS StaticDataManager
	{
	public:
		StaticDataManager() = default;
		~StaticDataManager();

		DISABLE_COPY_AND_MOVE(StaticDataManager);

		static StaticDataManager* GetInstance();

		// Get BTC from BTCUSDT
		static std::string GetSymbolFromTradingPair(
			const std::string& input,
			const std::string& stableCoin) {
			size_t pos = input.find(stableCoin); // Find "USDT" in the string
			return (pos != std::string::npos) ? input.substr(0, pos) : input;
		}

		void LoadStaticDatabase(const tinyxml2::XMLElement* staticDataConfigXml);

		std::vector<std::string> GetAllRemoteListingSymbols(const bool logDataToFile);

		//USDT(Tether) is a type of stablecoin,
		//which is a cryptocurrency designed to maintain a
		//stable value by being pegged to a reserve asset.
		//In the case of USDT, its value is pegged 1:1 to the US dollar.
		//Tether(USDT) was created by Tether Limited in 2014,
		//and it is widely used in the cryptocurrency ecosystem
		//for trading, payments, and as a store of value in
		//volatile markets.
		const std::string& GetStableCoinUSDTSymbol() const;
	private:
		std::string m_stablecoinUSDT;
		std::unique_ptr<LibraryUtils::Logger> m_logger
		{ std::make_unique<LibraryUtils::Logger>("StaticDataManager") };
	};
};
// Lets shorten the code line!
#define StaticDataMgr StaticData::StaticDataManager::GetInstance()