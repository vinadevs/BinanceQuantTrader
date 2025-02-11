/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include "../LibraryUtils/MacroUtils.h"
#include "../RestAPI/RestAPI.h"

#include <string>

namespace OrderManagement {
	class BinanceNewOrder;
	class BinanceCancelOrder;
	class BinanceReplaceOrder;
}

namespace LibraryUtils {
	class Logger;
};

namespace ExchangeConnectivity {

	// Connecting to Binance's exchange by thi-s SINGLETON class allows us
	// to place and queries orders from downstream side

	class DLL_CLASS_EXCHANGECONNECTIVITY_EXPORTS BinanceExchangeConnectivity final
	{
	public:
        DISABLE_COPY_AND_MOVE(BinanceExchangeConnectivity);

		static BinanceExchangeConnectivity* GetInstance();

		//////////////////////BINANCE EXCHANGE APIS ///////////////////

		binapi::rest::api::result<binapi::rest::new_order_resp_type>
			SendNewBinanceOrderFull(OrderManagement::BinanceNewOrder* newOrder);

		binapi::rest::api::result<binapi::rest::new_order_resp_type>
			SendNewBinanceTestOrderFull(OrderManagement::BinanceNewOrder* newTestOrder);

		binapi::rest::api::result<binapi::rest::my_trades_info_t>
			QueryBinanceOrderStatus(const std::string& symbol);

		binapi::rest::api::result<binapi::rest::cancel_order_info_t>
			SendCancelBinanceOrder(OrderManagement::BinanceCancelOrder* cancelOrder);

		binapi::rest::api::result<binapi::rest::cancel_order_info_t>
			SendCancelReplaceBinanceOrder(OrderManagement::BinanceReplaceOrder* replaceOrder);
	private:
		BinanceExchangeConnectivity();
		~BinanceExchangeConnectivity();

		std::unique_ptr<LibraryUtils::Logger> m_logger;
	};
};
// Lets shorten the code line!
#define BinanceExchangeGateWay ExchangeConnectivity::BinanceExchangeConnectivity::GetInstance()