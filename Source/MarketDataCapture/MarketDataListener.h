/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "../MarketData/MarketDataObserver.h"

namespace LibraryUtils {
    class Logger;
};

namespace tinyxml2 {
    class XMLElement;
};

namespace MarketDataCapture {

 /**
 * @class MarketDataListener
 * @brief Provides callback mechanisms for handling real-time market data events.
 *
 * The MarketDataListener class is responsible for receiving and processing real-time
 * market data updates from a WebSocket or another data feed. It provides callback
 * mechanisms that allow subscribers to react to price quotes, order book updates,
 * and trade events in real-time.
 *
 * Features:
 * - Establishes a WebSocket connection to receive market data.
 * - Supports subscription to multiple financial instruments (e.g., stocks, futures, forex).
 * - Provides event-driven callbacks for:
 *   - Price quotes (best bid/ask updates).
 *   - Order book depth updates (Level 2 market data).
 *   - Trade execution updates (last traded price and volume).
 *
 */

	enum class DataCaptureMode : unsigned
	{
		Undefined = 0,
		LocalFile, // store to local file
		ConsoleLog, // log to console
		PythonServer, // send to python client via MQ
	};

    class MarketDataFileWriter;

    class MarketDataListener final
        : public MarketData::MarketDataObserver
    {
    public:
        MarketDataListener(const tinyxml2::XMLElement* dataCaptureCfg);
        ~MarketDataListener() override;

        // book quote for downstream orders
        bool OnIndividualBookTickerChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
        // last trade for downstream orders
        bool OnTradeChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		// market ticker for downstream orders
        bool OnIndividualMarketTickerChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		// mini ticker for downstream orders
        bool OnMiniTickerChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		// aggregate trade for downstream orders
        bool OnAggregateTradeChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		// kline candle stick for downstream orders
		bool OnKlineCandleStickChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		// all market tickers for downstream orders
        bool OnAllMarketTickersChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		// all mini tickers for downstream orders
        bool OnAllMiniTickersChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		// all part depth for downstream orders
        bool OnAllPartDepthChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
		// all diff depth for downstream orders
        bool OnAllDiffDepthChange(MarketData::MarketDataSubject* marketData, const std::string& symbol) override;
    private:
        std::unique_ptr<LibraryUtils::Logger> m_logger;
        DataCaptureMode m_dataCaptureMode{ DataCaptureMode::Undefined };
		std::string m_localFilePath;
		std::unique_ptr<MarketDataFileWriter> m_fileWriter;
    };
};
