/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/Logger.h"

#include "MessageDelivery.h"

#include <zmq.h>
#include <stdexcept>

using namespace MiddlewareMQ;
using namespace tinyxml2;

MessageDelivery::MessageDelivery(const tinyxml2::XMLElement* messageDeliveryCfg)
	: m_logger{ std::make_unique<LibraryUtils::Logger>("MessageDelivery") }
{
    const auto* messageDeliveryXml = messageDeliveryCfg->FirstChildElement("CommunicationChannel");
    assert(messageDeliveryXml);
    const auto* topicName = messageDeliveryXml->Attribute("TopicName");
    assert(topicName);
    const auto* publisherAddress = messageDeliveryXml->Attribute("PublisherAddress");
    assert(publisherAddress);
    CreateCommunicationChannel(topicName, publisherAddress);
}

MiddlewareMQResult MessageDelivery::DeliveryMessage(const BqtJsonMessage& message)
{
	std::lock_guard<std::mutex> lock(m_zmqMutex); // Ensure thread safety
    if (m_isServerSideReady)
    {
        zmq_api_result result = zmq_send(
            m_chanel->m_publisherSocket,
            m_chanel->m_topicName.c_str(),
            m_chanel->m_topicName.length(),
            ZMQ_SNDMORE); // Send topic

        const auto rawMessage = message.SerializeMessage();
        result = zmq_send(m_chanel->m_publisherSocket,
            rawMessage.c_str(),
            rawMessage.length(),
            ZMQ_NULL); // Send message

        LOG_INFO_STREAM(m_logger, "Published: topic="
            << m_chanel->m_topicName << ", message=" << rawMessage);
        // Check the result
        return TranslateResult(result);
    }
    else
    {
        return MiddlewareMQResult("Backtest simulator Json server is not available now.", false);
    }
}

void MessageDelivery::CreateCommunicationChannel(
    const std::string& topicName,
    const std::string& publisherAddress)
{
    m_chanel = std::make_unique<PublisherCommunicationChannel>(topicName, publisherAddress);
    LOG_INFO_STREAM(m_logger, "Publisher running: topicName=" << topicName 
        << ", channelID=" << m_chanel->m_channelID
        << ", XPUB bound to address=" << publisherAddress);
    m_chanel->StartChannel(); // this is a NO wait call
}
