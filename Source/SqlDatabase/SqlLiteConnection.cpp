/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "SqlLiteConnection.h"
#include "../LibraryUtils/Logger.h"

using namespace SqlDatabase;

SQLiteConnection::SQLiteConnection() : 
    m_logger{ std::make_unique<LibraryUtils::Logger>("SQLiteConnection") } {}

SQLiteConnection* SQLiteConnection::GetInstance() {
    static SQLiteConnection instance;
    return &instance;
}

bool SQLiteConnection::Open(const std::string& dbName) {
    if (m_db) {
        LOG_ERROR_STREAM(m_logger, "Database already opened.");
        return false;
    }

    int rc = sqlite3_open(dbName.c_str(), &m_db);
    if (rc != SQLITE_OK) {
        LOG_ERROR_STREAM(m_logger, "Can't open database: " << sqlite3_errmsg(m_db));
        return false;
    }
    return true;
}

void SQLiteConnection::Close() {
    if (m_db) {
        sqlite3_close(m_db);
        m_db = nullptr;
    }
}

bool SQLiteConnection::ExecuteQuery(const std::string& query) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db, query.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        LOG_ERROR_STREAM(m_logger, "SQL error: " << errMsg);
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

std::vector<std::vector<std::string>> SQLiteConnection::ExecuteSelectQuery(const std::string& query) {
    std::vector<std::vector<std::string>> results;
    char* errMsg = nullptr;
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        LOG_ERROR_STREAM(m_logger, "Failed to prepare statement: " << sqlite3_errmsg(m_db));
        return results;
    }

    int numColumns = sqlite3_column_count(stmt);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::vector<std::string> row;
        for (int i = 0; i < numColumns; i++) {
            const char* value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
            row.push_back(value ? value : "NULL");
        }
        results.push_back(row);
    }

    if (rc != SQLITE_DONE) {
        LOG_ERROR_STREAM(m_logger, "Error during execution: " << sqlite3_errmsg(m_db));
    }

    sqlite3_finalize(stmt);
    return results;
}

SQLiteConnection::~SQLiteConnection() {
    Close();
}
