/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../OrderManagement/BinanceNewOrder.h"
#include "../OrderManagement/BinanceCancelOrder.h"
#include "../OrderManagement/BinanceReplaceOrder.h"
#include "../LibraryUtils/StringDefinitions.h"
#include "../LibraryUtils/Logger.h"
#include "../SettingNConfig/tinyxml2.h"

#include "BinanceWalletClient.h"
#include "ExchangeSimulatorConnectivity.h"

using namespace ExchangeConnectivity;
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

void ExchangeSimulatorConnectivity::InitBinanceWalletClient(const XMLElement* binanceWalletClientXmlCfg)
{
    m_binanceWalletClient = std::make_unique<BinanceWalletClient>(binanceWalletClientXmlCfg);
}

MiddlewareMQ::MiddlewareMQResult
ExchangeSimulatorConnectivity::SendNewSimulatorOrderFull(
    OrderManagement::BinanceNewOrder* newOrder)
{
    const auto& message = newOrder->ToBqtJsonMessage();
    const auto newOrderResult = m_messageDelivery->DeliveryMessage(message);
    if (!newOrderResult.m_result)
    {
        LOG_ERROR_STREAM(m_logger, "could not place a new order="
            << message << ", reason=" << newOrderResult.m_errMsg);
    }
    else
    {
        LOG_INFO_STREAM(m_logger, "placed a new order=" << message);
    }
    return newOrderResult;
}

MiddlewareMQ::MiddlewareMQResult
ExchangeSimulatorConnectivity::QuerySimulatorOrderStatus(const std::string& symbol)
{
    return MiddlewareMQ::MiddlewareMQResult("", true);
}

MiddlewareMQ::MiddlewareMQResult
ExchangeSimulatorConnectivity::SendCancelSimulatorOrder(
    OrderManagement::BinanceCancelOrder* cancelOrder)
{
    const auto& message = cancelOrder->ToBqtJsonMessage();
    const auto cancelOrderResult = m_messageDelivery->DeliveryMessage(message);
    if (!cancelOrderResult.m_result)
    {
        LOG_ERROR_STREAM(m_logger, "could not cancel order="
            << message << ", reason=" << cancelOrderResult.m_errMsg);
    }
    else
    {
        LOG_INFO_STREAM(m_logger, "cancelled order=" << message);
    }
    return cancelOrderResult;
}

MiddlewareMQ::MiddlewareMQResult
ExchangeSimulatorConnectivity::SendCancelReplaceSimulatorOrder(
    OrderManagement::BinanceReplaceOrder* replaceOrder)
{
    const auto& message = replaceOrder->ToBqtJsonMessage();
    const auto replaceOrderResult = m_messageDelivery->DeliveryMessage(message);
    if (!replaceOrderResult.m_result)
    {
        LOG_ERROR_STREAM(m_logger, "could not cancel order="
            << message << ", reason=" << replaceOrderResult.m_errMsg);
    }
    else
    {
        LOG_INFO_STREAM(m_logger, "cancelled order=" << message);
    }
    return replaceOrderResult;
}

void ExchangeSimulatorConnectivity::GetUserAccountData(const std::string& user_id) 
{
    m_binanceWalletClient->GetUserAccountDataResponse(user_id);
}