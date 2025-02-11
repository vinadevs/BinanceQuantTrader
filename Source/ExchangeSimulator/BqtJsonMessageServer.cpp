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

#include "BqtJsonMessageServer.h"

using namespace ExchangeSimulator;
using namespace MiddlewareMQ;

BqtJsonMessageServer::BqtJsonMessageServer(
	const tinyxml2::XMLElement* messageServerCfg
	, MessageHandler* messageHandler)
	: MessageReceiver(messageServerCfg),
	m_logger{ std::make_unique<LibraryUtils::Logger>("BqtJsonMessageServer") }
{
	this->RegisterMessageHandler(messageHandler);
}

BqtJsonMessageServer::~BqtJsonMessageServer()
{
}

void BqtJsonMessageServer::Start()
{
	m_logger->Info("Starting receiver thread.");
	MessageReceiver::Run();
}

void BqtJsonMessageServer::Stop()
{
	MessageReceiver::Terminate();
}

void BqtJsonMessageServer::OnReceivedMessage(const BqtJsonMessage& message)
{
	m_logger->Debug("Received: " + message.SerializeMessage());
	m_nessageHandler->OnHandlingReceivedMessage(message);
}

void BqtJsonMessageServer::RegisterMessageHandler(MessageHandler* handler)
{
	assert(handler);
	m_nessageHandler = handler;
}
