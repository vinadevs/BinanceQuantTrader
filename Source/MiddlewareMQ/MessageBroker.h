/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#ifndef MESSAGE_BROKER_H
#define MESSAGE_BROKER_H

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <thread>

#include "dlldefine.h"
#include "CommunicationChannel.h"

namespace LibraryUtils {
    class Logger;
};

namespace MiddlewareMQ {

    /* In a Pub - Sub(Publish - Subscribe) messaging model using ZeroMQ (C API), 
    we typically have a broker(server) and multiple clients(subscribers and publishers).
    - This broker is responsible for receiving messages from the 
    publishers and distributing them to the subscribers based on 
    the chanels/topics they are interested in. */

    // MessageDelivery -> BqtJsonMessage ->
    // MessageBroker -> BqtJsonMessage -> MessageReceiver
    
    // Each application want to send/receive message have to implement both 
    // MessageDelivery and MessageReceiver classes

    // This class can handle multiple communication channels via external config file
    struct BrokerCommunicationChannel;
    class DLL_CLASS_MIDDLEWAREMQ_EXPORTS MessageBroker final
    {
    public:
        MessageBroker(const std::string& configFile);
        ~MessageBroker();

        void Run();
        void Stop();
    private:
        void JoinThreads();

        // We use this struct to share data with threads from the main thread
        struct ChannelConfig final
        {
            ChannelConfig(const std::string& topicName,
                          const std::string& publisherAddress,
                          const std::string& subscriberAddress)
                        : m_topicName(topicName),
                          m_publisherAddress(publisherAddress),
                          m_subscriberAddress(subscriberAddress) {}
            std::string m_topicName;
            std::string m_publisherAddress;
            std::string m_subscriberAddress;
        };

        void CreateCommunicationChannel(std::shared_ptr<ChannelConfig> channelConfig);

        void CreateBrokers(const std::string& configFile);
        BrokerCommunicationChannel* LookupChannel(const std::string& topicName) const;

        std::map<std::string, std::unique_ptr<BrokerCommunicationChannel>> m_channels;
        std::vector<std::thread> m_threadPools;
        std::unique_ptr<LibraryUtils::Logger> m_logger;
    };
};

#endif // MESSAGE_BROKER_H
