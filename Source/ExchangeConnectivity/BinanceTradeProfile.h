/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "protobuf/user_trade_profile.pb.h"
#include "protobuf/user_trade_profile.grpc.pb.h"
#undef max
#undef min
#include <grpcpp/grpcpp.h>

#include <memory>
#include <string>

namespace LibraryUtils {
	class Logger;
};

namespace tinyxml2 {
	class XMLElement;
};

namespace ExchangeConnectivity {

	struct UserTradeProfileGrpcConnection final
	{
		std::unique_ptr<usertradeprofile::TradeService::Stub> m_grpcStub;
		std::shared_ptr<grpc::Channel> m_grpcChannel;
		grpc::ClientContext m_context;
		std::string m_serverIpAddress;
		std::string m_serverPort;
		std::string m_serverConnection;
	};

	class BinanceTradeProfile final
	{
	public:
		BinanceTradeProfile(const tinyxml2::XMLElement* binanceTradeProfileXmlCfg);
		~BinanceTradeProfile();

		bool UpdateUserTradeProfileData(
			const std::string& userId,
			const double leverageRate,
			std::string& errorMessage);
	private:
		std::unique_ptr<LibraryUtils::Logger> m_logger;
		UserTradeProfileGrpcConnection m_grpcConnection;
	};
};
