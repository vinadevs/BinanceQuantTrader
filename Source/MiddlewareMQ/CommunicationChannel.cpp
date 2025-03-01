/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "CommunicationChannel.h"

#include <zmq.h>
#include <stdexcept>
#include <iostream>

using namespace MiddlewareMQ;

////////////////////////////////////////////////////////////////////////////////

BrokerCommunicationChannel::BrokerCommunicationChannel(
	const std::string& topicName,
	const std::string& publisherAddress,
	const std::string& subscriberAddress)
	: CommunicationChannel(topicName, ApplicationType::BROKER)
	, m_publisherAddress(publisherAddress)
	, m_subscriberAddress(subscriberAddress)
{
	m_zeroMQContext = zmq_ctx_new();
	// Create the XPUB socket to forward messages to subscribers
	m_publisherSocket = zmq_socket(m_zeroMQContext, ZMQ_XPUB);
	// Create the XSUB socket to receive messages from publishers
	m_subscriberSocket = zmq_socket(m_zeroMQContext, ZMQ_XSUB);
}

BrokerCommunicationChannel::~BrokerCommunicationChannel()
{
	Cleanup();
}

void BrokerCommunicationChannel::Cleanup()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_publisherSocket)
		zmq_close(m_publisherSocket);
	if (m_subscriberSocket)
		zmq_close(m_subscriberSocket);
	if (m_zeroMQContext)
		zmq_ctx_destroy(m_zeroMQContext);
}

void BrokerCommunicationChannel::StopChannel()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	zmq_unbind(m_publisherSocket, m_publisherAddress.c_str());
	zmq_unbind(m_subscriberSocket, m_subscriberAddress.c_str());
	Cleanup();
}

void BrokerCommunicationChannel::StartChannel()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	// Bind sockets
	if (zmq_bind(m_publisherSocket, m_publisherAddress.c_str()) != 0) {
		zmq_close(m_publisherSocket);
		zmq_ctx_destroy(m_zeroMQContext);
		throw std::runtime_error("Error binding publisher to the socket: " 
			+ std::string(zmq_strerror(zmq_errno())));
	}
	if (zmq_bind(m_subscriberSocket, m_subscriberAddress.c_str()) != 0) {
		zmq_close(m_subscriberSocket);
		zmq_close(m_publisherSocket);
		zmq_ctx_destroy(m_zeroMQContext);
		throw std::runtime_error("Error binding subscriber to the socket: " 
			+ std::string(zmq_strerror(zmq_errno())));
	}
	// Use ZeroMQ's built-in proxy function to relay messages
	zmq_proxy(m_subscriberSocket, m_publisherSocket, NULL);
	// Clean up (This code should never be reached)
	Cleanup();
}

////////////////////////////////////////////////////////////////////////////////

PublisherCommunicationChannel::PublisherCommunicationChannel(
	const std::string& topicName,
	const std::string& publisherAddress)
	: CommunicationChannel(topicName, ApplicationType::PUBLISHER)
	, m_publisherAddress(publisherAddress)
{
	m_zeroMQContext = zmq_ctx_new();
	m_publisherSocket = zmq_socket(m_zeroMQContext, ZMQ_PUB);
}

PublisherCommunicationChannel::~PublisherCommunicationChannel()
{
	Cleanup();
}

void PublisherCommunicationChannel::Cleanup()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	// TODO: use TranslateResult to check api call result
	if (m_publisherSocket)
		zmq_close(m_publisherSocket);
	if (m_zeroMQContext)
		zmq_ctx_destroy(m_zeroMQContext);
}

void PublisherCommunicationChannel::StopChannel()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	zmq_unbind(m_publisherSocket, m_publisherAddress.c_str());
	Cleanup();
}

void PublisherCommunicationChannel::StartChannel()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	// Try connecting publisher to the endpoint
	if (zmq_connect(m_publisherSocket, m_publisherAddress.c_str()) != 0) {
		zmq_close(m_publisherSocket);
		zmq_ctx_destroy(m_zeroMQContext);
		throw std::runtime_error("Error connecting publisher to the endpoint: " + std::string(zmq_strerror(zmq_errno())));
	}
}

////////////////////////////////////////////////////////////////////////////////

SubcriberCommunicationChannel::SubcriberCommunicationChannel(
	const std::string& topicName,
	const std::string& subscriberAddress)
	: CommunicationChannel(topicName, ApplicationType::PUBLISHER)
	, m_subscriberAddress(subscriberAddress)
{
	m_zeroMQContext = zmq_ctx_new();
	// Create the XSUB socket to receive messages from publishers
	m_subscriberSocket = zmq_socket(m_zeroMQContext, ZMQ_SUB);
	// Subscribe to all topics
	zmq_setsockopt(m_subscriberSocket, ZMQ_SUBSCRIBE, "", 0);
}

SubcriberCommunicationChannel::~SubcriberCommunicationChannel()
{
	Cleanup();
}

void SubcriberCommunicationChannel::Cleanup()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_subscriberSocket)
		zmq_close(m_subscriberSocket);
	if (m_zeroMQContext)
		zmq_ctx_destroy(m_zeroMQContext);
}

void SubcriberCommunicationChannel::StopChannel()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	zmq_unbind(m_subscriberSocket, m_subscriberAddress.c_str());
	Cleanup();
}

void SubcriberCommunicationChannel::StartChannel()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (zmq_connect(m_subscriberSocket, m_subscriberAddress.c_str()) != 0) {
		perror("Failed to bind XSUB socket");
		zmq_close(m_subscriberSocket);
		zmq_ctx_destroy(m_zeroMQContext);
		throw std::runtime_error("Error binding subscriber to the socket: " + std::string(zmq_strerror(zmq_errno())));
	}
}