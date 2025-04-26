/********************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be useddistributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"

#include <string>
#include <unordered_map>
#include <memory>

#include "QuantMarketDataAnalyzer.h"

namespace LibraryUtils {
	class Logger;
}

namespace QuantitativeModel {

	class DLL_CLASS_QUANTITATIVEMODEL_EXPORTS MarketDataAnalyzer final {
	public:
		MarketDataAnalyzer(LibraryUtils::Logger* logger);
		~MarketDataAnalyzer();
		QuantMarketDataAnalyzer* GetQuantMarketDataAnalyzer(const std::string& symbol);
	private:
		std::unordered_map<std::string, std::shared_ptr<QuantMarketDataAnalyzer>> m_quantMarketDataAnalyzers;
		LibraryUtils::Logger* m_logger{ nullptr };
	};
};
