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
#include "../LibraryUtils/TimeUtils.h"
#include "../LibraryUtils/StringUtils.h"
#include "../OrderManagement/FieldLabels.h"
#include "../MarketData/RealTimeMarketData.h"

#include "MatchingEngine.h"
#include "AlgosConnectivity.h"
#include "UpstreamOrderQueueMgr.h"
#include "UpstreamOrderMatchedMgr.h"

#include "Participant.h"
#include "RTMarketParticipant.h"
#include "HistoricalParticipant.h"
#include "SimulatorParticipant.h"

using namespace ExchangeSimulator;
using namespace MiddlewareMQ;
using namespace OrderManagement;
using namespace MarketData;
using namespace binapi::rest;

#define WAIT_TIME_OUT std::chrono::seconds(1)

#define ZERO_DOUBLE_VALUE 0

MatchingEngine::MatchingEngine(
	const tinyxml2::XMLElement* matchingEngineXmlCfg,
	UserAccountManager* userAccountManager)
	: m_logger{ std::make_unique<LibraryUtils::Logger>("MatchingEngine") },
	  m_upstreamOrderQueueMgr{ std::make_unique<UpstreamOrderQueueMgr>() },
	  m_upstreamOrderMatchedMgr{ std::make_unique<UpstreamOrderMatchedMgr>() },
	  m_userAccountManager(userAccountManager)
{
	assert(matchingEngineXmlCfg);
	const auto* usingParticipantXml = matchingEngineXmlCfg->FirstChildElement("UsingParticipant");
	assert(usingParticipantXml);
	if (StringUtils::IsConfigAttributeMatched(usingParticipantXml->Attribute("Name"), "RTMarketParticipant"))
	{
		m_logger->Info("Initiating RTMarketParticipant.");
		const auto maxDownstreamOrderBookSize = usingParticipantXml->UnsignedAttribute("MaximumDownstreamOrderBookSize");
		m_participant = std::make_unique<RTMarketParticipant>(maxDownstreamOrderBookSize, userAccountManager);
		m_logger->Info("Initiating Real Time Market Data.");
		const auto* realTimeMarketDataCfg = matchingEngineXmlCfg->FirstChildElement("RealTimeMarketData");
		m_marketData = std::make_unique<RealTimeMarketData>(realTimeMarketDataCfg);
		auto* rtMarketParticipant = dynamic_cast<RTMarketParticipant*>(m_participant.get());
		assert(rtMarketParticipant);
		rtMarketParticipant->CreateDownstreamOrderBooks(m_marketData->GetSubscribingSymbols());
		m_marketData->RegisterDataStream(rtMarketParticipant);
	}
	else if (StringUtils::IsConfigAttributeMatched(usingParticipantXml->Attribute("Name"), "HistoricalParticipant"))
	{
		m_logger->Info("Initiating HistoricalParticipant.");
		m_participant = std::make_unique<HistoricalParticipant>(userAccountManager);
	}
	else if (StringUtils::IsConfigAttributeMatched(usingParticipantXml->Attribute("Name"), "SimulatorParticipant"))
	{
		m_logger->Info("Initiating SimulatorParticipant.");
		m_participant = std::make_unique<SimulatorParticipant>(userAccountManager);
	}
	else
	{
		throw std::runtime_error("MatchingEngine: unsupported Participant config");
	}
}

MatchingEngine::~MatchingEngine() {}

void MatchingEngine::Start()
{
	m_isRunning.store(true);
	m_thread = std::thread(&MatchingEngine::ProcessIncommingOrders, this);
	if (m_participant 
		&& m_participant->GetParticipantType() == ParticipantType::REAL_TIME_MARKET_DATA
		&& m_marketData) // if using RTMarketParticipant
	{
		// Start receive real time market data
		m_marketData->StartStreamingData();
	}
	else
	{
		m_thread.join();
	}
	// Should not reach here...
}

void MatchingEngine::Stop()
{
	m_isRunning.store(false);
	m_thread.join();
}

