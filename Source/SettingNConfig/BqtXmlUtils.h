/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"

#include <memory>
#include <string>
#include <utility>

namespace tinyxml2 {
	class XMLDocument;
	class XMLElement;
};

namespace SettingNConfig {

	namespace BqtXmlUtils {
		// Using to load market data config from Algo/Simulator applications
		std::pair<std::string, std::unique_ptr<tinyxml2::XMLDocument>>
			DLL_CLASS_SETTINGS_AND_CONFIGS_EXPORTS 
			GetMarketDataConfig(const tinyxml2::XMLElement* realTimeMarketDataCfg);
	};
};