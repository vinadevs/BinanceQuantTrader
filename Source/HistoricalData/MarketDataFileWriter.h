/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"
#include "HistoricalDataDef.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

namespace HistoricalData {
	// This class handles writing market data to files or SQLite databases.
    class DLL_CLASS_HISTORICALDATA_EXPORTS MarketDataFileWriter final
    {
    public:

        MarketDataFileWriter(const std::string& filePath, DataSourceType sourceType);

        // Write market data to file or database
        void Write(const std::string& line);
        void Write(const std::vector<std::unordered_map<std::string, std::string>>& records);

    private:
        std::string m_filePath;
		std::ofstream m_fileStream;
		DataSourceType m_sourceType{ DataSourceType::Undefined };

        // Internal helpers
        std::unordered_map<std::string, std::string> ParseLine(const std::string& line);
        std::string SerializeLine(const std::unordered_map<std::string, std::string>& record);

        // SQLite helper
        void WriteToSQLite(const std::vector<std::unordered_map<std::string, std::string>>& records);
    };

} // namespace MarketDataCapture
