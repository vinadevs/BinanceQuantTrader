/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "ExchangeInfoManager.h"
#include "ExchangeInfoHttpService.h"
#include "ExchangeRuleAndCompliance.h"

#include "../LibraryUtils/Logger.h"

using namespace ExchangeSimulator;

ExchangeInfoHttpService::ExchangeInfoHttpService(ExchangeInfoManager* exchangeInfoManager)
    : m_logger{ std::make_unique<LibraryUtils::Logger>("ExchangeInfoHttpService") },
      m_exchangeInfoManager(exchangeInfoManager) {}

ExchangeInfoHttpService::~ExchangeInfoHttpService() {}

grpc::Status ExchangeInfoHttpService::GetExchangeInfo(
    grpc::ServerContext* context,
    const exchange::ExchangeInfoRequest* request,
    exchange::ExchangeInfoResponse* response)
{
    m_logger->Info("Received exchange_info_t request for User ID=" + request->symbol_id());

	const auto& exchangeInfo = m_exchangeInfoManager->GetExchangeInfo();

    return grpc::Status::OK;
}
