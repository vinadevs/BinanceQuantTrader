/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "Logger.h"
#include "TimeUtils.h"

using namespace LibraryUtils;

Logger::Logger(const std::string& modulename, const std::string& filename)
    : m_moduleName(modulename), m_fileStream(), m_logToFile(!filename.empty())
{
    if (m_logToFile)
    {
        m_fileStream.open(filename, std::ios::app);
        if (!m_fileStream.is_open())
        {
            std::cerr << "LibraryUtils::Logger: could not open log file: " << filename << std::endl;
            m_logToFile = false; // Fallback to console if file cannot be opened
        }
    }
}

Logger::~Logger()
{
    if (m_fileStream.is_open())
    {
        m_fileStream.close();
    }
}

void Logger::Log(const LogLevel level, const std::string& message)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    const std::string timestamp = TimeUtils::GetCurrentTimestampString();
    const std::string threadAddress = GetThreadAddress();
    const std::string moduleName = GetModuleName();
    const std::string levelStr = LogLevelToString(level);
    const std::string fullMessage = timestamp + " " +
        threadAddress + " " + moduleName + " " + levelStr + " " + message;
    if (m_logToFile && m_fileStream.is_open())
    {
        m_fileStream << fullMessage << std::endl;
    }
    else
    {
        std::cout << fullMessage << std::endl;
    }
}

void Logger::Debug(const std::string& message)
{
#ifdef _DEBUG
    Log(LogLevel::DEBUG, message);
#endif // DEBUG
}

void Logger::Info(const std::string& message)
{
    Log(LogLevel::INFO, message);
}

void Logger::Warning(const std::string& message)
{
    Log(LogLevel::WARNING, message);
}

void Logger::Error(const std::string& message)
{
    Log(LogLevel::ERR, message);
}

void Logger::Exception(const std::string& message)
{
    Log(LogLevel::EXCEPTION, message);
}

std::string Logger::GetThreadAddress() const
{
    std::stringstream ss;
    ss << std::hex << "[" << std::this_thread::get_id() << "]";
    return ss.str();
}

std::string Logger::GetModuleName() const
{
    std::stringstream ss;
    ss << "[" << m_moduleName << "]";
    return ss.str();
}

std::string Logger::LogLevelToString(LogLevel level) const
{
    switch (level)
    {
    case LogLevel::DEBUG: return "[DEBUG]";
    case LogLevel::INFO: return "[INFO]";
    case LogLevel::WARNING: return "[WARNING]";
    case LogLevel::ERR: return "[ERROR]";
    case LogLevel::EXCEPTION: return "[EXCEPTION]";
    default: return "[UNKNOWN]";
    }
}