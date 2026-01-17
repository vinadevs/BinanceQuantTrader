/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "../MarketData/MarketDataSubject.h"
#include "../MarketData/SynchronousMarketData.h"
#include "../LibraryUtils/Logger.h"
#include "../LibraryUtils/TimeUtils.h"
#include "../LibraryUtils/StringUtils.h"
#include "../LibraryUtils/PathUtils.h"
#include "../SettingNConfig/tinyxml2.h"
#include "../PythonPlugin/PythonClientConnectivity.h"
#include "../HistoricalData/HistoricalDataManager.h"
#include "../HistoricalData/MarketDataFileWriter.h"

#include "MarketDataListener.h"
#include "MarketDataToJsonConverter.h"

#include <filesystem>

using namespace MarketDataCapture;
using namespace MarketData;

MarketDataListener::MarketDataListener(const tinyxml2::XMLElement* dataCaptureCfg)
    : m_logger{ std::make_unique<LibraryUtils::Logger>("MarketDataListener") }
{
    const auto* dataCaptureModeXml = dataCaptureCfg->FirstChildElement("DataCaptureMode");
    assert(dataCaptureModeXml);
    if (StringUtils::IsConfigAttributeMatched(dataCaptureModeXml->Attribute("Mode"), "SaveHistoricalData")) {
		m_dataCaptureMode = DataCaptureMode::SaveHistoricalData;
        const auto* localFilePathXml = dataCaptureCfg->FirstChildElement("DataRepository");
        assert(localFilePathXml);
        std::string dataPath = localFilePathXml->Attribute("Path");
        PathUtils::ReplaceSubString(dataPath, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());
        if (std::filesystem::exists(dataPath) == false) {
            throw std::runtime_error("MarketDataListener: local file path does not exist: " + m_localFilePath);
        }
		m_localFilePath = dataPath + TimeUtils::GetCurrentTimestampStringPath() + "_MarketData.txt";
		m_fileWriter = HistoricalDataMgr->GetHistoricalDataWriter(
            m_localFilePath, HistoricalData::DataSourceType::TextFile);
		if (!m_fileWriter) {
			throw std::runtime_error("MarketDataListener: could not create MarketDataFileWriter for file: " + m_localFilePath);
		}
		m_logger->Info("storing market data to local file: " + m_localFilePath);
    }
    else if (StringUtils::IsConfigAttributeMatched(dataCaptureModeXml->Attribute("Mode"), "ConsoleLog")) {
		m_dataCaptureMode = DataCaptureMode::ConsoleLog;
		m_logger->Info("logging market data to console");
    }
    else if (StringUtils::IsConfigAttributeMatched(dataCaptureModeXml->Attribute("Mode"), "PythonServer")) {
        m_dataCaptureMode = DataCaptureMode::PythonServer;
		m_logger->Info("sending market data to Python server via MQ");
    }
    else {
		throw std::runtime_error("Unsupported data capture mode: " + std::string(dataCaptureModeXml->Attribute("Mode")));
    }
}

MarketDataListener::~MarketDataListener() {}