void MatchingEngine::ProcessIncommingOrders()
{
	// $$ Steps in the Order Processing $$ 
	//Receive an Order:
	//  The engine receives a buy or sell order from a client and put to order queue.
	//Order Validation:
	//  Ensure the order is valid (e.g., sufficient funds, correct format).
	//Check the Order Book:
	//  Compare the incoming order against the existing orders in the order book.
	//Matching Logic:
	//  Check TryToMatchOrder()

	m_logger->Info("Checking incoming upstream orders...");

	while (m_isRunning.load())
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		// Wait for new order or timeout after 1 second
		m_orderQueueCond.wait_for(lock, WAIT_TIME_OUT, [&]()
		{
			return m_hasNewOrder.load();
		});
		// If new trading hint is available, process it
		if (m_hasNewOrder.load())
		{
			while (!m_upstreamOrderQueueMgr->HasNoOrders())
			{
				auto order = m_upstreamOrderQueueMgr->GetNextOrder();

				const auto orderInfoStr = "OrderClientId= "
					+ UpstreamOrderUtils::GetOrderClientId(order) + "Symbol="
					+ UpstreamOrderUtils::GetOrderSymbol(order) + "OrderType="
					+ UpstreamOrderUtils::GetOrderTypeName(order);

				m_logger->Info("From upstream order queue, processing, " + orderInfoStr);

				lock.unlock();  // Unlock mutex during processing
				if (IsOrderEligibleToProcess(order))
				{
					m_logger->Info("From upstream order pre-trade checker, passed, " + orderInfoStr);

					if (std::holds_alternative<BinanceNewOrder>(order))
					{
						m_logger->Info("From order matching engine, looking for liquidity, " + orderInfoStr);

						auto& newOrder = std::get<BinanceNewOrder>(order);
						if (m_participant->TryToMatchOrder(newOrder))
						{
							m_upstreamOrderMatchedMgr->AddOrder(newOrder.GetClientOrderId(), order);
						}
						PostProcessingMatchedNewOrder(newOrder);
					}
					else if (std::holds_alternative<BinanceCancelOrder>(order))
					{
						m_logger->Info("From upstream order queue, cancelling, " + orderInfoStr);
						auto& cancelOrder = std::get<BinanceCancelOrder>(order);
						m_upstreamOrderQueueMgr->RemoveOrder(cancelOrder.GetOrigClientOrderId());
					}
					else if (std::holds_alternative<BinanceReplaceOrder>(order))
					{
						m_logger->Info("From upstream order queue, replacing, " + orderInfoStr);
						auto& replaceOrder = std::get<BinanceReplaceOrder>(order);
						m_upstreamOrderQueueMgr->ReplaceOrder(replaceOrder.GetOrigClientOrderId(), replaceOrder);
					}
					else if (std::holds_alternative<BinanceQueryOrder>(order))
					{
						m_logger->Info("From upstream order queue, querying, " + orderInfoStr);

						auto& queryOrder = std::get<BinanceQueryOrder>(order);
						m_upstreamOrderQueueMgr->LookupOrder(queryOrder.GetOrigClientOrderId());
					}
				}
				lock.lock();  // Lock mutex again for the next iteration
			}
			m_hasNewOrder.store(false); // Reset the flag after processing
		}
	}
}

void MatchingEngine::OnHandlingReceivedMessage(const BqtJsonMessage& message)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	if (message.IsValid())
	{
		const auto messageType = message.GetStringValueByTag(FieldLabels::BinanceOrderType);
		if (messageType == "BinanceNewOrder")
		{
			auto newOrder = ConstructUpstreamNewOrder(message);
			newOrder.SetOrderStatus(BinanceNewOrderStatus::WAITING_FOR_FILL);
			m_upstreamOrderQueueMgr->PushOrderToQueue(newOrder);
		}
		else if (messageType == "BinanceCancelOrder")
		{
			const auto cancelOrder = ConstructUpstreamCancelOrder(message);
			//cancelOrder.SetOrderStatus(BinanceNewOrderStatus::WAITING_FOR_FILL);
			m_upstreamOrderQueueMgr->PushOrderToQueue(cancelOrder);
		}
		else if (messageType == "BinanceReplaceOrder")
		{
			const auto replaceOrder = ConstructUpstreamReplaceOrder(message);
			//cancelOrder.SetOrderStatus(BinanceNewOrderStatus::WAITING_FOR_FILL);
			m_upstreamOrderQueueMgr->PushOrderToQueue(replaceOrder);
		}
		else if (messageType == "BinanceQueryOrder")
		{
			const auto queryOrder = ConstructUpstreamQueryOrder(message);
			//cancelOrder.SetOrderStatus(BinanceNewOrderStatus::WAITING_FOR_FILL);
			m_upstreamOrderQueueMgr->PushOrderToQueue(queryOrder);
		}
		else
		{
			const auto errMsg = "Unsupported BinanceOrder =" + messageType;
			m_logger->Error(errMsg);
			const auto ack = AckUtils::CreateErrorOrderAck(message.GetStringValueByTag(FieldLabels::Symbol),
				message.GetStringValueByTag(FieldLabels::ClientOrderId), errMsg);
			UpstreamGateWay->SendDownstreamOrderAck(ack);
			return; // dont process if message is invalid
		}
		m_hasNewOrder.store(true);
    }
	else
	{
		const auto errMsg = "Received an invalid BqtJsonMessage=" + message.SerializeMessage();
		m_logger->Error(errMsg);
		const auto ack = AckUtils::CreateErrorOrderAck(message.GetStringValueByTag(FieldLabels::Symbol),
			message.GetStringValueByTag(FieldLabels::ClientOrderId), errMsg);
		UpstreamGateWay->SendDownstreamOrderAck(ack);
	}
}

