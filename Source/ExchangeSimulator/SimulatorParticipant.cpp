/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "SimulatorParticipant.h"

using namespace ExchangeSimulator;

SimulatorParticipant::SimulatorParticipant(UserAccountManager* userAccountManager)
	: Participant(ParticipantType::REAL_TIME_MARKET_DATA, userAccountManager)
{
}

SimulatorParticipant::~SimulatorParticipant()
{
}
