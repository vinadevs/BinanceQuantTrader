/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include "../MiddlewareMQ/MessageDelivery.h"
#include "../LibraryUtils/MacroUtils.h"

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

namespace ExchangeConnectivity {

	// Connecting to Simulator's exchange by this SINGLETON class allows us
	// to place and queries orders from downstream side

	class BinanceWalletClient;
	class DLL_CLASS_EXCHANGECONNECTIVITY_EXPORTS ExchangeSimulatorConnectivity final
	{
	public:
		DISABLE_COPY_AND_MOVE(ExchangeSimulatorConnectivity);

		static ExchangeSimulatorConnectivity* GetInstance();

		void InitMessageTransporter(const tinyxml2::XMLElement* messageDeliveryCfg);

		void InitBinanceWalletClient(const tinyxml2::XMLElement* binanceWalletClientXmlCfg);

		////////////////////// EXCHANGE SIMULATOR APIS ///////////////////

		MiddlewareMQ::MiddlewareMQResult
			SendNewSimulatorOrderFull(OrderManagement::BinanceNewOrder* newOrder);

		MiddlewareMQ::MiddlewareMQResult
			QuerySimulatorOrderStatus(const std::string& symbol);

		MiddlewareMQ::MiddlewareMQResult
			SendCancelSimulatorOrder(OrderManagement::BinanceCancelOrder* cancelOrder);

		MiddlewareMQ::MiddlewareMQResult
			SendCancelReplaceSimulatorOrder(OrderManagement::BinanceReplaceOrder* replaceOrder);

		void GetUserAccountData(const std::string& user_id);
	private:
		ExchangeSimulatorConnectivity();
		~ExchangeSimulatorConnectivity();

		std::unique_ptr<LibraryUtils::Logger> m_logger;
		std::unique_ptr<MiddlewareMQ::MessageDelivery> m_messageDelivery;
		std::unique_ptr<BinanceWalletClient> m_binanceWalletClient;
	};
};
// Lets shorten the code line!
#define ExchangeSimulatorGateWay ExchangeConnectivity::ExchangeSimulatorConnectivity::GetInstance()