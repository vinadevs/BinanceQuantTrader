/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/Logger.h"
#include "../MiddlewareMQ/MessageHandler.h"

#include "StrategyMessageServer.h"

using namespace TradingStrategies;
using namespace MiddlewareMQ;

StrategyMessageServer::StrategyMessageServer(const tinyxml2::XMLElement* messageServerCfg)
	: MessageReceiver(messageServerCfg),
	m_logger{ std::make_unique<LibraryUtils::Logger>("StrategyMessageServer") }
{
}

StrategyMessageServer::~StrategyMessageServer()
{
}

void StrategyMessageServer::Start()
{
	MessageReceiver::Run();
}

void StrategyMessageServer::OnReceivedMessage(const BqtJsonMessage& message)
{
	m_nessageHandler->OnHandlingReceivedSimulatorMessage(message);
}

void StrategyMessageServer::RegisterMessageHandler(MiddlewareMQ::MessageHandler* handler)
{
	assert(handler);
	m_nessageHandler = handler;
}
