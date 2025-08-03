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
#include "../KernelTrading/types.h"
#include "../LibraryUtils/MacroUtils.h"

#include <string>

namespace OrderManagement {
	class BinanceNewOrder;
	class BinanceCancelOrder;
	class BinanceReplaceOrder;
	class BinanceQueryOrder;
}

namespace KernelTrading {
	class UserFutureAccount;
};

namespace LibraryUtils {
	class Logger;
};

namespace tinyxml2 {
	class XMLElement;
};

namespace binapi {
	namespace rest {
		struct account_info_t;
	};
};

namespace ExchangeConnectivity {

	// Connecting to Simulator's exchange by this SINGLETON class allows us
	// to place and queries orders from downstream side

	class BinanceWalletClient;
	class BinanceExchangeClient;
	class BinanceTradeProfile;

	class DLL_CLASS_EXCHANGECONNECTIVITY_EXPORTS ExchangeSimulatorConnectivity final
	{
	public:
		DISABLE_COPY_AND_MOVE(ExchangeSimulatorConnectivity);

		static ExchangeSimulatorConnectivity* GetInstance();

		void InitMessageTransporter(const tinyxml2::XMLElement* messageDeliveryCfg);

		void InitBinanceWalletClient(const tinyxml2::XMLElement* binanceWalletClientXmlCfg);
		
		void InitBinanceExchangeClient(const tinyxml2::XMLElement* binanceExchangeClientXmlCfg);

		void InitBinanceTradeProfile(const tinyxml2::XMLElement* binanceTradeProfileXmlCfg);

		////////////////////// EXCHANGE SIMULATOR APIS ///////////////////

		MiddlewareMQ::MiddlewareMQResult
			SendNewSimulatorOrderFull(OrderManagement::BinanceNewOrder* newOrder);

		MiddlewareMQ::MiddlewareMQResult
			SendCancelSimulatorOrder(OrderManagement::BinanceCancelOrder* cancelOrder);

		MiddlewareMQ::MiddlewareMQResult
			SendCancelReplaceSimulatorOrder(OrderManagement::BinanceReplaceOrder* replaceOrder);

		MiddlewareMQ::MiddlewareMQResult
			QuerySimulatorOrderStatus(OrderManagement::BinanceQueryOrder* queryOrder);

		bool GetUserAccountInfo(
			const std::string& userId,
			binapi::rest::account_info_t* account,
			std::string& errorMessage);

		bool GetUserFutureAccountInfo(
			const std::string& userId,
			KernelTrading::UserFutureAccount* userFutureAccount,
			std::string& errorMessage);

		bool GetExchangeInfo(
			const std::string& symbol,
			binapi::rest::exchange_info_t* exchangeInfo,
			std::string& errorMessage);

		bool UpdateUserTradeProfileData(
			const std::string& userId,
			const double leverageRate,
			std::string& resultMessage);
	private:
		ExchangeSimulatorConnectivity();
		~ExchangeSimulatorConnectivity();

		std::unique_ptr<LibraryUtils::Logger> m_logger;
		std::unique_ptr<MiddlewareMQ::MessageDelivery> m_messageDelivery;
		std::unique_ptr<BinanceWalletClient> m_binanceWalletClient;
		std::unique_ptr<BinanceExchangeClient> m_binanceExchangeClient;
		std::unique_ptr<BinanceTradeProfile> m_binanceTradeProfile;
	};
};
// Lets shorten the code line!
#define ExchangeSimulatorGateWay ExchangeConnectivity::ExchangeSimulatorConnectivity::GetInstance()