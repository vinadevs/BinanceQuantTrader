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

#include "MessageReceiver.h"

#include <zmq.h>
#include <stdexcept>

using namespace MiddlewareMQ;
using namespace tinyxml2;

static constexpr int InvalidSize = -1;

MessageReceiver::MessageReceiver(const tinyxml2::XMLElement* messageReceiverCfg)
    : m_logger{ std::make_unique<LibraryUtils::Logger>("MessageReceiver") }
{
    const auto* messageReceiverXml = messageReceiverCfg->FirstChildElement("CommunicationChannel");
    assert(messageReceiverXml);
    const auto* topicName = messageReceiverXml->Attribute("TopicName");
    assert(topicName);
    const auto* subscriberAddress = messageReceiverXml->Attribute("SubscriberAddress");
    assert(subscriberAddress);
    CreateCommunicationChannel(topicName, subscriberAddress);
}

MessageReceiver::~MessageReceiver()
{
    Terminate();
}

void MessageReceiver::Run()
{
    // start worker thread for receiver chanel
    m_isRunning.store(true);
    m_channelThread = std::thread(&MessageReceiver::WaitForIncomingMessage, this);
    m_channelThread.detach();
}

void MessageReceiver::Terminate()
{
    m_isRunning.store(false);
    // wait sometime for detached thread is totally exited
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void MessageReceiver::WaitForIncomingMessage()
{
    while (m_isRunning.load()) 
    {
        zmq_msg_t topicMsg, messageMsg;

        zmq_msg_init(&topicMsg);
        zmq_msg_init(&messageMsg);

        // Receive topic
        const int topicSize = zmq_msg_recv(&topicMsg, m_chanel->m_subscriberSocket, ZMQ_NULL);
        if (topicSize == InvalidSize) {
            LOG_ERROR_STREAM(m_logger, "Error receiving topic="
                << std::string(zmq_strerror(zmq_errno())));
            break;
        }

        // Receive message
        const int messageSize = zmq_msg_recv(&messageMsg, m_chanel->m_subscriberSocket, ZMQ_NULL);
        if (messageSize == InvalidSize) {
            LOG_ERROR_STREAM(m_logger, "Error receiving message="
                << std::string(zmq_strerror(zmq_errno())));
            break;
        }

        // Convert received topic and message to std::string
        const std::string topicStr(static_cast<char*>(
            zmq_msg_data(&topicMsg)), zmq_msg_size(&topicMsg));
        const std::string messageStr(static_cast<char*>(
            zmq_msg_data(&messageMsg)), zmq_msg_size(&messageMsg));

		// Validate the received topic and message
        if (IsValidMessage(topicStr, messageStr)) {
           // Null-terminate the strings if needed (optional)
           // Typically not needed if using the length from zmq_msg_size()

           // Debug the received topic and message
           //LOG_DEBUG_STREAM(m_logger, "Received Topic: " << topicStr);
           //LOG_DEBUG_STREAM(m_logger, "Received Message: " << messageStr);

           // pass the message in BqtJsonMessage format to derived class
           OnReceivedMessage(BqtJsonMessage(messageStr));
        }

        // Clean up
        zmq_msg_close(&topicMsg);
        zmq_msg_close(&messageMsg);
    }
}

void MessageReceiver::CreateCommunicationChannel(
    const std::string& topicName,
    const std::string& subscriberAddress)
{
	m_topicName = topicName;
    m_chanel = std::make_unique<SubcriberCommunicationChannel>(topicName, subscriberAddress);
    LOG_INFO_STREAM(m_logger, "Subcriber running: topicName=" << topicName
        << ", channelID=" << m_chanel->m_channelID
        << ", XSUB bound to address=" << subscriberAddress);
    m_chanel->StartChannel(); // this is a NO wait call
}

bool MessageReceiver::IsValidMessage(const std::string& topic, const std::string& message) const
{
	// Check if the topic and message are not empty
	if (topic.empty() || message.empty()) {
        LOG_ERROR_STREAM(m_logger, "Received empty topic or message. Topic: " << topic << ", Message: " << message);
		return false;
	}
	// Check if the topic and message are same
    if (topic == message) {
        LOG_ERROR_STREAM(m_logger, "Received topic is same with message. Topic: " << topic << ", Message: " << message);
        return false;
    }
	// Check if the topic is valid (e.g., matches a specific pattern or format)
	if (topic != m_topicName) {
		LOG_ERROR_STREAM(m_logger, "Received topic does not match expected topic. Expected: " << m_topicName << ", Received: " << topic);
		return false;
	}
	return true; // If all checks pass, the message is considered valid
}
