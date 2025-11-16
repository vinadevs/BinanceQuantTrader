#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

namespace MarketDataCapture
{
	// This class handles reading and writing market data to/from files or SQLite databases.
    class MarketDataFileWriter
    {
    public:
        // Enum for data source type
        enum class DataSourceType : unsigned
        {
            TextFile,
            SQLite
        };

        explicit MarketDataFileWriter(const std::string& filePath, DataSourceType sourceType);

        // Read market data into memory (key-value pairs)
        std::vector<std::unordered_map<std::string, std::string>> Read();

        // Write market data to file or database
        void Write(const std::string& line);
        void Write(const std::vector<std::unordered_map<std::string, std::string>>& records);

    private:
        std::string m_filePath;
        DataSourceType m_sourceType;

        // Internal helpers
        std::unordered_map<std::string, std::string> ParseLine(const std::string& line);
        std::string SerializeLine(const std::unordered_map<std::string, std::string>& record);

        // SQLite helpers (if needed)
        void ReadFromSQLite(std::vector<std::unordered_map<std::string, std::string>>& records);
        void WriteToSQLite(const std::vector<std::unordered_map<std::string, std::string>>& records);
    };

} // namespace MarketDataCapture
