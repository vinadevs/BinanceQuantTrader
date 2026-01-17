#pragma once

#include "dlldefine.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

namespace HistoricalData {
    // Enum for data source type
    enum class DataSourceType : unsigned
    {
        Undefined = 0,
		TextFile, // Plain text file
		SQLite // SQLite database
    };

	enum class HistoricalDataType : unsigned
	{
		OHLCV, // Open High Low Close Volume data
		TICK_BY_TICK, // tick by tick data
		LAST_TRADE, // last trade data
		ORDER_BOOK, // order book data
		FULL, // full historical data
	};
} // namespace MarketDataCapture
