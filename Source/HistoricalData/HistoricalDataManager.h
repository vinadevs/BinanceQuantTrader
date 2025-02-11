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

/**
 * @class HistoricalDataManager
 * @brief Manages the retrieval, storage, and processing of historical trading data.
 *
 * This class is responsible for interacting with a database or file system
 * to store and retrieve historical trading data, such as price, volume,
 * and other market metrics. It provides methods for querying data
 * by various criteria (e.g., time range, symbol) and supports preprocessing
 * steps like aggregation or data normalization for analytical purposes.
 *
 * Key Responsibilities:
 * - Load and store historical trading data from/to a persistent source.
 * - Provide efficient query capabilities for trading algorithms and analysis tools.
 * - Perform preprocessing and transformations on the data.
 *
*/

// We can use this SINGLETON class to store global settings also as it is singleton...

namespace tinyxml2 {
	class XMLElement;
};

namespace HistoricalData {
	class DLL_CLASS_HISTORICALDATA_EXPORTS HistoricalDataManager final
	{
	public:
		HistoricalDataManager() = default;
		~HistoricalDataManager();

		DISABLE_COPY_AND_MOVE(HistoricalDataManager);

		static HistoricalDataManager* GetInstance();

		void LoadHistoricalDatabase(const tinyxml2::XMLElement* staticDataConfigXml);
	};
};
// Lets shorten the code line!
#define HistoricalDataMgr HistoricalData::HistoricalDataManager::GetInstance()