/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../LibraryUtils/Logger.h"
#include "Participant.h"

using namespace ExchangeSimulator;
using namespace OrderManagement;

Participant::Participant(const ParticipantType& mode, UserAccountManager* userAccountManager)
  : m_logger{ std::make_unique<LibraryUtils::Logger>("Participant") }, 
	m_downstreamOrderBook{ std::make_unique<DownstreamOrderBook>() },
	m_tradeMode(mode),
	m_userAccountManager(userAccountManager)
{
}

Participant::~Participant()
{
}
