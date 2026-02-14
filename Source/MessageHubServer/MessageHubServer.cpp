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

#include "../MiddlewareMQ/MessageBroker.h"

#include <memory>
#include <exception>
#include <string>

using namespace MiddlewareMQ;

/*
 * ZeroMQ Connection Load Balancer
 *
 * Description:
 * This program implements a load balancer using ZeroMQ to efficiently distribute
 * workload among multiple workers. It acts as an intermediary between clients and
 * workers, ensuring that requests are balanced across available resources for
 * optimal performance and minimal latency.
 *
 * Features:
 * - Uses ZeroMQ sockets for asynchronous messaging.
 * - Implements a round-robin or dynamic load balancing strategy.
 * - Handles connection management and fault tolerance.
 * - Supports scalable worker pool configuration.
 * - Provides real-time logging and monitoring of request distribution.
 *
 * Usage:
 * - Configure the ZeroMQ socket types (e.g., DEALER, ROUTER).
 * - Specify the number of worker nodes and start the balancer.
 * - Clients send requests to the balancer; workers process the tasks.
 * - Monitor task distribution and worker utilization using logs.
 *
 * Dependencies:
 * - ZeroMQ library (ensure it's installed and linked during compilation).
 *
 * Disclaimer:
 * This program is intended for demonstration and educational purposes. Proper testing
 * and evaluation are necessary before deploying it in production environments.
 *
 */

BINANCE_MESSAGE_HUB_SERVER
{
	// LOGO
	ProgramUtils::PRINT_PROGRAM_HEADER(ProgramUtils::Programs::MESSAGE_HUB_SERVER);

	// LOGGER
    auto logger = std::make_unique<LibraryUtils::Logger>(BINANCE_MESSAGE_HUB_SERVER_TITLE);

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
		const auto config_message_hub_server_path
			= PathUtils::GetConfigFolderPath(PathUtils::Path_Type::MESSAGE_SERVER) + "\\MessageBrokerCfg.xml";
#else
		// ARGUMENTS
		argparse::ArgumentParser program(BINANCE_MESSAGE_HUB_SERVER_TITLE);
		program.add_argument("--config_message_hub_server_path").help("Configuration file : MessageHubServerCfg.xml");
		program.parse_args(argc, argv);
		const auto config_message_hub_server_path = program.get<std::string>("--config_message_hub_server_path");
#endif
		// APPLICATION
 		auto messageHubBroker = std::make_unique<MessageBroker>(config_message_hub_server_path);
		messageHubBroker->Run(); // this is a wait call, so should not return
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
