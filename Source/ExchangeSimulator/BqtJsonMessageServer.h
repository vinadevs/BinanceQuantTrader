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
		BqtJsonMessageServer(const tinyxml2::XMLElement* messageServerCfg);
		~BqtJsonMessageServer() override;

		void Start() override;
		void Stop() override;

		void OnReceivedMessage(const MiddlewareMQ::BqtJsonMessage& message) override;

		void RegisterMessageHandler(MiddlewareMQ::MessageHandler* handler) override;

		DISABLE_COPY_AND_ENABLE_MOVE(BqtJsonMessageServer);
	private:
		std::unique_ptr<LibraryUtils::Logger> m_logger;
		MiddlewareMQ::MessageHandler* m_nessageHandler {nullptr};
	};
};