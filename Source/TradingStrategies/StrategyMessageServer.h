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