/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../LibraryUtils/Logger.h"
#include "../SettingNConfig/tinyxml2.h"

#include "PythonClientConnectivity.h"

using namespace PythonPlugin;
using namespace tinyxml2;

PythonClientConnectivity::PythonClientConnectivity()
    : m_logger{ std::make_unique<LibraryUtils::Logger>("PythonClientConnectivity") } {}

PythonClientConnectivity::~PythonClientConnectivity() {}

PythonClientConnectivity* PythonClientConnectivity::GetInstance()
{
    static PythonClientConnectivity instance;
    return &instance;
}

void PythonClientConnectivity::InitMessageTransporter(const XMLElement* messageDeliveryCfg)
{
    m_messageDelivery = std::make_unique<MiddlewareMQ::MessageDelivery>(messageDeliveryCfg);
}

void PythonClientConnectivity::StopMessageTransporter()
{
    if (m_messageDelivery)
    {
        m_messageDelivery->GetCommunicationChannel()->StopChannel();
    }
}

MiddlewareMQ::MiddlewareMQResult
PythonClientConnectivity::SendBqtJsonMessage(const MiddlewareMQ::BqtJsonMessage& message)
{
    const auto newOrderResult = m_messageDelivery->DeliveryMessage(message);
    if (!newOrderResult.m_result)
    {
        LOG_ERROR_STREAM(m_logger, "could not place a message="
            << message << ", reason=" << newOrderResult.m_errMsg);
    }
    else
    {
        //LOG_DEBUG_STREAM(m_logger, "successfully placed a messager=" << message);
    }
    return newOrderResult;
}
