/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#ifndef SQLITE_CONNECTION_H
#define SQLITE_CONNECTION_H

#include "dlldefine.h"

#include "../LibraryUtils/MacroUtils.h"

#include <sqlite3.h>
#include <string>
#include <vector>
#include <mutex>
#include <memory>

namespace LibraryUtils {
    class Logger;
};

namespace SqlDatabase {
    class DLL_CLASS_SQLDATABASE_EXPORTS SQLiteConnection {
    private:
        sqlite3* m_db {nullptr};
        std::unique_ptr<LibraryUtils::Logger> m_logger;

        // Private constructor
        SQLiteConnection();
        ~SQLiteConnection();
        // Disable copy constructor and assignment operator
        DISABLE_COPY_AND_MOVE(SQLiteConnection);
    public:
        // Get the singleton instance
        static SQLiteConnection* GetInstance();

        // Open database connection
        bool Open(const std::string& dbFilePath);

        // Close database connection
        void Close();

        // Execute a query without results (e.g., INSERT, UPDATE, DELETE)
        bool ExecuteQuery(const std::string& query);

        // Execute a query and retrieve results (e.g., SELECT)
        std::vector<std::vector<std::string>> ExecuteSelectQuery(const std::string& query);
    };
};

// Lets shorten the code line!
#define SQLiteConnectionPool SqlDatabase::SQLiteConnection::GetInstance()

#endif // SQLITE_CONNECTION_H
