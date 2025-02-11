/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "../MiddlewareMQ/MessageDelivery.h"
#include "../LibraryUtils/MacroUtils.h"

#include "DownstreamOrderAck.h"

#include <string>

namespace OrderManagement {
	class BinanceNewOrder;
	class BinanceCancelOrder;
	class BinanceReplaceOrder;
}

namespace LibraryUtils {
	class Logger;
};

namespace tinyxml2 {
	class XMLElement;
};

namespace ExchangeSimulator {

	// Connecting to Strategy/Algo system by this SINGLETON class allows us
	// to send acks for orders from upstream side

	class OrderAckManager;
	class AlgosConnectivity
	{
	public:
		DISABLE_COPY_AND_MOVE(AlgosConnectivity);

		static AlgosConnectivity* GetInstance();

		void InitMessageTransporter(const tinyxml2::XMLElement* messageDeliveryCfg);

		////////////////////// UPSTREAM SIMULATOR APIS ///////////////////

		MiddlewareMQ::MiddlewareMQResult
			SendDownstreamOrderAck(const DownstreamOrderAck& ack);
	private:
		AlgosConnectivity();
		~AlgosConnectivity();

		std::unique_ptr<LibraryUtils::Logger> m_logger;
		std::unique_ptr<MiddlewareMQ::MessageDelivery> m_messageDelivery;
		std::unique_ptr<OrderAckManager> m_orderAckManager;
	};
};
// Lets shorten the code line!
#define UpstreamGateWay ExchangeSimulator::AlgosConnectivity::GetInstance()