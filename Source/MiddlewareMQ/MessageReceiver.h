/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#ifndef MESSAGE_RECEIVER_H
#define MESSAGE_RECEIVER_H

#include <memory>
#include <thread>
#include <mutex>
#include <atomic>

#include "dlldefine.h"
#include "CommunicationChannel.h"
#include "BqtJsonMessage.h"

namespace tinyxml2 {
    class XMLElement;
};

namespace MiddlewareMQ {

    /* This class that provides basic message-receiving
    capabilities using ZeroMQ. This class define a transport layer
    where messages can be subscribed to, using ZeroMQ sockets. */

    struct SubcriberCommunicationChannel;
    class  MessageHandler;
    class DLL_CLASS_MIDDLEWAREMQ_EXPORTS MessageReceiver
    {
    public:
        MessageReceiver(const tinyxml2::XMLElement* messageReceiverCfg);
        virtual ~MessageReceiver();

        // Receive Message APIs, derived class has to override this method
        // to receive messages from publishers
        virtual void OnReceivedMessage(const BqtJsonMessage& message) = 0;
        // Register the class that will handler received message
        virtual void RegisterMessageHandler(MessageHandler* handler) = 0;

        void Run();
        void Terminate();
    private:
        void WaitForIncomingMessage();
        void CreateCommunicationChannel(
            const std::string& topicName,
            const std::string& subscriberAddress);
		bool IsValidMessage(const std::string& topic, const std::string& message) const;

		std::string m_topicName; // Topic name for this receiver
        std::unique_ptr<SubcriberCommunicationChannel> m_chanel;
        std::unique_ptr<LibraryUtils::Logger> m_logger;
        std::atomic<bool> m_isRunning {false}; // Non blocking thread
        std::thread m_channelThread;
    };
};

#endif // MESSAGE_RECEIVER_H