bool MarketDataListener::OnIndividualBookTickerChange(
	MarketDataSubject* marketData, const std::string& symbol)
{
    if (auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
        if (m_dataCaptureMode == DataCaptureMode::ConsoleLog) {
            LOG_INFO_STREAM(m_logger, "[Level2] Symbol=" << syncedData->GetSymbol() << "|"
                << syncedData->m_individualBookTickerData);
        }
        else if (m_dataCaptureMode == DataCaptureMode::SaveHistoricalData) {
			m_fileWriter->Write("SYMBOL=" + symbol + "|" + (syncedData->m_individualBookTickerData).ToString());
        }
        else if (m_dataCaptureMode == DataCaptureMode::PythonServer) {
			MiddlewareMQ::BqtJsonMessage message = PythonMessage::IndividualBookTickerToJsonMessage(
				syncedData->m_individualBookTickerData, syncedData->GetSymbol());
            return PythonClientGateWay->SendBqtJsonMessage(message).m_result;
        }
        return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}

bool MarketDataListener::OnTradeChange(
	MarketDataSubject* marketData, const std::string& symbol)
{
    if (auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
        if (m_dataCaptureMode == DataCaptureMode::ConsoleLog) {
            LOG_INFO_STREAM(m_logger, "[Level1] Symbol=" << syncedData->GetSymbol() << "|"
                << syncedData->m_tradeData);
        }
		else if (m_dataCaptureMode == DataCaptureMode::SaveHistoricalData) {
			m_fileWriter->Write("SYMBOL=" + symbol + "|" + (syncedData->m_tradeData).ToString());
		}
		else if (m_dataCaptureMode == DataCaptureMode::PythonServer) {
			MiddlewareMQ::BqtJsonMessage message = PythonMessage::TradeToJsonMessage(
				syncedData->m_tradeData, syncedData->GetSymbol());
            return PythonClientGateWay->SendBqtJsonMessage(message).m_result;
		}
        return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}

bool MarketDataListener::OnIndividualMarketTickerChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (auto* syncedData = marketData->GetSynchronousMarketData(symbol)) 
    {
        if (m_dataCaptureMode == DataCaptureMode::ConsoleLog) {
            LOG_INFO_STREAM(m_logger, "[Level1] Symbol=" << syncedData->GetSymbol() << "|"
                << syncedData->m_individualMarketTickerData);
        }
		else if (m_dataCaptureMode == DataCaptureMode::SaveHistoricalData) {
			m_fileWriter->Write("SYMBOL=" + symbol + "|" + (syncedData->m_individualMarketTickerData).ToString());
		}
		else if (m_dataCaptureMode == DataCaptureMode::PythonServer) {
			MiddlewareMQ::BqtJsonMessage message = PythonMessage::IndividualMarketTickerToJsonMessage(
				syncedData->m_individualMarketTickerData, syncedData->GetSymbol());
            return PythonClientGateWay->SendBqtJsonMessage(message).m_result;
		}
		return true;
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
	return false;
}

bool MarketDataListener::OnMiniTickerChange(MarketDataSubject* marketData, const std::string& symbol)
{
    if (auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
        if (m_dataCaptureMode == DataCaptureMode::ConsoleLog) {
            LOG_INFO_STREAM(m_logger, "[Level1] Symbol=" << syncedData->GetSymbol() << "|"
                << syncedData->m_individualMiniTickerData);
        }
		else if (m_dataCaptureMode == DataCaptureMode::SaveHistoricalData) {
			m_fileWriter->Write("SYMBOL=" + symbol + "|" + (syncedData->m_individualMiniTickerData).ToString());
		}
        else if (m_dataCaptureMode == DataCaptureMode::PythonServer) {
			MiddlewareMQ::BqtJsonMessage message = PythonMessage::IndividualMiniTickerToJsonMessage(
				syncedData->m_individualMiniTickerData, syncedData->GetSymbol());
            return PythonClientGateWay->SendBqtJsonMessage(message).m_result;
        }
        return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}

bool MarketDataListener::OnAggregateTradeChange(MarketDataSubject* marketData, const std::string& symbol)
{
    if (auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
        if (m_dataCaptureMode == DataCaptureMode::ConsoleLog) {
            LOG_INFO_STREAM(m_logger, "[Level1] Symbol=" << syncedData->GetSymbol() << "|"
                << syncedData->m_aggregateTradeData);
        }
		else if (m_dataCaptureMode == DataCaptureMode::SaveHistoricalData) {
			m_fileWriter->Write((syncedData->m_aggregateTradeData).ToString());
		}
        else if (m_dataCaptureMode == DataCaptureMode::PythonServer) {
			MiddlewareMQ::BqtJsonMessage message = PythonMessage::AggregateTradeToJsonMessage(
				syncedData->m_aggregateTradeData, syncedData->GetSymbol());
            return PythonClientGateWay->SendBqtJsonMessage(message).m_result;
        }
        return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}

bool MarketDataListener::OnKlineCandleStickChange(MarketDataSubject* marketData, const std::string& symbol)
{
	if (auto* syncedData = marketData->GetSynchronousMarketData(symbol))
	{
        if (m_dataCaptureMode == DataCaptureMode::ConsoleLog) {
            LOG_INFO_STREAM(m_logger, "[Level1] Symbol=" << syncedData->GetSymbol() << "|"
                << syncedData->m_klineCandleStickData);
        }
		else if (m_dataCaptureMode == DataCaptureMode::SaveHistoricalData) {
			m_fileWriter->Write("SYMBOL=" + symbol + "|" + (syncedData->m_klineCandleStickData).ToString());
		}
        else if (m_dataCaptureMode == DataCaptureMode::PythonServer) {
			MiddlewareMQ::BqtJsonMessage message = PythonMessage::KlineCandleStickToJsonMessage(
				syncedData->m_klineCandleStickData, syncedData->GetSymbol());
            return PythonClientGateWay->SendBqtJsonMessage(message).m_result;
        }
		return true;
	}
	else
	{
		m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
	}
    return false;
}

bool MarketDataListener::OnAllMarketTickersChange(MarketDataSubject* marketData, const std::string& symbol)
{
    if (auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
        if (m_dataCaptureMode == DataCaptureMode::ConsoleLog) {
            LOG_INFO_STREAM(m_logger, "[Level2] Symbol=" << syncedData->GetSymbol() << "|"
                << syncedData->m_allMarketTickerData);
        }
		else if (m_dataCaptureMode == DataCaptureMode::SaveHistoricalData) {
			m_fileWriter->Write("SYMBOL=" + symbol + "|" + (syncedData->m_allMarketTickerData).ToString());
		}
        else if (m_dataCaptureMode == DataCaptureMode::PythonServer) {
			MiddlewareMQ::BqtJsonMessage message = PythonMessage::AllMarketTickersToJsonMessage(
				syncedData->m_allMarketTickerData, syncedData->GetSymbol());
            return PythonClientGateWay->SendBqtJsonMessage(message).m_result;
        }
		return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}

bool MarketDataListener::OnAllMiniTickersChange(MarketDataSubject* marketData, const std::string& symbol)
{
    if (auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
        if (m_dataCaptureMode == DataCaptureMode::ConsoleLog) {
            LOG_INFO_STREAM(m_logger, "[Level1] Symbol=" << syncedData->GetSymbol() << "|"
                << syncedData->m_allMiniTickerData);
        }
		else if (m_dataCaptureMode == DataCaptureMode::SaveHistoricalData) {
			m_fileWriter->Write("SYMBOL=" + symbol + "|" + (syncedData->m_allMiniTickerData).ToString());
		}
        else if (m_dataCaptureMode == DataCaptureMode::PythonServer) {
			MiddlewareMQ::BqtJsonMessage message = PythonMessage::IndividualBookTickerToJsonMessage(
				syncedData->m_individualBookTickerData, syncedData->GetSymbol());
            return PythonClientGateWay->SendBqtJsonMessage(message).m_result;
        }
		return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}

#if 0 // Not interested for now
bool MarketDataListener::OnAllDiffDepthChange(MarketDataSubject* marketData, const std::string& symbol)
{
    if (auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
        if (m_dataCaptureMode == DataCaptureMode::ConsoleLog) {
            LOG_INFO_STREAM(m_logger, "[Level2] Symbol=" << syncedData->GetSymbol() << "|"
                << syncedData->m_allDiffDepthData);
        }
        else if (m_dataCaptureMode == DataCaptureMode::SaveHistoricalData) {
            m_fileWriter->Write("SYMBOL=" + symbol + "|" + (syncedData->m_allDiffDepthData).ToString());
        }
        else if (m_dataCaptureMode == DataCaptureMode::PythonServer) {
            MiddlewareMQ::BqtJsonMessage message = PythonMessage::AllDiffDepthToJsonMessage(
                syncedData->m_allDiffDepthData, syncedData->GetSymbol());
            return PythonClientGateWay->SendBqtJsonMessage(message).m_result;
        }
        return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}
#endif // 0

#if 0 // Not interested for now
bool MarketDataListener::OnAllPartDepthChange(MarketDataSubject* marketData, const std::string& symbol)
{
    if (auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
        if (m_dataCaptureMode == DataCaptureMode::ConsoleLog) {
            LOG_INFO_STREAM(m_logger, "[Level2] Symbol=" << syncedData->GetSymbol() << "|"
                << syncedData->m_allPartDepthData);
        }
		else if (m_dataCaptureMode == DataCaptureMode::SaveHistoricalData) {
			m_fileWriter->Write("SYMBOL=" + symbol + "|" + (syncedData->m_allPartDepthData).ToString());
		}
        else if (m_dataCaptureMode == DataCaptureMode::PythonServer) {
			MiddlewareMQ::BqtJsonMessage message = PythonMessage::AllPartDepthDataToJsonMessage(
				syncedData->m_allPartDepthData, syncedData->GetSymbol());
            return PythonClientGateWay->SendBqtJsonMessage(message).m_result;
        }
		return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}
#endif // 0