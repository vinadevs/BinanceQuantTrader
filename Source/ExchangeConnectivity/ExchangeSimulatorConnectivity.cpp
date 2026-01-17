/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../KernelTrading/types.h"
#include "../OrderManagement/BinanceNewOrder.h"
#include "../OrderManagement/BinanceCancelOrder.h"
#include "../OrderManagement/BinanceReplaceOrder.h"
#include "../OrderManagement/BinanceQueryOrder.h"
#include "../LibraryUtils/StringDefinitions.h"
#include "../LibraryUtils/Logger.h"
#include "../SettingNConfig/tinyxml2.h"

#include "BinanceWalletClient.h"
#include "BinanceExchangeClient.h"
#include "BinanceTradeProfile.h"
#include "ExchangeSimulatorConnectivity.h"

using namespace ExchangeConnectivity;
using namespace OrderManagement;
using namespace tinyxml2;

ExchangeSimulatorConnectivity::ExchangeSimulatorConnectivity()
    : m_logger{ std::make_unique<LibraryUtils::Logger>("ExchangeSimulatorConnectivity") } {}

ExchangeSimulatorConnectivity::~ExchangeSimulatorConnectivity() {}

ExchangeSimulatorConnectivity* ExchangeSimulatorConnectivity::GetInstance()
{
    static ExchangeSimulatorConnectivity instance;
    return &instance;
}

void ExchangeSimulatorConnectivity::InitMessageTransporter(const XMLElement* messageDeliveryCfg)
{
    m_messageDelivery = std::make_unique<MiddlewareMQ::MessageDelivery>(messageDeliveryCfg);
}

void ExchangeSimulatorConnectivity::StopMessageTransporter()
{
    if (m_messageDelivery)
    {
        m_messageDelivery->GetCommunicationChannel()->StopChannel();
    }
}

void ExchangeSimulatorConnectivity::InitBinanceWalletClient(const XMLElement* binanceWalletClientXmlCfg)
{
    m_binanceWalletClient = std::make_unique<BinanceWalletClient>(binanceWalletClientXmlCfg);
}

void ExchangeSimulatorConnectivity::InitBinanceExchangeClient(const tinyxml2::XMLElement* binanceExchangeClientXmlCfg)
{
    m_binanceExchangeClient = std::make_unique<BinanceExchangeClient>(binanceExchangeClientXmlCfg);
}

void ExchangeSimulatorConnectivity::InitBinanceTradeProfile(const tinyxml2::XMLElement* binanceTradeProfileXmlCfg)
{
	m_binanceTradeProfile = std::make_unique<BinanceTradeProfile>(binanceTradeProfileXmlCfg);
}

MiddlewareMQ::MiddlewareMQResult
ExchangeSimulatorConnectivity::SendNewSimulatorOrderFull(
    BinanceNewOrder* newOrder)
{
    const auto& message = newOrder->ToBqtJsonMessageOrder();
    const auto newOrderResult = m_messageDelivery->DeliveryMessage(message);
    if (!newOrderResult.m_result)
    {
        LOG_ERROR_STREAM(m_logger, "could not place a new [SIMULATOR] order="
            << message << ", reason=" << newOrderResult.m_errMsg);
    }
    else
    {
        LOG_INFO_STREAM(m_logger, "successfully placed a new [SIMULATOR] order=" << message);
    }
    return newOrderResult;
}

MiddlewareMQ::MiddlewareMQResult
ExchangeSimulatorConnectivity::SendCancelSimulatorOrder(
    BinanceCancelOrder* cancelOrder)
{
    const auto& message = cancelOrder->ToBqtJsonMessageOrder();
    const auto cancelOrderResult = m_messageDelivery->DeliveryMessage(message);
    if (!cancelOrderResult.m_result)
    {
        LOG_ERROR_STREAM(m_logger, "could not cancel [SIMULATOR] order="
            << message << ", reason=" << cancelOrderResult.m_errMsg);
    }
    else
    {
        LOG_INFO_STREAM(m_logger, "successfully sent cancel [SIMULATOR] order=" << message);
    }
    return cancelOrderResult;
}

MiddlewareMQ::MiddlewareMQResult
ExchangeSimulatorConnectivity::SendCancelReplaceSimulatorOrder(
    BinanceReplaceOrder* replaceOrder)
{
    const auto& message = replaceOrder->ToBqtJsonMessageOrder();
    const auto replaceOrderResult = m_messageDelivery->DeliveryMessage(message);
    if (!replaceOrderResult.m_result)
    {
        LOG_ERROR_STREAM(m_logger, "could not replace [SIMULATOR] order="
            << message << ", reason=" << replaceOrderResult.m_errMsg);
    }
    else
    {
        LOG_INFO_STREAM(m_logger, "successfully sent replace [SIMULATOR] order=" << message);
    }
    return replaceOrderResult;
}

MiddlewareMQ::MiddlewareMQResult
ExchangeSimulatorConnectivity::QuerySimulatorOrderStatus(BinanceQueryOrder* queryOrder)
{
    const auto& message = queryOrder->ToBqtJsonMessageOrder();
    const auto queryOrderResult = m_messageDelivery->DeliveryMessage(message);
    if (!queryOrderResult.m_result)
    {
        LOG_ERROR_STREAM(m_logger, "could not query [SIMULATOR] order="
            << message << ", reason=" << queryOrderResult.m_errMsg);
    }
    else
    {
        LOG_INFO_STREAM(m_logger, "successfully sent query [SIMULATOR] order=" << message);
    }
    return queryOrderResult;
}

bool ExchangeSimulatorConnectivity::GetUserAccountInfo(
    const std::string& userId,
    binapi::rest::account_info_t* account,
    std::string& errorMessage)
{
    return m_binanceWalletClient->GetUserAccountDataResponse(userId, account, errorMessage);
}

bool ExchangeSimulatorConnectivity::GetUserFutureAccountInfo(
    const std::string& userId,
    KernelTrading::UserFutureAccount* userFutureAccount,
    std::string& errorMessage)
{
	return m_binanceWalletClient->GetUserFutureAccountDataResponse(userId, userFutureAccount, errorMessage);
}

bool ExchangeSimulatorConnectivity::GetExchangeInfo(
    const std::string& symbol,
    binapi::rest::exchange_info_t* exchangeInfo,
    std::string& errorMessage)
{
    return m_binanceExchangeClient->GetExchangeInfoResponse(symbol, exchangeInfo, errorMessage);
}

bool ExchangeSimulatorConnectivity::UpdateUserTradeProfileData(
    const std::string& userId,
    const double leverageRate,
    std::string& resultMessage)
{
	return m_binanceTradeProfile->UpdateUserTradeProfileData(userId, leverageRate, resultMessage);
}
