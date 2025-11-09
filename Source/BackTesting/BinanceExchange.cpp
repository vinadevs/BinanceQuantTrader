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

#include "../ExchangeSimulator/BinanceExchangeSimulator.h"

#include <memory>
#include <exception>
#include <string>

using namespace ExchangeSimulator;

/*
 * Binance Exchange Simulator
 *
 * Description:
 * This program simulates trading activities on the Binance cryptocurrency exchange.
 * It provides functionality to mimic market operations such as order placement,
 * matching, and transaction history tracking. The simulator is designed to help
 * users test trading strategies, analyze performance, and learn about market dynamics
 * in a controlled environment without real monetary risk.
 *
 * Features:
 * - Simulates market orders, limit orders, and stop-limit orders.
 * - Order matching engine for buy/sell transactions.
 * - Tracks portfolio and P&L (profit and loss).
 * - Configurable market conditions and parameters.
 * - Supports various cryptocurrency trading pairs.
 *
 * Usage:
 * Compile and run the program. Follow the on-screen instructions to place trades
 * and view simulated trading results.
 *
 * Disclaimer:
 * This simulator is for educational and testing purposes only. It does not interact
 * with the actual Binance exchange and should not be used for real trading activities.
 * Results obtained in the simulator do not guarantee similar performance in live markets.
 *
 */

BINANCE_EXCHANGE_SIMULATOR
{
	// LOGO
	ProgramUtils::PRINT_PROGRAM_HEADER(ProgramUtils::Programs::EXCHANGE_SIMULATOR);

	// Dependency!
	std::cout << "(THIS PROGRAM REQUIRES MessageHubServer PROGRAM, SO PLEASE MAKE SURE TO START IT FIRST.)"
		<< std::endl << std::endl;

	// LOGGER
    auto logger = std::make_unique<LibraryUtils::Logger>(BINANCE_EXCHANGE_SIMULATOR_TITLE);

	// SYSTEMS
	// if the binary has been terminated by signal, exit(), ctrl + C,... then
	// destructors will never be called, this hooker will clean up resources
#ifdef _WIN32
	if (!SetConsoleCtrlHandler(GeneralUtils::ConsoleCtrlHandler, TRUE))
	{
		logger->Error("Failed to set exit control handler.");
		return EXIT_FAILURE;
	}
#endif
	try
	{
	#ifdef _DEBUG
		const auto config_exchange_simulator_path
			= PathUtils::GetConfigFolderPath(PathUtils::Path_Type::SIMULATOR) + "\\BinanceExchangeSimulatorCfgTest.xml";
	#else
		// ARGUMENTS
		argparse::ArgumentParser program(BINANCE_EXCHANGE_SIMULATOR_TITLE);
		program.add_argument("--config_exchange_simulator_path").help("Configuration file : BinanceExchangeSimulatorCfg.xml");
		program.parse_args(argc, argv);
		const auto config_exchange_simulator_path = program.get<std::string>("--config_exchange_simulator_path");
	#endif
		// PRELOAD CONFIG XML FILE
		logger->Info("Loading xml config files.");
		auto configSimulatorXml = std::make_unique<tinyxml2::XMLDocument>();
		const auto errSimulatorXml = configSimulatorXml->LoadFile(config_exchange_simulator_path.c_str());
		if (errSimulatorXml != tinyxml2::XML_SUCCESS)
		{
			logger->Error("Load file Xml error: "
				+ std::string(tinyxml2::XMLDocument::ErrorIDToName(errSimulatorXml)) + ", error path:" + config_exchange_simulator_path);
			return EXIT_FAILURE;
		}

		// APPLICATION
		auto simulator = std::make_unique<BinanceExchangeSimulator>(configSimulatorXml.get());
		simulator->Run(); // should have never returned from this call
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
