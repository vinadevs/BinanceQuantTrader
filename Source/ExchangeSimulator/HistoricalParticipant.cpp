/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "HistoricalParticipant.h"

using namespace ExchangeSimulator;

HistoricalParticipant::HistoricalParticipant(UserAccountManager* userAccountManager)
	: Participant(ParticipantType::HISTORICAL_DATA, userAccountManager, nullptr)
{
}

HistoricalParticipant::~HistoricalParticipant()
{
}
