/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include "../LibraryUtils/MacroUtils.h"
#include "../MiddlewareMQ/MessageReceiver.h"

namespace tinyxml2 {
	class XMLElement;
};

namespace LibraryUtils {
	class Logger;
};

namespace MiddlewareMQ {
	class MessageHandler;
};

namespace TradingStrategies {

	// The StrategyMessageServer class is responsible for managing the communication 
	// between trading strategies and the middleware message queue (Exchange Simulator).
	// It inherits from MiddlewareMQ::MessageReceiver to handle incoming messages and
	// provides functionality to start the server, process received messages, and 
	// register message handlers. This class is designed to be non-copyable but 
	// movable, ensuring efficient resource management. It utilizes a logger for 
	// logging purposes and adheres to the C++17 standard.

	class DLL_CLASS_TRADING_TRATEGIES_EXPORTS
		StrategyMessageServer final : public MiddlewareMQ::MessageReceiver
	{
	public:
		StrategyMessageServer(const tinyxml2::XMLElement* messageServerCfg);
		~StrategyMessageServer() override;

		void Start();

		void OnReceivedMessage(const MiddlewareMQ::BqtJsonMessage& message) override;

		void RegisterMessageHandler(MiddlewareMQ::MessageHandler* handler) override;

		DISABLE_COPY_AND_ENABLE_MOVE(StrategyMessageServer);
	private:
		std::unique_ptr<LibraryUtils::Logger> m_logger;
		MiddlewareMQ::MessageHandler* m_nessageHandler{ nullptr };
	};
};