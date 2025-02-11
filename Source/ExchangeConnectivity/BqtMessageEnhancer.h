/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "../MiddlewareMQ/BqtJsonMessage.h"

namespace ExchangeConnectivity {
	namespace BqtMessageEnhancer {
		MiddlewareMQ::BqtJsonMessage AddTag(
			const MiddlewareMQ::BqtJsonMessage& message,
			const std::string& fieldLabels,
			const std::string& value);
	}
};
