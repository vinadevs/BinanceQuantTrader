/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/PathUtils.h"
#include "../LibraryUtils/Logger.h"

#include "BqtGlobalSettings.h"

#include <cassert>
#include <filesystem>

using namespace SettingNConfig;
using namespace tinyxml2;

BqtGlobalSettings::BqtGlobalSettings()
    : m_logger{ std::make_unique<LibraryUtils::Logger>("BqtGlobalSettings") } {}

BqtGlobalSettings::~BqtGlobalSettings() {}

BqtGlobalSettings* BqtGlobalSettings::GetInstance()
{
    static BqtGlobalSettings instance;
    return &instance;
}

void BqtGlobalSettings::InitGlobalSetting(const XMLElement* globalSettingsCfg)
{
    assert(globalSettingsCfg);
    const auto* applicationDataPathXml = globalSettingsCfg->FirstChildElement("ApplicationDataPath");
    assert(applicationDataPathXml);
    std::string applicationDataFolder(applicationDataPathXml->Attribute("Folder"));
    PathUtils::ReplaceSubString(applicationDataFolder, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());
    if (!std::filesystem::exists(applicationDataFolder)) {
        std::filesystem::create_directories(applicationDataFolder);
    }
    m_logger->Error("Using application data folder=" + applicationDataFolder);
    m_dataAppPath = applicationDataFolder;
}

const std::string& BqtGlobalSettings::GetdDataAppPath() const
{
    return m_dataAppPath;
}

