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
#include "protobuf/user_future_account_data.pb.h"
#include "protobuf/user_future_account_data.grpc.pb.h"

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

namespace KernelTrading {
	class UserFutureAccount;
};

namespace ExchangeConnectivity {

	struct UserAccountGrpcConnection final
	{
		std::unique_ptr<account::UserAccountService::Stub> m_grpcStubSpotAccount;
		std::unique_ptr<futureaccount::UserAccountService::Stub> m_grpcStubFutureAccount;
		std::shared_ptr<grpc::Channel> m_grpcChannel;
		grpc::ClientContext m_context;
		std::string m_serverIpAddress;
		std::string m_serverPort;
		std::string m_serverConnection;
	};

	// This class is responsible for communicating with the Binance Wallet gRPC service
	// to retrieve spot and future account data from exchange simulator.
	class BinanceWalletClient final
	{
	public:
		BinanceWalletClient(const tinyxml2::XMLElement* binanceWalletClientXmlCfg);
		~BinanceWalletClient();

		bool GetUserAccountDataResponse(
			const std::string& userId,
			binapi::rest::account_info_t* account,
			std::string& errorMessage);

		bool GetUserFutureAccountDataResponse(
			const std::string& userId,
			KernelTrading::UserFutureAccount* account,
			std::string& errorMessage);
	private:
		std::unique_ptr<LibraryUtils::Logger> m_logger;
		UserAccountGrpcConnection m_grpcConnectionSpotAccount;
		UserAccountGrpcConnection m_grpcConnectionFutureAccount;
	};
};
