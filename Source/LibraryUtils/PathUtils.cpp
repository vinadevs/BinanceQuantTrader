/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "PathUtils.h"

#include <filesystem>
#include <exception>
#include <limits.h>   // for PATH_MAX

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>   // for readlink
    #include <cerrno>     // for errno
#endif

std::string PathUtils::GetApplicationFolderPath()
{
#ifdef _WIN32
    char szPath[MAX_PATH];
    if (GetModuleFileNameA(NULL, szPath, ARRAYSIZE(szPath)) == 0)
    {
        throw std::runtime_error("Error: GetModuleFileName failed, error code:" + std::to_string(GetLastError()));
    }
#else
    char szPath[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", szPath, sizeof(szPath) - 1);
    if (len == -1)
    {
        throw std::runtime_error("Error: readlink failed, errno:" + std::to_string(errno));
    }
    szPath[len] = '\0';
#endif

    std::filesystem::path exePath(szPath);
    
#ifdef _DEBUG

#ifdef _WIN32
	// Debug: executable is in Build\Debug\Source\BinanceQuantTrader\
    // Go up 3 levels to reach project root
	return exePath.parent_path().parent_path().parent_path().string();
#else
    // Debug: executable is in Build/Debug/Source/BinanceQuantTrader/
    // Go up 4 levels to reach project root
    return exePath.parent_path().parent_path().parent_path().parent_path().string();
#endif

#else
    // Release: executable is in bin/ folder
    return exePath.parent_path().string();
#endif
}

std::string PathUtils::GetConfigFolderPath(const Path_Type type)
{
    std::filesystem::path basePath = PathUtils::GetApplicationFolderPath();
    std::filesystem::path configPath;
    
    switch (type)
    {
    case Path_Type::ROOT:
        configPath = basePath / "Configurations";
        break;
    case Path_Type::BQT:
        configPath = basePath / "Configurations" / "BQT";
        break;
    case Path_Type::COMMON:
        configPath = basePath / "Configurations" / "Common";
        break;
    case Path_Type::STRATEGY:
        configPath = basePath / "Configurations" / "Strategy";
        break;
    case Path_Type::SIMULATOR:
        configPath = basePath / "Configurations" / "Simulator";
        break;
    case Path_Type::MESSAGE_SERVER:
        configPath = basePath / "Configurations" / "MessageServer";
        break;
    case Path_Type::MARKET_DATA_CAPTURE:
        configPath = basePath / "Configurations" / "MarketData";
        break;
    default:
        throw std::runtime_error("Error: GetConfigFolderPath failed, wrong path type.");
    }
    
    return configPath.string();
}

void PathUtils::ReplaceSubString(
    std::string& originalString, 
    const std::string& targetString,
    const std::string& replaceBylString)
{
    size_t pos = originalString.find(targetString);
    if (pos != std::string::npos)
    {
        while (pos != std::string::npos)
        {
            originalString.replace(pos, targetString.length(), replaceBylString);
            pos = originalString.find(targetString, pos + replaceBylString.length());
        }
    }
}
