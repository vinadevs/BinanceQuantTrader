/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "OrderAckManager.h"

void ExchangeSimulator::OrderAckManager::AddNewAck(
	const std::string& orderId, const DownstreamOrderAck& ack)
{
    const auto result = m_acks.try_emplace(orderId, ack);
    if (!result.second) { // Check if the insertion was successful
    }
}
