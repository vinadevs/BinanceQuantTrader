/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "../MarketData/RealTimeMarketData.h"
#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/Logger.h"
#include "../LibraryUtils/StringUtils.h"
#include "../PythonPlugin/PythonClientConnectivity.h"

#include "MarketDataListener.h"
#include "MarketDataService.h"

using namespace MarketDataCapture;
using namespace MarketData;
using namespace tinyxml2;

MarketDataService::MarketDataService(const std::string& configFile)
	: m_logger{ std::make_unique<LibraryUtils::Logger>("MarketDataService") }
{
	m_logger->Info("Loading xml config files.");
	m_rootConfigXml = std::make_unique<tinyxml2::XMLDocument>();
	const auto errMBXml = m_rootConfigXml->LoadFile(configFile.c_str());
	if (errMBXml != tinyxml2::XML_SUCCESS)
	{
		throw std::runtime_error("Load file Xml error: "
			+ std::string(tinyxml2::XMLDocument::ErrorIDToName(errMBXml)) + ", error path:" + configFile);
	}
	m_logger->Info("Initiating Market Data Listener.");
	const auto* marketDataSourceCfg = m_rootConfigXml->FirstChildElement("MarketDataCaptureSource");
	assert(marketDataSourceCfg);
	const auto* mkDataTypeName = marketDataSourceCfg->FirstChildElement("Source")->Attribute("Type");
	const auto* dataCaptureCfg = m_rootConfigXml->FirstChildElement("MarketDataCapture");
	assert(dataCaptureCfg);
	m_marketDataListener = std::make_unique<MarketDataListener>(dataCaptureCfg);
	m_logger->Info("Initiating Real Time Market Data.");
	m_marketData = std::make_unique<RealTimeMarketData>(m_rootConfigXml.get(), mkDataTypeName);
	m_marketData->RegisterDataListener(m_marketDataListener.get());
	if (m_marketDataListener->GetDataCaptureMode() == DataCaptureMode::PythonServer) // init MQ transporter
	{
		m_logger->Info("Initiating Test Message Transporter.");
		const auto* messageTransporterCfg = m_rootConfigXml->FirstChildElement("MessageTransporter");
		PythonClientGateWay->InitMessageTransporter(messageTransporterCfg);
	}
}

void MarketDataService::SubscribeTargetSymbols()
{
	const auto* targetSymbolXml = m_rootConfigXml->FirstChildElement("SubscribingSymbol");
	assert(targetSymbolXml);
	const XMLElement* symbolsXml = targetSymbolXml->FirstChildElement("Symbols");
	assert(symbolsXml);
	auto targetTradeSymbols = StringUtils::SplitAndTrimString(symbolsXml->Attribute("List"), ',');
	if (targetTradeSymbols.empty())
	{
		throw std::runtime_error("No target symbols to subscribe market data.");
	}
	for (const auto& symbol : targetTradeSymbols)
	{
		m_marketData->SubscribeSymbol(symbol);
	}
}

MarketDataService::~MarketDataService()
{
	if (m_marketDataListener->GetDataCaptureMode() == DataCaptureMode::PythonServer) // init MQ transporter
	{
		PythonClientGateWay->StopMessageTransporter();
	}
	m_marketData->UnRegisterDataListener(m_marketDataListener.get());
}

void MarketDataService::Run()
{
	m_marketData->StartStreamingData();
}