bool MatchingEngine::IsOrderEligibleToProcess(const UpstreamOrder& order)
{
	//Order Validation:
	//  Ensure the order is valid (e.g., sufficient funds, correct format).

	// User account check:
	const auto isAccountEligibleToTrade = m_userAccountManager->LookupUserAccount(
		std::get<BinanceNewOrder>(order).GetUserAccountID()).IsAccountEligibleToTrade();

	//if (m_userAccountManager->LookupUserAccount(
	//	order.GetUserAccountID()).IsHoldingThisAsset(order.GetSymbol()))
	//{
	//	const auto errMsg = "Received an invalid BinanceNewOrder with ammount is less than zero.";
	//	m_logger->Error(errMsg);
	//	const auto ack = AckUtils::CreateErrorOrderAck(order.GetSymbol(), order.GetClientOrderId(), errMsg);
	//	UpstreamGateWay->SendDownstreamOrderAck(ack);
	//	return false;
	//}
	
	// Upstream order check:
	auto isOrderEligibleToProcess{ false };
	if (std::holds_alternative<BinanceNewOrder>(order))
	{
		isOrderEligibleToProcess = VerifyUpstreamBinanceNewOrder(std::get<BinanceNewOrder>(order));
	}
	else if (std::holds_alternative<BinanceCancelOrder>(order)) 
	{
		isOrderEligibleToProcess = VerifyUpstreamBinanceCancelOrder(std::get<BinanceCancelOrder>(order));
	}
	else if (std::holds_alternative<BinanceReplaceOrder>(order)) 
	{
		isOrderEligibleToProcess = VerifyUpstreamBinanceReplaceOrder(std::get<BinanceReplaceOrder>(order));
	}
	else if (std::holds_alternative<BinanceQueryOrder>(order))
	{
		isOrderEligibleToProcess = VerifyUpstreamBinanceQueryOrder(std::get<BinanceQueryOrder>(order));

	}
	return isOrderEligibleToProcess && isAccountEligibleToTrade;
}

bool MatchingEngine::VerifyUpstreamBinanceNewOrder(const BinanceNewOrder& order)
{
	if (order.GetAmountDouble() <= ZERO_DOUBLE_VALUE)
	{
		const auto errMsg = "Received an invalid BinanceNewOrder with ammount is less than zero.";
		m_logger->Error(errMsg);
		const auto ack = AckUtils::CreateErrorOrderAck(order.GetSymbol(), order.GetClientOrderId(), errMsg);
		UpstreamGateWay->SendDownstreamOrderAck(ack);
		return false;
	}
	if (order.GetPriceDouble() <= ZERO_DOUBLE_VALUE)
	{
		const auto errMsg = "Received an invalid BinanceNewOrder with price is less than zero.";
		m_logger->Error(errMsg);
		const auto ack = AckUtils::CreateErrorOrderAck(order.GetSymbol(), order.GetClientOrderId(), errMsg);
		UpstreamGateWay->SendDownstreamOrderAck(ack);
		return false;
	}
	return true;
}

bool MatchingEngine::VerifyUpstreamBinanceCancelOrder(const BinanceCancelOrder& order)
{
	return true;
}

bool MatchingEngine::VerifyUpstreamBinanceReplaceOrder(const BinanceReplaceOrder& order)
{
	return true;
}

bool MatchingEngine::VerifyUpstreamBinanceQueryOrder(const BinanceQueryOrder& order)
{
	return true;
}

