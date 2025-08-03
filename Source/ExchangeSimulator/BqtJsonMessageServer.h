/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "../LibraryUtils/MacroUtils.h"
#include "../MiddlewareMQ/MessageReceiver.h"

#include "ExchangeServiceInterface.h"

#include <mutex>

namespace tinyxml2 {
	class XMLElement;
};

namespace LibraryUtils {
	class Logger;
};

namespace MiddlewareMQ {
	class MessageHandler;
};

namespace ExchangeSimulator {

	class BqtJsonMessageServer final 
		: public MiddlewareMQ::MessageReceiver,
		  public ExchangeServiceInterface
	{
	public:
		BqtJsonMessageServer(
			const tinyxml2::XMLElement* messageServerCfg,
			MiddlewareMQ::MessageHandler* messageHandler);
		~BqtJsonMessageServer() override;

		void Start() override;
		void Stop() override;

		void OnReceivedMessage(const MiddlewareMQ::BqtJsonMessage& message) override;

		DISABLE_COPY_AND_ENABLE_MOVE(BqtJsonMessageServer);
	private:
		void RegisterMessageHandler(MiddlewareMQ::MessageHandler* handler) override;

		std::unique_ptr<LibraryUtils::Logger> m_logger;
		MiddlewareMQ::MessageHandler* m_nessageHandler {nullptr};
		std::mutex m_mutex;
	};
};