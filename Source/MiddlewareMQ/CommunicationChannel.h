/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#ifndef COMMUNICATION_CHANEL_H
#define COMMUNICATION_CHANEL_H

#include <string>
#include <mutex>

#include "../LibraryUtils/GeneralUtils.h"

#include "dlldefine.h"

namespace tinyxml2 {
    class XMLElement;
};

namespace MiddlewareMQ {

    // Pub-Sub Model Apps
    enum class ApplicationType : unsigned
    {
        UNDEF,
        BROKER, // replay messages
        PUBLISHER, // send messages
        SUBCRIBER, // receive messages
    };

    /////////////////////////////////////////////////////////////////////

    /* A chanel is the definition of the network connection
    between broker and clients, NOTE: ports should be unique! */

    using ZeroMQHandle = void*;

    struct DLL_CLASS_MIDDLEWAREMQ_EXPORTS CommunicationChannel
    {
        CommunicationChannel(
            const std::string& topicName,
            const ApplicationType appType) 
            : m_channelID(GeneralUtils::GenerateUniqueID(topicName)),
              m_topicName(topicName),
              m_appType(appType) {}
        virtual ~CommunicationChannel() {}

        virtual void Cleanup() = 0;
        virtual void StartChannel() = 0;
        virtual void StopChannel() = 0;

        std::string m_channelID;
        std::string m_topicName;
        ApplicationType m_appType{ ApplicationType::UNDEF };
        ZeroMQHandle m_zeroMQContext{ nullptr };
        std::mutex m_mutex; // Thread safe
    };

    /////////////////////////////////////////////////////////////////////

    struct DLL_CLASS_MIDDLEWAREMQ_EXPORTS BrokerCommunicationChannel final
        : public CommunicationChannel
    {
        BrokerCommunicationChannel(
            const std::string& topicName,
            const std::string& publisherAddress,
            const std::string& subscriberAddress);
        ~BrokerCommunicationChannel();

        void Cleanup() override;
        void StartChannel() override;
        void StopChannel() override;

        std::string m_publisherAddress;
        std::string m_subscriberAddress;
        ZeroMQHandle m_publisherSocket{ nullptr };
        ZeroMQHandle m_subscriberSocket{ nullptr };
    };

    /////////////////////////////////////////////////////////////////////

    struct DLL_CLASS_MIDDLEWAREMQ_EXPORTS PublisherCommunicationChannel final
        : public CommunicationChannel
    {
        PublisherCommunicationChannel(
            const std::string & topicName,
            const std::string & publisherAddress);
        ~PublisherCommunicationChannel();

        void Cleanup() override;
        void StartChannel() override;
        void StopChannel() override;

        std::string m_publisherAddress;
        ZeroMQHandle m_publisherSocket{ nullptr };
    };

    /////////////////////////////////////////////////////////////////////

    struct DLL_CLASS_MIDDLEWAREMQ_EXPORTS SubcriberCommunicationChannel final
        : public CommunicationChannel
    {
        SubcriberCommunicationChannel(
            const std::string & topicName,
            const std::string & subscriberAddress);
        ~SubcriberCommunicationChannel();

        void Cleanup() override;
        void StartChannel() override;
        void StopChannel() override;

        std::string m_subscriberAddress;
        ZeroMQHandle m_subscriberSocket{ nullptr };
    };
};

#endif // COMMUNICATION_CHANEL_H
