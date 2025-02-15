/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../LibraryUtils/ArgumentParser.h"
#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/GeneralUtils.h"
#include "../LibraryUtils/Logger.h"
#include "../LibraryUtils/PathUtils.h"
#include "../LibraryUtils/ProgramLogo.h"

#include "TradingModel.h"

#include <memory>
#include <exception>
#include <string>

//****************************************************************************************************
// Quantitative trading model for Binance Crypto Exchange
//****************************************************************************************************
// - Binance Quantitative Trader is an advanced High-Frequency Trading (HFT) system designed to
// automateand optimize trading strategies on the Binance exchange. Leveraging quantitative analysis,
// statistical models, and real-time data, this system aims to execute trades with precision,
// speed, and efficiency beyond manual capabilities. Quantitative high speed trading systems are 
// built to make data-driven decisions, minimizing emotional influence in trading and
// maximizing profit potential by analyzing large volumes of market data and historical patterns.
//****************************************************************************************************

BINANCE_QUANT_TRADING_MODEL
{
	// LOGO
	ProgramUtils::PRINT_PROGRAM_HEADER(ProgramUtils::Programs::BQT);

	// LOGGER
    auto logger = std::make_unique<LibraryUtils::Logger>(BINANCE_QUANT_TRADING_MODEL_TITLE);

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
		const auto config_binance_quant_trader_path
			= PathUtils::GetConfigFolderPath(PathUtils::Path_Type::BQT) + "\\BinanceQuantTraderCfg.xml";
		const auto config_binance_access_key_path
			= PathUtils::GetConfigFolderPath(PathUtils::Path_Type::BQT) + "\\BinanceAKCfgTest.xml";
#else
		// ARGUMENTS
		argparse::ArgumentParser program(BINANCE_QUANT_TRADING_MODEL_TITLE);
		program.add_argument("--config_binance_quant_trader_path").help("Configuration file : BinanceQuantTraderCfg.xml");
		program.add_argument("--config_binance_access_key_path").help("Access key file : BinanceAKCfg.xml");
		program.parse_args(argc, argv);
		const auto config_binance_quant_trader_path = program.get<std::string>("--config_binance_quant_trader_path");
		const auto config_binance_access_key_path = program.get<std::string>("--config_binance_access_key_path");
#endif
		// PRELOAD CONFIG XML FILE
		logger->Info("Loading xml config files.");
		auto configBQTXml = std::make_unique<tinyxml2::XMLDocument>();
		const auto errBQTXml = configBQTXml->LoadFile(config_binance_quant_trader_path.c_str());
		if (errBQTXml != tinyxml2::XML_SUCCESS)
		{
			logger->Error("Load file Xml error: " 
				+ std::string(tinyxml2::XMLDocument::ErrorIDToName(errBQTXml)) + ", error path:" + config_binance_quant_trader_path);
			return EXIT_FAILURE;
		}

		auto configAccessKeyXml = std::make_unique<tinyxml2::XMLDocument>();
		const auto errAccessKeyXml = configAccessKeyXml->LoadFile(config_binance_access_key_path.c_str());
		if (errAccessKeyXml != tinyxml2::XML_SUCCESS)
		{
			logger->Error("Load file Xml error: " 
				+ std::string(tinyxml2::XMLDocument::ErrorIDToName(errAccessKeyXml)) + ", error path:" + config_binance_access_key_path);
			return EXIT_FAILURE;
		}

		// APPLICATION: BUILD AND START TRADING MODEL
		logger->Info("Starting Trading Model.");
		std::unique_ptr<BinanceQuantTrader::TradingModel> model
			= std::make_unique<BinanceQuantTrader::TradingModel>(configBQTXml.get(), configAccessKeyXml.get());
		model->RunModel();
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