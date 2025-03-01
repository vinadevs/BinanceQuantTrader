/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "Protobuf/user_account_data.pb.h"
#include "Protobuf/user_account_data.grpc.pb.h"
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

namespace binapi {
	namespace rest {
		struct account_info_t;
	};
};

namespace ExchangeConnectivity {

	struct GrpcConnection final
	{
		std::unique_ptr<UserAccountService::Stub> m_grpcStub;
		std::shared_ptr<grpc::Channel> m_grpcChannel;
		grpc::ClientContext m_context;
		std::string m_serverIpAddress;
		std::string m_serverPort;
		std::string m_serverConnection;
	};

	class BinanceWalletClient final
	{
	public:
		BinanceWalletClient(const tinyxml2::XMLElement* binanceWalletClientXmlCfg);
		~BinanceWalletClient();

		bool GetUserAccountDataResponse(
			const std::string& userId,
			binapi::rest::account_info_t* account,
			std::string& errorMessage);
	private:
		std::unique_ptr<LibraryUtils::Logger> m_logger;
		GrpcConnection m_grpcConnection;
	};
};