void MatchingEngine::PostProcessingMatchedNewOrder(BinanceNewOrder& order)
{
	//Update the Order Book:
	// If the order is fully matched, remove the order from the order book.
	const auto& clientOrderId = order.GetClientOrderId();
	const auto orderStr = order.ToString();
	if (order.GetOrderStatus() == BinanceNewOrderStatus::FULL_FILLED)
	{
		if (m_upstreamOrderQueueMgr->RemoveOrder(clientOrderId))
		{
			m_logger->Info("Full filled order info: " + orderStr);
			m_logger->Info("Removed filled order from upstream order queue.");
			// Notify the involved parties of the trade execution.
			const auto ack = AckUtils::CreateFilledOrderAck(order);
			UpstreamGateWay->SendDownstreamOrderAck(ack);
		}
	}
	// If the order is not fully matched, insert the order into back of the order book to continue fill it later.
	else if (order.GetOrderStatus() == BinanceNewOrderStatus::PRTIAL_FILLED)
	{
		if (m_upstreamOrderQueueMgr->RemoveOrder(clientOrderId)) // remove order first
		{
			m_logger->Info("Partial filled order info: " + orderStr);
			m_upstreamOrderQueueMgr->PushOrderToQueue(order); // move to back of the queue
			// Notify the involved parties of the trade execution.
			const auto ack = AckUtils::CreateFilledOrderAck(order);
			UpstreamGateWay->SendDownstreamOrderAck(ack);
		}
	}
	// If the order does not have liquidity, insert the order into back of the order book to continue fill it later.
	else if (order.GetOrderStatus() == BinanceNewOrderStatus::WAITING_FOR_FILL)
	{
		m_logger->Info("Tried to match but order has no liquidity from market: " + orderStr);
	}
}

BinanceNewOrder MatchingEngine::ConstructUpstreamNewOrder(
	const BqtJsonMessage& message)
{
	const std::string symbol = message.GetStringValueByTag(FieldLabels::Symbol);
	const binapi::e_side side = binapi::e_side_from_string(message.GetStringValueByTag(FieldLabels::Side).c_str());
	const binapi::e_type type = binapi::e_type_from_string(message.GetStringValueByTag(FieldLabels::Type).c_str());
	const binapi::e_time time = binapi::e_time_from_string(message.GetStringValueByTag(FieldLabels::Time).c_str());
	const double amount = message.GetDoubleValueByTag(FieldLabels::Time);
	const double price = message.GetDoubleValueByTag(FieldLabels::Price);
	const std::string clientOrderId = message.GetStringValueByTag(FieldLabels::ClientOrderId);
	const std::string stopPrice = message.GetStringValueByTag(FieldLabels::StopPrice);
	const std::string icebergAmount = message.GetStringValueByTag(FieldLabels::IcebergAmount);
	return BinanceNewOrder(
		symbol,
		side,
		type,
		time,
		amount,
		price,
		clientOrderId,
		stopPrice,
		icebergAmount);
}

BinanceCancelOrder MatchingEngine::ConstructUpstreamCancelOrder(
	const BqtJsonMessage& message)
{
	const std::string symbol = message.GetStringValueByTag(FieldLabels::Symbol);
	const std::string orderId = message.GetStringValueByTag(FieldLabels::OrderId);
	const std::string origClientOrderId = message.GetStringValueByTag(FieldLabels::OrigClientOrderId);
	const std::string clientOrderId = message.GetStringValueByTag(FieldLabels::ClientOrderId);
	return BinanceCancelOrder(
		symbol,
		std::stoull(orderId),
		origClientOrderId,
		clientOrderId);
}

BinanceReplaceOrder MatchingEngine::ConstructUpstreamReplaceOrder(
	const BqtJsonMessage& message)
{
	const std::string symbol = message.GetStringValueByTag(FieldLabels::Symbol);
	const std::string orderId = message.GetStringValueByTag(FieldLabels::OrderId);
	const std::string origClientOrderId = message.GetStringValueByTag(FieldLabels::OrigClientOrderId);
	const std::string clientOrderId = message.GetStringValueByTag(FieldLabels::ClientOrderId);
	return BinanceReplaceOrder(
		symbol,
		std::stoull(orderId),
		origClientOrderId,
		clientOrderId);
}

BinanceQueryOrder MatchingEngine::ConstructUpstreamQueryOrder(
	const BqtJsonMessage& message)
{
	const std::string symbol = message.GetStringValueByTag(FieldLabels::Symbol);
	const std::string orderId = message.GetStringValueByTag(FieldLabels::OrderId);
	const std::string origClientOrderId = message.GetStringValueByTag(FieldLabels::OrigClientOrderId);
	const std::string clientOrderId = message.GetStringValueByTag(FieldLabels::ClientOrderId);
	return BinanceQueryOrder(
		symbol,
		std::stoull(orderId),
		origClientOrderId,
		clientOrderId);
}