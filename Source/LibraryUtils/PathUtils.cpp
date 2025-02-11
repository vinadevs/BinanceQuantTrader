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

std::string PathUtils::GetApplicationFolderPath()
{
    char szPath[MAX_PATH];
    if (GetModuleFileNameA(NULL, szPath, ARRAYSIZE(szPath)) == 0)
    {
        throw std::runtime_error("Error: GetModuleFileName failed, error code:" + std::to_string(GetLastError()));
    }
#ifdef _DEBUG
    return std::filesystem::path(szPath).parent_path().parent_path().parent_path().string();
#else
    return std::filesystem::path(szPath).parent_path().string();
#endif
}

std::string PathUtils::GetConfigFolderPath(const Path_Type type)
{
    switch (type)
    {
    case Path_Type::ROOT:
        return PathUtils::GetApplicationFolderPath() + "\\Configurations";
    case Path_Type::BQT:
        return PathUtils::GetApplicationFolderPath() + "\\Configurations\\BQT";
    case Path_Type::COMMON:
        return PathUtils::GetApplicationFolderPath() + "\\Configurations\\Common";
    case Path_Type::STRATEGY:
        return PathUtils::GetApplicationFolderPath() + "\\Configurations\\Strategy";
    case Path_Type::SIMULATOR:
        return PathUtils::GetApplicationFolderPath() + "\\Configurations\\Simulator";
    case Path_Type::MESSAGE_SERVER:
        return PathUtils::GetApplicationFolderPath() + "\\Configurations\\MessageServer";
    default:
        throw std::runtime_error("Error: GetConfigFolderPath failed, wrong path type.");
    }
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
