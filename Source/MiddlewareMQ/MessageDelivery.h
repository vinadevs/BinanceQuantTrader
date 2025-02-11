/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#ifndef MESSAGE_DELIVERY_H
#define MESSAGE_DELIVERY_H

#include <memory>

#include "dlldefine.h"
#include "CommunicationChannel.h"
#include "BqtJsonMessage.h"
#include "ZeroMQUtils.h"

namespace tinyxml2 {
    class XMLElement;
};

namespace LibraryUtils {
    class Logger;
};

namespace MiddlewareMQ {

    /* This class that provides basic message-sending 
    capabilities using ZeroMQ. This class define a transport layer
    where messages can be published to, using ZeroMQ sockets. */

    struct PublisherCommunicationChannel;

    class DLL_CLASS_MIDDLEWAREMQ_EXPORTS MessageDelivery final
    {
    public:
        MessageDelivery(const tinyxml2::XMLElement* messageDeliveryCfg);
        // Deliver Message APIs
        MiddlewareMQResult DeliveryMessage(const BqtJsonMessage& message);
    private:
        void CreateCommunicationChannel(
            const std::string& topicName,
            const std::string& publisherAddress);

        std::unique_ptr<PublisherCommunicationChannel> m_chanel;
        std::unique_ptr<LibraryUtils::Logger> m_logger;
    };
};

#endif // MESSAGE_DELIVERY_H
