/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <mutex>
#include <iomanip>
#include <thread>
#include <memory>
#include <sstream>

#include "dlldefine.h"

#pragma warning(disable : 4251)

namespace LibraryUtils {
class DLL_CLASS_LIBRARYUTILS_EXPORTS Logger
{
public:
    enum class LogLevel : unsigned
    {
        DEBUG,
        INFO,
        WARNING,
        ERR,
        EXCEPTION,
    };

    Logger(const std::string& modulename, const std::string& filename = "");
    ~Logger();

    // Disable copy and move
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    void Debug(const std::string& message);
    void Info(const std::string& message);
    void Warning(const std::string& message);
    void Error(const std::string& message);
    void Exception(const std::string& message);

private:
    std::ofstream m_fileStream;
    bool m_logToFile{ false };
    std::mutex m_mutex;
    std::string m_moduleName;

    void Log(const LogLevel level, const std::string& message);
    std::string GetThreadAddress() const;
    std::string GetModuleName() const;
    std::string LogLevelToString(LogLevel level) const;
};

// Macro to log stream <<
#define LOG_DEBUG_STREAM(logger, ...) (logger)->Debug((std::ostringstream() << __VA_ARGS__).str())
#define LOG_INFO_STREAM(logger, ...) (logger)->Info((std::ostringstream() << __VA_ARGS__).str())
#define LOG_WARNING_STREAM(logger, ...) (logger)->Warning((std::ostringstream() << __VA_ARGS__).str())
#define LOG_ERROR_STREAM(logger, ...) (logger)->Error((std::ostringstream() << __VA_ARGS__).str())
#define LOG_EXCEPTION_STREAM(logger, ...) (logger)->Exception((std::ostringstream() << __VA_ARGS__).str())

};
