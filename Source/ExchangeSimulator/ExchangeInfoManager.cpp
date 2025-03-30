/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "ExchangeInfoManager.h"
#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/FileUtils.h"
#include "../LibraryUtils/PathUtils.h"
#include "../KernelTrading/flatjson.h"

using namespace ExchangeSimulator;

ExchangeInfoManager::ExchangeInfoManager(
	const tinyxml2::XMLElement* exchangeInfoManagerCfg)
{
    assert(exchangeInfoManagerCfg);
    const auto* exchangeInfoXml = exchangeInfoManagerCfg->FirstChildElement("ExchangeInfo");
    assert(exchangeInfoXml);
    std::string exchangeInfoJsonFile(exchangeInfoXml->Attribute("File"));
    PathUtils::ReplaceSubString(exchangeInfoJsonFile, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());
	const std::string exchangeInfoJsonStr = FileUtils::ReadFileContent(exchangeInfoJsonFile);
	const flatjson::fjson exchangeInfoJson{ exchangeInfoJsonStr.c_str(), exchangeInfoJsonStr.size()};
	m_exchangeInfo = binapi::rest::exchange_info_t::construct(exchangeInfoJson);
}

ExchangeInfoManager::~ExchangeInfoManager() {}
