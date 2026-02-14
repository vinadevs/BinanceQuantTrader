/*#*******************************************************************************
# COPYRIGHT NOTES
#-----------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "HistoricalMarketDataEvents.h"
#include "HistoricalMarketDataFeedHandler.h"

#include "../HistoricalData/HistoricalDataManager.h"
#include "../SettingNConfig/tinyxml2.h"

#include "../LibraryUtils/StringUtils.h"
#include "../LibraryUtils/PathUtils.h"
#include "../LibraryUtils/EnumIteration.h"
#include "../LibraryUtils/TimeUtils.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>

#include <fstream>
#include <filesystem>
#include <exception>

using namespace MarketData;
using namespace tinyxml2;
using namespace LibraryUtils;

HistoricalMarketDataEvents::HistoricalMarketDataEvents(
    const XMLElement* marketDataConfigXml,
	MarketDataSubject* feedHandler)
    : MarketDataEventBase(marketDataConfigXml, feedHandler)
{
	m_feedHandler = dynamic_cast<HistoricalMarketDataFeedHandler*>(feedHandler);
	assert(m_feedHandler);
    assert(m_marketDataConfigXml);
    m_logger = std::make_unique<Logger>("HistoricalMarketDataEvents");

	// prepare data file reader
    const auto* replayDataXml = m_marketDataConfigXml->FirstChildElement("ReplayData");
    assert(replayDataXml);
    std::string filePath = replayDataXml->Attribute("FilePath");
	PathUtils::ReplaceSubString(filePath, PathUtils::RootBQTPath, PathUtils::GetApplicationFolderPath());
	if (std::filesystem::exists(filePath) == false)
	{
		throw std::runtime_error("HistoricalMarketDataEvents: data file path does not exist: " + filePath);
	}
	const std::string sourceTypeStr = replayDataXml->Attribute("DataType");
	if (sourceTypeStr == "TextFile")
	{
		m_historicalDataFileReader = HistoricalDataMgr->GetHistoricalDataReader(
			filePath,
			HistoricalData::DataSourceType::TextFile);
	}
	else if (sourceTypeStr == "SQLite") 
	{
		m_historicalDataFileReader = HistoricalDataMgr->GetHistoricalDataReader(
			filePath,
			HistoricalData::DataSourceType::SQLite);
	}
	else
	{
		throw std::runtime_error("HistoricalMarketDataEvents: unsupported data source type: " + sourceTypeStr);
	}

	// prepare repeation option
	const auto* allowRepeat = replayDataXml->Attribute("AllowRepeat");
	if (StringUtils::IsConfigAttributeMatched(allowRepeat, "true"))
	{
		m_AllowRepeat = true;
	}
	else
	{
		m_AllowRepeat = false;
	}

	// prepare load event type
	const auto* loadEventXml = m_marketDataConfigXml->FirstChildElement("LoadEvent");
	assert(loadEventXml);
	const std::string loadEventType = loadEventXml->Attribute("Type");
	if (loadEventType == "TickByTick")
	{
		m_loadEvent = LoadEvent::TickByTick;
	}
	else if (loadEventType == "LastUpdateTime")
	{
		m_loadEvent = LoadEvent::LastUpdateTime;
	}
	else
	{
		throw std::runtime_error("HistoricalMarketDataEvents: unsupported load event type: " + loadEventType);
	}

	// prepare interval seconds for tick-by-tick mode
	if (m_loadEvent == LoadEvent::TickByTick)
	{
		const auto* intervalSecondsStr = loadEventXml->Attribute("IntervalSeconds");
		if (intervalSecondsStr != nullptr)
		{
			m_intervalSeconds = std::stoi(intervalSecondsStr);
			if (m_intervalSeconds <= 0)
			{
				throw std::runtime_error("HistoricalMarketDataEvents: invalid IntervalSeconds for TickByTick mode.");
			}
		}
		else
		{
			throw std::runtime_error("HistoricalMarketDataEvents: missing IntervalSeconds for TickByTick mode.");
		}
	}

	// prepare subscription types
	const auto* subscriptionDataXml = m_marketDataConfigXml->FirstChildElement("SpotSubscriptionData");
	assert(subscriptionDataXml);
	if (StringUtils::IsConfigAttributeMatched(subscriptionDataXml->Attribute("IndividualBookTickerData"), "true"))
	{
		m_subscriptionTypes.emplace_back("IndividualBookTickerData");
	}
	if (StringUtils::IsConfigAttributeMatched(subscriptionDataXml->Attribute("TradeData"), "true"))
	{
		m_subscriptionTypes.emplace_back("TradeData");
	}
	if (StringUtils::IsConfigAttributeMatched(subscriptionDataXml->Attribute("IndividualMiniTickerData"), "true"))
	{
		m_subscriptionTypes.emplace_back("IndividualMiniTickerData");
	}
	if (StringUtils::IsConfigAttributeMatched(subscriptionDataXml->Attribute("AggregateTradeData"), "true"))
	{
		m_subscriptionTypes.emplace_back("AggregateTradeData");
	}

	const auto* subscriptionFutureDataXml = m_marketDataConfigXml->FirstChildElement("FutureSubscriptionData");
	assert(subscriptionFutureDataXml);
	if (StringUtils::IsConfigAttributeMatched(subscriptionFutureDataXml->Attribute("FutureTradeData"), "true"))
	{
		m_subscriptionTypes.emplace_back("FutureTradeData");
	}
	if (StringUtils::IsConfigAttributeMatched(subscriptionFutureDataXml->Attribute("FutureBookData"), "true"))
	{
		m_subscriptionTypes.emplace_back("FutureBookData");
	}
	m_logger->Info("Initialized historical market data events");
}

HistoricalMarketDataEvents::~HistoricalMarketDataEvents()
{
    m_logger->Info("Shutdown update events and unsubscribe all symbols.");
}

bool HistoricalMarketDataEvents::Subscribe(const std::string& symbol)
{
	if (symbol.empty())
	{
		return false;
	}
	std::unique_lock<std::mutex> lock(m_eventMutex);
	m_logger->Info("Starting subscribing real time market data for symbol=" + symbol);
	if (!m_feedHandler->CreateNewMarketDataFeed(symbol))
	{
		m_logger->Warning("Could not create new market data feed for symbol=" + symbol);
		return false;
	}
	m_subscribedSymbols.emplace(symbol);
    return true;
}

bool HistoricalMarketDataEvents::Unsubscribe(const std::string& symbol)
{
	if (symbol.empty())
	{
		return false;
	}
	std::unique_lock<std::mutex> lock(m_eventMutex);
	m_logger->Info("Starting unsubscribing real time market data for symbol=" + symbol);
	m_feedHandler->RemoveMarketDataFeed(symbol);
	m_subscribedSymbols.erase(symbol);
	return true;
}

bool HistoricalMarketDataEvents::IsSubscribed(const std::string& symbol)
{
	std::unique_lock<std::mutex> lock(m_eventMutex);
	return m_subscribedSymbols.find(symbol) != m_subscribedSymbols.end();
}

void HistoricalMarketDataEvents::Wait()
{
	m_logger->Info("Starting historical market data event processing with mode=" +
		(m_loadEvent == LoadEvent::TickByTick ? std::string("TickByTick, interval seconds=") 
		+ std::to_string(m_intervalSeconds) : std::string("LastUpdateTime")) 
		+ ", AllowRepeat=" + (m_AllowRepeat ? "true" : "false"));

	const auto historicalDataPool = m_historicalDataFileReader->Read();
	do
	{
		for (const auto& dataItem : historicalDataPool)
		{
			auto symIt = dataItem.find("SYMBOL");
			if (symIt == dataItem.end())
			{
				m_logger->Error("Missing SYMBOL field in data item.");
				continue;
			}

			const std::string& symbol = symIt->second;
			if (!IsSubscribed(symbol))
				continue;

			auto typeIt = dataItem.find("DATA_NAME");
			if (typeIt == dataItem.end())
				continue;

			const std::string& dataType = typeIt->second;

			// subscription match
			if (std::find(m_subscriptionTypes.begin(),
				m_subscriptionTypes.end(),
				dataType) == m_subscriptionTypes.end())
			{
				continue;
			}

			// timing control
			if (m_loadEvent == LoadEvent::LastUpdateTime)
			{
				auto timeIt = dataItem.find("EVENT_TIME_MS");
				if (timeIt != dataItem.end() && !timeIt->second.empty())
				{
					const auto updateTimeMs = TimeUtils::StringDateTimeToEpochSeconds(timeIt->second);
					std::this_thread::sleep_until(
						std::chrono::system_clock::time_point{
							std::chrono::milliseconds(updateTimeMs) });
				}
			}
			else if (m_loadEvent == LoadEvent::TickByTick && m_intervalSeconds > 0)
			{
				std::this_thread::sleep_for(
					std::chrono::seconds(m_intervalSeconds));
			}
			// dispatch data to feed handler
			m_feedHandler->HandleDataItem(symbol, dataType, dataItem);
		}

	} while (m_AllowRepeat); // repeat if allowed from config
	m_logger->Info("Completed historical market data event processing.");
}
