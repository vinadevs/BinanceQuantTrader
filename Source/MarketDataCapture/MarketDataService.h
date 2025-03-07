/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "MarketDataListener.h"

#include <string>
#include <memory>

namespace tinyxml2 {
    class XMLDocument;
};

namespace LibraryUtils {
    class Logger;
};

namespace MarketData {
    class RealTimeMarketData;
};

namespace MarketDataCapture {

 /**
 * @class MarketDataService
 * @brief Wrapper class for handling real-time market data events.
 *
 * The MarketDataService class acts as a high-level interface that wraps around
 * the MarketDataListener.
 *
 * Features:
 * - Manages the lifecycle of MarketDataListener.
 *
 * Dependencies:
 * - Requires an instance of MarketDataListener for receiving raw market data.
 * - Uses WebSocket for real-time market updates.
 *
 */
    class MarketDataListener;

    class MarketDataService final
    {
    public:
        MarketDataService(const std::string& configFile);
        ~MarketDataService();

        void Run();
    private:
        std::unique_ptr<tinyxml2::XMLDocument> m_rootConfigXml;
        std::unique_ptr<LibraryUtils::Logger> m_logger;
        std::unique_ptr<MarketData::RealTimeMarketData> m_marketData;
        std::unique_ptr<MarketDataListener> m_marketDataListener;
    };
};
