/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "MarketDataFileReader.h"
#include <sqlite3.h> // Requires SQLite3 library

namespace HistoricalData {
    MarketDataFileReader::MarketDataFileReader(const std::string& filePath, DataSourceType sourceType)
        : m_filePath(filePath), m_sourceType(sourceType)
    {
        m_fileStream.open(m_filePath, std::ios::out | std::ios::app);
        if (!m_fileStream.is_open()) {
            throw std::runtime_error("Cannot open file: " + m_filePath);
        }
    }

    // ======================= PUBLIC =======================

    std::vector<std::unordered_map<std::string, std::string>> MarketDataFileReader::Read()
    {
        std::vector<std::unordered_map<std::string, std::string>> records;

        if (m_sourceType == DataSourceType::TextFile)
        {
            std::ifstream file(m_filePath);
            if (!file.is_open())
                throw std::runtime_error("Cannot open file: " + m_filePath);

            std::string line;
            while (std::getline(file, line))
            {
                if (!line.empty())
                    records.push_back(ParseLine(line));
            }
        }
        else if (m_sourceType == DataSourceType::SQLite)
        {
            ReadFromSQLite(records);
        }

        return records;
    }

    // ======================= HELPERS =======================

    std::unordered_map<std::string, std::string>
        MarketDataFileReader::ParseLine(const std::string& line)
    {
        std::unordered_map<std::string, std::string> result;
        result.reserve(16); // reduce rehashing

        std::size_t start = 0;

        while (start < line.size())
        {
            auto sep = line.find('|', start);
            if (sep == std::string::npos)
                sep = line.size();

            auto eq = line.find('=', start);
            if (eq != std::string::npos && eq < sep)
            {
                result.emplace(
                    line.substr(start, eq - start),
                    line.substr(eq + 1, sep - eq - 1));
            }

            start = sep + 1;
        }

        return result;
    }

    // ======================= SQLITE IMPLEMENTATION =======================

    void MarketDataFileReader::ReadFromSQLite(std::vector<std::unordered_map<std::string, std::string>>& records)
    {
        sqlite3* db = nullptr;
        if (sqlite3_open(m_filePath.c_str(), &db) != SQLITE_OK)
            throw std::runtime_error("Cannot open SQLite DB: " + m_filePath);

        const char* sql = "SELECT * FROM MarketData;";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
            throw std::runtime_error("Failed to prepare query.");

        int colCount = sqlite3_column_count(stmt);
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            std::unordered_map<std::string, std::string> row;
            for (int i = 0; i < colCount; ++i)
            {
                const char* colName = sqlite3_column_name(stmt, i);
                const unsigned char* colText = sqlite3_column_text(stmt, i);
                row[colName] = colText ? reinterpret_cast<const char*>(colText) : "";
            }
            records.push_back(std::move(row));
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }
} // namespace MarketDataCapture
