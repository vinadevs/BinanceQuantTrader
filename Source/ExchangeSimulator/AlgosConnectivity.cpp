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
#include "../OrderManagement/FieldLabels.h"
#include "../LibraryUtils/StringDefinitions.h"
#include "../LibraryUtils/Logger.h"
#include "../SettingNConfig/tinyxml2.h"

#include "OrderAckManager.h"
#include "AlgosConnectivity.h"

using namespace ExchangeSimulator;
using namespace OrderManagement;
using namespace MiddlewareMQ;
using namespace tinyxml2;

AlgosConnectivity::AlgosConnectivity()
    : m_logger{ std::make_unique<LibraryUtils::Logger>("AlgosConnectivity") },
      m_orderAckManager{ std::make_unique<OrderAckManager>() } {}

AlgosConnectivity::~AlgosConnectivity() {}

AlgosConnectivity* AlgosConnectivity::GetInstance()
{
    static AlgosConnectivity instance;
    return &instance;
}

void AlgosConnectivity::InitMessageTransporter(const XMLElement* messageDeliveryCfg)
{
    m_messageDelivery = std::make_unique<MessageDelivery>(messageDeliveryCfg);
}

void AlgosConnectivity::StopMessageTransporter()
{
    if (m_messageDelivery)
    {
        m_messageDelivery->GetCommunicationChannel()->StopChannel();
    }
}

MiddlewareMQResult AlgosConnectivity::SendDownstreamOrderAck(const DownstreamOrderAck& ack)
{
    const auto newOrderResult = m_messageDelivery->DeliveryMessage(ack);
    if (!newOrderResult.m_result)
    {
        LOG_ERROR_STREAM(m_logger, "could not delivery a new ack=" 
            << ack << ", reason=" << newOrderResult.m_errMsg);
    }
    else
    {
        LOG_INFO_STREAM(m_logger, "sent a new ack=" << ack);
        m_orderAckManager->AddNewAck(ack.GetStringValueByTag(FieldLabels::ClientOrderId), ack);
    }
    return newOrderResult;
}