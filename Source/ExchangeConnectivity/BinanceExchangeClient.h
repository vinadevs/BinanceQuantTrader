/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "Protobuf/exchange_info_data.pb.h"
#include "Protobuf/exchange_info_data.grpc.pb.h"
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

	struct ExchangeGrpcConnection final
	{
		std::unique_ptr<exchange::ExchangeService::Stub> m_grpcStub;
		std::shared_ptr<grpc::Channel> m_grpcChannel;
		grpc::ClientContext m_context;
		std::string m_serverIpAddress;
		std::string m_serverPort;
		std::string m_serverConnection;
	};

	class BinanceExchangeClient final
	{
	public:
		BinanceExchangeClient(const tinyxml2::XMLElement* BinanceExchangeClientXmlCfg);
		~BinanceExchangeClient();

		bool GetExchangeInfoResponse(
			const std::string& symbol,
			binapi::rest::exchange_info_t*
			exchangeInfo, std::string& errorMessage);
	private:
		std::unique_ptr<LibraryUtils::Logger> m_logger;
		ExchangeGrpcConnection m_grpcConnection;
	};
};
