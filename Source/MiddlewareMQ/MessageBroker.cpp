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
#include "MessageBroker.h"

#include <zmq.h>
#include <stdexcept>
#include <cassert>

using namespace MiddlewareMQ;
using namespace tinyxml2;

MessageBroker::MessageBroker(const std::string& configFile)
    : m_logger{ std::make_unique<LibraryUtils::Logger>("MessageBroker") }
{
    CreateBrokers(configFile);
}

MessageBroker::~MessageBroker()
{
    Stop();
}

void MessageBroker::CreateBrokers(const std::string& configFile)
{
    m_logger->Info("Loading xml config files.");
    auto configMBXml = std::make_unique<tinyxml2::XMLDocument>();
    const auto errMBXml = configMBXml->LoadFile(configFile.c_str());
    if (errMBXml != tinyxml2::XML_SUCCESS)
    {
        throw std::runtime_error("Load file Xml error: "
            + std::string(tinyxml2::XMLDocument::ErrorIDToName(errMBXml)) + ", error path:" + configFile);
    }
    const auto* channelXml = configMBXml->FirstChildElement("Brokers");
    assert(channelXml);

    const auto* exchangeSimulatorToAlgosXml = channelXml->FirstChildElement("ExchangeSimulatorToAlgos");
    assert(exchangeSimulatorToAlgosXml);
    const auto* topicNameSimulator = exchangeSimulatorToAlgosXml->Attribute("TopicName");
    const auto* publisherAddressSimulator = exchangeSimulatorToAlgosXml->Attribute("PublisherAddress");
    const auto* subscriberAddressSimulator = exchangeSimulatorToAlgosXml->Attribute("SubscriberAddress");
    m_threadPools.emplace_back(
        std::thread(&MessageBroker::CreateCommunicationChannel,
            this, std::make_shared<ChannelConfig>(
                topicNameSimulator, 
                publisherAddressSimulator,
                subscriberAddressSimulator)));

    const auto* algosToExchangeSimulatorXml = channelXml->FirstChildElement("AlgosToExchangeSimulator");
    assert(algosToExchangeSimulatorXml);
    const auto* topicNameAlgos = algosToExchangeSimulatorXml->Attribute("TopicName");
    const auto* publisherAddressAlgos = algosToExchangeSimulatorXml->Attribute("PublisherAddress");
    const auto* subscriberAddressAlgos = algosToExchangeSimulatorXml->Attribute("SubscriberAddress");
    m_threadPools.emplace_back(
        std::thread(&MessageBroker::CreateCommunicationChannel,
            this, std::make_shared<ChannelConfig>(
                topicNameAlgos,
                publisherAddressAlgos,
                subscriberAddressAlgos)));
    
	const auto* marketDataCaptureToPythonClient = channelXml->FirstChildElement("MarketDataCaptureToPythonClient");
	assert(marketDataCaptureToPythonClient);
	const auto* topicNameMDC = marketDataCaptureToPythonClient->Attribute("TopicName");
	const auto* publisherAddressMDC = marketDataCaptureToPythonClient->Attribute("PublisherAddress");
	const auto* subscriberAddressMDC = marketDataCaptureToPythonClient->Attribute("SubscriberAddress");
	m_threadPools.emplace_back(
		std::thread(&MessageBroker::CreateCommunicationChannel,
			this, std::make_shared<ChannelConfig>(
				topicNameMDC,
				publisherAddressMDC,
				subscriberAddressMDC)));
}

void MessageBroker::Run()
{
    JoinThreads();
}

void MessageBroker::Stop()
{
    for (auto& channel : m_channels)
    {
        channel.second->StopChannel();
    }
    JoinThreads();
}

void MessageBroker::JoinThreads()
{
    for (auto& broker : m_threadPools)
    {
        if (broker.joinable())
        {
            broker.join();
        }
    }
}

void MessageBroker::CreateCommunicationChannel(std::shared_ptr<ChannelConfig> channelConfig)
{
    // Initialize the ZeroMQ context
    auto [chanel, result] = m_channels.try_emplace(channelConfig->m_topicName,
        std::make_unique<BrokerCommunicationChannel>(
            channelConfig->m_topicName,
            channelConfig->m_publisherAddress,
            channelConfig->m_subscriberAddress));
    if (result) 
    {
        LOG_INFO_STREAM(m_logger, "New broker created: topicName=" << channelConfig->m_topicName
            << ", channelID=" << chanel->second->m_channelID);
    }
    else 
    {
        LOG_INFO_STREAM(m_logger, "Broker is already existed: topicName=" << channelConfig->m_topicName);
    }
    LOG_INFO_STREAM(m_logger, "Broker running: topicName=" << channelConfig->m_topicName
        << "XSUB bound to address=" << channelConfig->m_subscriberAddress 
        << ", XPUB bound to address=" << channelConfig->m_publisherAddress);
    chanel->second->StartChannel(); // this is a WAIT loop call
}

// Lookup an order by clientOrderId
BrokerCommunicationChannel* MessageBroker::LookupChannel(const std::string& topicName) const {
    auto it = m_channels.find(topicName);
    if (it != m_channels.end()) 
    {
        return it->second.get(); // Return the raw pointer
    }
    LOG_WARNING_STREAM(m_logger, "Error: No chanel found with topicName=" << topicName << "'.");
    return nullptr; // Return nullptr if not found
}
