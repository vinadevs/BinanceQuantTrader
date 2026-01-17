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
    // This class handles reading market data from files or SQLite databases.
    class DLL_CLASS_HISTORICALDATA_EXPORTS MarketDataFileReader final
    {
    public:

        MarketDataFileReader(const std::string& filePath, DataSourceType sourceType);

        // Read market data into memory (key-value pairs)
        std::vector<std::unordered_map<std::string, std::string>> Read();

    private:
        std::string m_filePath;
        std::ofstream m_fileStream;
        DataSourceType m_sourceType{ DataSourceType::Undefined };

        std::unordered_map<std::string, std::string> ParseLine(const std::string& line);

        // SQLite helper
        void ReadFromSQLite(std::vector<std::unordered_map<std::string, std::string>>& records);
    };

} // namespace MarketDataCapture
