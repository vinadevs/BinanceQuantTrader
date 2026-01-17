/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "MarketDataFileWriter.h"
#include <sqlite3.h> // Requires SQLite3 library

namespace HistoricalData {
    MarketDataFileWriter::MarketDataFileWriter(const std::string& filePath, DataSourceType sourceType)
        : m_filePath(filePath), m_sourceType(sourceType)
    {
		m_fileStream.open(m_filePath, std::ios::out | std::ios::app);
        if (!m_fileStream.is_open()) {
            throw std::runtime_error("Cannot open file: " + m_filePath);
        }
    }

    // ======================= PUBLIC =======================

    void MarketDataFileWriter::Write(const std::string& line)
    {
        if (m_sourceType == DataSourceType::TextFile)
        {
            m_fileStream << line << "\n";
        }
    }

    void MarketDataFileWriter::Write(const std::vector<std::unordered_map<std::string, std::string>>& records)
    {
        if (m_sourceType == DataSourceType::SQLite)
        {
            WriteToSQLite(records);
        }
    }

    // ======================= HELPERS =======================

    std::unordered_map<std::string, std::string> MarketDataFileWriter::ParseLine(const std::string& line)
    {
        std::unordered_map<std::string, std::string> result;
        std::stringstream ss(line);
        std::string token;

        while (std::getline(ss, token, ','))
        {
            auto pos = token.find('=');
            if (pos != std::string::npos)
            {
                std::string key = token.substr(0, pos);
                std::string value = token.substr(pos + 1);
                result[key] = value;
            }
        }
        return result;
    }

    std::string MarketDataFileWriter::SerializeLine(const std::unordered_map<std::string, std::string>& record)
    {
        std::ostringstream oss;
        bool first = true;
        for (const auto& [key, value] : record)
        {
            if (!first)
                oss << ",";
            first = false;
            oss << key << "=" << value;
        }
        return oss.str();
    }

    // ======================= SQLITE IMPLEMENTATION =======================

    void MarketDataFileWriter::WriteToSQLite(const std::vector<std::unordered_map<std::string, std::string>>& records)
    {
        sqlite3* db = nullptr;
        if (sqlite3_open(m_filePath.c_str(), &db) != SQLITE_OK)
            throw std::runtime_error("Cannot open SQLite DB: " + m_filePath);

        const char* createTableSQL =
            "CREATE TABLE IF NOT EXISTS MarketData (Key TEXT, Value TEXT);";
        sqlite3_exec(db, createTableSQL, nullptr, nullptr, nullptr);

        const char* deleteSQL = "DELETE FROM MarketData;";
        sqlite3_exec(db, deleteSQL, nullptr, nullptr, nullptr);

        sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

        const char* insertSQL = "INSERT INTO MarketData (Key, Value) VALUES (?, ?);";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr);

        for (const auto& record : records)
        {
            for (const auto& [key, value] : record)
            {
                sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmt, 2, value.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_step(stmt);
                sqlite3_reset(stmt);
            }
        }

        sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }
} // namespace MarketDataCapture
