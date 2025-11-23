/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "BqtMessageEnhancer.h"
#include "../OrderManagement/FieldLabels.h"

using namespace OrderManagement;
using namespace ExchangeConnectivity;

MiddlewareMQ::BqtJsonMessage BqtMessageEnhancer::AddTag(
	const MiddlewareMQ::BqtJsonMessage& message,
	const std::string& fieldLabels,
	const std::string& value)
{
	MiddlewareMQ::BqtJsonMessage newMessage = message;
	newMessage.AddTag(fieldLabels, value);
	return newMessage;
}
