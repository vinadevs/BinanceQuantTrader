/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "../LibraryUtils/ArgumentParser.h"
#include "../LibraryUtils/Logger.h"
#include "../LibraryUtils/PathUtils.h"
#include "../LibraryUtils/ProgramLogo.h"
#include "../LibraryUtils/GeneralUtils.h"
#include "../SettingNConfig/tinyxml2.h"

#include "MarketDataService.h"

#include <memory>
#include <exception>
#include <string>

using namespace MarketDataCapture;

/**
 * Market Data Capture Tool
 * 
 * @brief Real-time Market Data Capture using WebSocket
 *
 * This program connects to Binance market data provider using a WebSocket API
 * to capture real-time market data, such as price quotes, order book updates,
 * and trade executions. The captured data can be used for trading strategies,
 * market analysis, or algorithmic trading systems.
 *
 * Features:
 * - Establishes a WebSocket connection to a market data provider.
 * - Subscribes to specific symbols (e.g., S&P 500, Nasdaq, Forex pairs).
 * - Receives and processes live market data in JSON format.
 * - Parses and stores the data for further analysis.
 * - Handles WebSocket reconnections and error handling.
 *
 * Dependencies:
 * - WebSocket library (e.g., Boost.Beast, uWebSockets, cpp-httplib)
 * - JSON parsing library (e.g., JSON for Modern C++, nlohmann/json)
 *
 */
BINANCE_MARKET_DATA_CAPTURE_TOOL
{
	// LOGO
	ProgramUtils::PRINT_PROGRAM_HEADER(ProgramUtils::Programs::MARKET_DATA_CAPTURE);

	// LOGGER
	auto logger = std::make_unique<LibraryUtils::Logger>(BINANCE_MARKET_DATA_CAPTURE_TOOL_TITLE);

	// SYSTEMS
	// if the binary has been terminated by signal, exit(), ctrl + C,... then
	// destructors will never be called, this hooker will clean up resources
	if (!SetConsoleCtrlHandler(GeneralUtils::ConsoleCtrlHandler, TRUE))
	{
		logger->Error("Failed to set exit control handler.");
		return EXIT_FAILURE;
	}

	try
	{
#ifdef _DEBUG
		const auto config_market_data_path
			= PathUtils::GetConfigFolderPath(PathUtils::Path_Type::MARKET_DATA_CAPTURE) + "\\RealtimeMarketDataCfg.xml";
#else
		// ARGUMENTS
		argparse::ArgumentParser program(BINANCE_MESSAGE_HUB_SERVER_TITLE);
		program.add_argument("--config_market_data_path").help("Configuration file : RealtimeMarketDataCfg.xml");
		program.parse_args(argc, argv);
		const auto config_market_data_path = program.get<std::string>("--config_market_data_path");
#endif
		// APPLICATION
		auto marketDataService = std::make_unique<MarketDataService>(config_market_data_path);
		marketDataService->Run(); // this is a wait call, so should not return
	}
	catch (const std::exception& e)
	{
		logger->Exception(std::string(e.what()));
		return EXIT_FAILURE;
	}
	catch (...)
	{
		logger->Exception("Unknown exception occurred.");
	}
	return EXIT_SUCCESS;
}
