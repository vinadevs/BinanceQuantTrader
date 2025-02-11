/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include <unordered_map>
#include <string>

#include "DownstreamOrderAck.h"

namespace ExchangeSimulator {

	/////////////////////////////////////////////////////////////////////////////////////

	class OrderAckManager final
	{
	public:
		// Add new ack
		void AddNewAck(const std::string& orderId, const DownstreamOrderAck& ack);
	private:
		std::unordered_map<std::string, DownstreamOrderAck> m_acks;
	};
};