/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "UserAccountManager.h"
#include "ExchangeInfoHttpService.h"
#include "ExchangeRuleAndCompliance.h"

#include "../LibraryUtils/Logger.h"

using namespace ExchangeSimulator;

ExchangeInfoHttpService::ExchangeInfoHttpService(UserAccountManager* userAccountManager)
    : m_logger{ std::make_unique<LibraryUtils::Logger>("ExchangeInfoHttpService") }
    , m_userAccountManager(userAccountManager) {}

ExchangeInfoHttpService::~ExchangeInfoHttpService() {}

grpc::Status ExchangeInfoHttpService::GetExchangeInfo(
    grpc::ServerContext* context,
    const exchange::ExchangeInfoRequest* request,
    exchange::ExchangeInfoResponse* response)
{
    return grpc::Status();
}

