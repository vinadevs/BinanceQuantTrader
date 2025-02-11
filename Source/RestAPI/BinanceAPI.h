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

#include <boost/asio/io_context.hpp>

#include <string>
#include <memory>

namespace binapi {
	namespace rest {
		struct api;
	}
}

namespace RestAPI {

	/* The Binance REST API allows developers to interact programmatically
	with Binance’s trading platform, enabling actions such as 
	retrieving market data, managing accounts, placing or canceling
	orders, and tracking trades.It is primarily aimed at developers 
	building trading bots, portfolio management tools, or other 
	financial applications on Binance. */

	class DLL_CLASS_RESTAPI_EXPORTS BinanceAPI final
	{
	public:
		BinanceAPI() = default;
		~BinanceAPI();

		DISABLE_COPY_AND_MOVE(BinanceAPI);

		static BinanceAPI* GetInstance();

		void InitiateAPI(const std::string& host,
						 const std::string& port,
						 const std::string& pk,
						 const std::string& sk,
						 const std::string& timeout);

		binapi::rest::api* AccessAPI();

	private:
		std::unique_ptr<binapi::rest::api> m_binAPI;
		boost::asio::io_context m_ioContext;
	};
};
// Lets shorten the code line!
#define BinanceApiGateWay RestAPI::BinanceAPI::GetInstance()->AccessAPI()