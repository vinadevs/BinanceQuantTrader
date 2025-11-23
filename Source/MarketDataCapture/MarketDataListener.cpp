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
#include "../SettingNConfig/tinyxml2.h"
#include "../PythonPlugin/PythonClientConnectivity.h"

#include "MarketDataJSONTags.h"
#include "MarketDataListener.h"
#include "MarketDataFileWriter.h"

#include <type_traits>
#include <filesystem>

using namespace MarketDataCapture;
using namespace MarketData;

MarketDataListener::MarketDataListener(const tinyxml2::XMLElement* dataCaptureCfg)
    : m_logger{ std::make_unique<LibraryUtils::Logger>("MarketDataListener") }
{
    const auto* dataCaptureModeXml = dataCaptureCfg->FirstChildElement("DataCaptureMode");
    assert(dataCaptureModeXml);
    if (StringUtils::IsConfigAttributeMatched(dataCaptureModeXml->Attribute("Mode"), "LocalFile")) {
		m_dataCaptureMode = DataCaptureMode::LocalFile;
        const auto* localFilePathXml = dataCaptureCfg->FirstChildElement("LocalFilePath");
        assert(localFilePathXml);
        m_localFilePath = localFilePathXml->Attribute("Path");
        if (std::filesystem::exists(m_localFilePath) == false) {
            throw std::runtime_error("MarketDataListener: local file path does not exist: " + m_localFilePath);
        }
        m_fileWriter = std::make_unique<MarketDataFileWriter>(m_localFilePath, MarketDataFileWriter::DataSourceType::TextFile);
    }
    else if (StringUtils::IsConfigAttributeMatched(dataCaptureModeXml->Attribute("Mode"), "ConsoleLog")) {
		m_dataCaptureMode = DataCaptureMode::ConsoleLog;
    }
    else if (StringUtils::IsConfigAttributeMatched(dataCaptureModeXml->Attribute("Mode"), "PythonServer")) {
        m_dataCaptureMode = DataCaptureMode::PythonServer;
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
        else if (m_dataCaptureMode == DataCaptureMode::LocalFile) {
			m_fileWriter->Write((syncedData->m_individualBookTickerData).ToString());
        }
        else if (m_dataCaptureMode == DataCaptureMode::PythonServer) {
            MiddlewareMQ::BqtJsonMessage message;
			message.AddTag(FieldLabels::Header::Symbol, syncedData->GetSymbol());
            message.AddTag(FieldLabels::Header::MessageType, "IndividualBookTicker");
            const auto result = PythonClientGateWay->SendBqtJsonMessage(message);
            if (!result.m_result) {
                LOG_ERROR_STREAM(m_logger, "Failed to send market data update to Python client for symbol="
                    << symbol << ", reason=" << result.m_errMsg);
            }
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
		else if (m_dataCaptureMode == DataCaptureMode::LocalFile) {
			m_fileWriter->Write((syncedData->m_tradeData).ToString());
		}
		else if (m_dataCaptureMode == DataCaptureMode::PythonServer) {
			MiddlewareMQ::BqtJsonMessage message;
            message.AddTag(FieldLabels::Header::Symbol, syncedData->GetSymbol());
			message.AddTag(FieldLabels::Header::MessageType, "Trade");
			const auto result = PythonClientGateWay->SendBqtJsonMessage(message);
			if (!result.m_result) {
				LOG_ERROR_STREAM(m_logger, "Failed to send market data update to Python client for symbol="
					<< symbol << ", reason=" << result.m_errMsg);
			}
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
		else if (m_dataCaptureMode == DataCaptureMode::LocalFile) {
			m_fileWriter->Write((syncedData->m_individualMarketTickerData).ToString());
		}
		else if (m_dataCaptureMode == DataCaptureMode::PythonServer) {
			MiddlewareMQ::BqtJsonMessage message;
            message.AddTag(FieldLabels::Header::Symbol, syncedData->GetSymbol());
			message.AddTag(FieldLabels::Header::MessageType, "IndividualMarketTicker");
			const auto result = PythonClientGateWay->SendBqtJsonMessage(message);
			if (!result.m_result) {
				LOG_ERROR_STREAM(m_logger, "Failed to send market data update to Python client for symbol="
					<< symbol << ", reason=" << result.m_errMsg);
			}
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
		else if (m_dataCaptureMode == DataCaptureMode::LocalFile) {
			m_fileWriter->Write((syncedData->m_individualMiniTickerData).ToString());
		}
        else if (m_dataCaptureMode == DataCaptureMode::PythonServer) {
            MiddlewareMQ::BqtJsonMessage message;
            message.AddTag(FieldLabels::Header::Symbol, syncedData->GetSymbol());
			message.AddTag(FieldLabels::Header::MessageType, "MiniTicker");
            const auto result = PythonClientGateWay->SendBqtJsonMessage(message);
            if (!result.m_result) {
                LOG_ERROR_STREAM(m_logger, "Failed to send market data update to Python client for symbol="
                    << symbol << ", reason=" << result.m_errMsg);
            }
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
		else if (m_dataCaptureMode == DataCaptureMode::LocalFile) {
			m_fileWriter->Write((syncedData->m_aggregateTradeData).ToString());
		}
        else if (m_dataCaptureMode == DataCaptureMode::PythonServer) {
            MiddlewareMQ::BqtJsonMessage message;
            message.AddTag(FieldLabels::Header::Symbol, syncedData->GetSymbol());
			message.AddTag(FieldLabels::Header::MessageType, "AggregateTrade");
            const auto result = PythonClientGateWay->SendBqtJsonMessage(message);
            if (!result.m_result) {
                LOG_ERROR_STREAM(m_logger, "Failed to send market data update to Python client for symbol="
                    << symbol << ", reason=" << result.m_errMsg);
            }
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
		else if (m_dataCaptureMode == DataCaptureMode::LocalFile) {
			m_fileWriter->Write((syncedData->m_klineCandleStickData).ToString());
		}
        else if (m_dataCaptureMode == DataCaptureMode::PythonServer) {
            MiddlewareMQ::BqtJsonMessage message;
            message.AddTag(FieldLabels::Header::Symbol, syncedData->GetSymbol());
			message.AddTag(FieldLabels::Header::MessageType, "KlineCandleStick");
            const auto result = PythonClientGateWay->SendBqtJsonMessage(message);
            if (!result.m_result) {
                LOG_ERROR_STREAM(m_logger, "Failed to send market data update to Python client for symbol="
                    << symbol << ", reason=" << result.m_errMsg);
            }
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
		else if (m_dataCaptureMode == DataCaptureMode::LocalFile) {
			m_fileWriter->Write((syncedData->m_allMarketTickerData).ToString());
		}
        else if (m_dataCaptureMode == DataCaptureMode::PythonServer) {
            MiddlewareMQ::BqtJsonMessage message;
            message.AddTag(FieldLabels::Header::Symbol, syncedData->GetSymbol());
			message.AddTag(FieldLabels::Header::MessageType, "AllMarketTickers");
            const auto result = PythonClientGateWay->SendBqtJsonMessage(message);
            if (!result.m_result) {
                LOG_ERROR_STREAM(m_logger, "Failed to send market data update to Python client for symbol="
                    << symbol << ", reason=" << result.m_errMsg);
            }
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
		else if (m_dataCaptureMode == DataCaptureMode::LocalFile) {
			m_fileWriter->Write((syncedData->m_allMiniTickerData).ToString());
		}
        else if (m_dataCaptureMode == DataCaptureMode::PythonServer) {
            MiddlewareMQ::BqtJsonMessage message;
            message.AddTag(FieldLabels::Header::Symbol, syncedData->GetSymbol());
			message.AddTag(FieldLabels::Header::MessageType, "AllMiniTickers");
            const auto result = PythonClientGateWay->SendBqtJsonMessage(message);
            if (!result.m_result) {
                LOG_ERROR_STREAM(m_logger, "Failed to send market data update to Python client for symbol="
                    << symbol << ", reason=" << result.m_errMsg);
            }
        }
		return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}

bool MarketDataListener::OnAllDiffDepthChange(MarketDataSubject* marketData, const std::string& symbol)
{
    if (auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
        if (m_dataCaptureMode == DataCaptureMode::ConsoleLog) {
		LOG_INFO_STREAM(m_logger, "[Level2] Symbol=" << syncedData->GetSymbol() << "|"
			<< syncedData->m_allDiffDepthData);
		}
		else if (m_dataCaptureMode == DataCaptureMode::LocalFile) {
			m_fileWriter->Write((syncedData->m_allDiffDepthData).ToString());
		}
        else if (m_dataCaptureMode == DataCaptureMode::PythonServer) {
            MiddlewareMQ::BqtJsonMessage message;
            message.AddTag(FieldLabels::Header::Symbol, syncedData->GetSymbol());
			message.AddTag(FieldLabels::Header::MessageType, "AllDiffDepth");
            const auto result = PythonClientGateWay->SendBqtJsonMessage(message);
            if (!result.m_result) {
                LOG_ERROR_STREAM(m_logger, "Failed to send market data update to Python client for symbol="
                    << symbol << ", reason=" << result.m_errMsg);
            }
        }
		return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}

bool MarketDataListener::OnAllPartDepthChange(MarketDataSubject* marketData, const std::string& symbol)
{
    if (auto* syncedData = marketData->GetSynchronousMarketData(symbol))
    {
        if (m_dataCaptureMode == DataCaptureMode::ConsoleLog) {
            LOG_INFO_STREAM(m_logger, "[Level2] Symbol=" << syncedData->GetSymbol() << "|"
                << syncedData->m_allPartDepthData);
        }
		else if (m_dataCaptureMode == DataCaptureMode::LocalFile) {
			m_fileWriter->Write((syncedData->m_allPartDepthData).ToString());
		}
        else if (m_dataCaptureMode == DataCaptureMode::PythonServer) {
            MiddlewareMQ::BqtJsonMessage message;
            message.AddTag(FieldLabels::Header::Symbol, syncedData->GetSymbol());
			message.AddTag(FieldLabels::Header::MessageType, "AllPartDepth");
            const auto result = PythonClientGateWay->SendBqtJsonMessage(message);
            if (!result.m_result) {
                LOG_ERROR_STREAM(m_logger, "Failed to send market data update to Python client for symbol="
                    << symbol << ", reason=" << result.m_errMsg);
            }
        }
		return true;
    }
    else
    {
        m_logger->Warning("Could not found synchronized market data for symbol=" + symbol);
    }
    return false;
}
