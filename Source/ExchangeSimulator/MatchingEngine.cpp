/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../SettingNConfig/tinyxml2.h"
#include "../SettingNConfig/BqtXmlUtils.h"
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

#include "CommonDef.h"

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
		m_binanceMarketDataConfig = SettingNConfig::BqtXmlUtils::GetBinanceMarketDataConfig(realTimeMarketDataCfg);
		const auto* exchangeSimulatorMarketDataCfg = m_binanceMarketDataConfig->FirstChildElement("RealTimeMarketData");
		m_marketData = std::make_unique<RealTimeMarketData>(exchangeSimulatorMarketDataCfg);
		SubscribeTargetSymbols(m_binanceMarketDataConfig.get());
		m_rtMarketDataParticipant = dynamic_cast<RTMarketParticipant*>(m_participant.get());
		assert(m_rtMarketDataParticipant);
		m_rtMarketDataParticipant->CreateDownstreamOrderBooks(m_marketData->GetSubscribingSymbols());
		m_marketData->RegisterDataListener(m_rtMarketDataParticipant);
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

void MatchingEngine::SubscribeTargetSymbols(const tinyxml2::XMLDocument* realTimeMarketDataCfg)
{
	const auto* targetSymbolXml = realTimeMarketDataCfg->FirstChildElement("ExchangeSimulatorMarketData");
	assert(targetSymbolXml);
	const tinyxml2::XMLElement* symbolsXml = targetSymbolXml->FirstChildElement("MatchingForSymbols");
	assert(symbolsXml);
	auto targetTradeSymbols = StringUtils::SplitAndTrimString(symbolsXml->Attribute("List"), ',');
	for (const auto& symbol : targetTradeSymbols)
	{
		m_marketData->SubscribeSymbol(symbol);
	}
}

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
	if (m_participant
		&& m_participant->GetParticipantType() == ParticipantType::REAL_TIME_MARKET_DATA
		&& m_marketData) // if using RTMarketParticipant
	{
		// Start receive real time market data
		m_marketData->UnRegisterDataListener(m_rtMarketDataParticipant);
	}
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
				// -Dequeue order from waiting list, so the order is not in the queue anymore
				// -We have to push it back to the end of the queue if it can not be filled by the matching engine
				auto order = m_upstreamOrderQueueMgr->GetNextOrder();

				const auto orderInfoStr = "OrderClientId="
					+ UpstreamOrderUtils::GetOrderClientId(order) + ", Symbol="
					+ UpstreamOrderUtils::GetOrderSymbol(order) + ", OrderType="
					+ UpstreamOrderUtils::GetOrderTypeName(order);

				lock.unlock();  // Unlock mutex during processing			

				if (std::holds_alternative<BinanceNewOrder>(order))
				{
					m_logger->Info("From order matching engine, looking for liquidity for order=" + orderInfoStr);

					auto& newOrder = std::get<BinanceNewOrder>(order);
					if (m_participant->TryToMatchOrder(newOrder))
					{
						m_upstreamOrderMatchedMgr->AddOrder(newOrder.GetClientOrderId(), order);
					}
					PostProcessingMatchedNewOrder(newOrder);
				}
				else if (std::holds_alternative<BinanceCancelOrder>(order))
				{
					m_logger->Info("From upstream order queue, cancelling order=" + orderInfoStr);
					auto& cancelOrder = std::get<BinanceCancelOrder>(order);
					if (m_upstreamOrderQueueMgr->RemoveOrder(cancelOrder.GetOrigClientOrderId()))
					{
						cancelOrder.SetOrderStatus(BinanceCancelOrderStatus::FILLED);
						PostProcessingMatchedCancelOrder(cancelOrder);
					}
					else
					{
						m_logger->Info("Failed to cancel order=" + orderInfoStr);
					}
				}
				else if (std::holds_alternative<BinanceReplaceOrder>(order))
				{
					m_logger->Info("From upstream order queue, replacing order=" + orderInfoStr);
					auto& replaceOrder = std::get<BinanceReplaceOrder>(order);
					if (m_upstreamOrderQueueMgr->ReplaceOrder(replaceOrder.GetOrigClientOrderId(), replaceOrder))
					{
						replaceOrder.SetOrderStatus(BinanceReplaceOrderStatus::FILLED);
						PostProcessingMatchedReplaceOrder(replaceOrder);
					}
					else
					{
						m_logger->Info("Failed to replace order=" + orderInfoStr);
					}
				}
				else if (std::holds_alternative<BinanceQueryOrder>(order))
				{
					m_logger->Info("From upstream order queue, querying order=" + orderInfoStr);
					auto& queryOrder = std::get<BinanceQueryOrder>(order);
					queryOrder.SetOrderStatus(BinanceQueryOrderStatus::FILLED);
					auto foundOrder = m_upstreamOrderQueueMgr->LookupOrder(queryOrder.GetOrigClientOrderId());
					auto& foundNewOrder = std::get<BinanceNewOrder>(foundOrder);
					PostProcessingMatchedQueryOrder(queryOrder);
				}		
				lock.lock();  // Lock mutex again for the next iteration
			}
			m_hasNewOrder.store(false); // Reset the flag after processing
		}
	}
}

void MatchingEngine::OnHandlingReceivedSimulatorMessage(const BqtJsonMessage& message)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	if (message.IsValid())
	{
		const auto messageType = message.GetStringValueByTag(FieldLabels::MessageType);
		if (messageType == Binance_Order_Type::New_Order)
		{
			auto newOrder = ConstructUpstreamNewOrder(message);
		
			// User account check:
			const bool isAccountEligibleToTrade = m_userAccountManager->LookupUserAccount(
				newOrder.GetUserAccountID())->IsAccountEligibleToTrade();

			if (!isAccountEligibleToTrade)
			{
				const auto errMsg = "User account is not eligibe to execute new order.";
				m_logger->Error(errMsg);
				const auto ack = AckUtils::CreateErrorRejectOrderAck(newOrder.GetSymbol(), newOrder.GetClientOrderId(), errMsg);
				UpstreamGateWay->SendDownstreamOrderAck(ack);
				return;
			}

			if (VerifyUpstreamBinanceNewOrder(newOrder))
			{
				newOrder.SetOrderStatus(BinanceNewOrderStatus::WAITING_FOR_FILL);
				m_upstreamOrderQueueMgr->PushOrderToQueue(newOrder.GetClientOrderId(), newOrder);
			}
			else return;
		}
		else if (messageType == Binance_Order_Type::Cancel_Order)
		{
			auto cancelOrder = ConstructUpstreamCancelOrder(message);
			if (VerifyUpstreamBinanceCancelOrder(cancelOrder))
			{
				cancelOrder.SetOrderStatus(BinanceCancelOrderStatus::WAITING_FOR_CANCEL);
				m_upstreamOrderQueueMgr->PushOrderToQueue(cancelOrder.GetClientOrderId(), cancelOrder);
			}
			else return;
		}
		else if (messageType == Binance_Order_Type::Replace_Order)
		{
			auto replaceOrder = ConstructUpstreamReplaceOrder(message);
			if (VerifyUpstreamBinanceReplaceOrder(replaceOrder))
			{
				replaceOrder.SetOrderStatus(BinanceReplaceOrderStatus::WAITING_FOR_REPLACE);
				m_upstreamOrderQueueMgr->PushOrderToQueue(replaceOrder.GetClientOrderId(), replaceOrder);
			}
			else return;
		}
		else if (messageType == Binance_Order_Type::Query_Order)
		{
			auto queryOrder = ConstructUpstreamQueryOrder(message);
			if (VerifyUpstreamBinanceQueryOrder(queryOrder))
			{
				queryOrder.SetOrderStatus(BinanceQueryOrderStatus::WAITING_FOR_QUERY);
				m_upstreamOrderQueueMgr->PushOrderToQueue(queryOrder.GetClientOrderId(), queryOrder);
			}
			else return;
		}
		else
		{
			const auto errMsg = "Unsupported BinanceOrder =" + messageType;
			m_logger->Error(errMsg);
			const auto ack = AckUtils::CreateErrorRejectOrderAck(message.GetStringValueByTag(FieldLabels::Symbol),
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
		const auto ack = AckUtils::CreateErrorRejectOrderAck(message.GetStringValueByTag(FieldLabels::Symbol),
			message.GetStringValueByTag(FieldLabels::ClientOrderId), errMsg);
		UpstreamGateWay->SendDownstreamOrderAck(ack);
	}
}

bool MatchingEngine::VerifyUpstreamBinanceNewOrder(const BinanceNewOrder& order)
{
	if (order.GetAmount() <= ZERO_DOUBLE_VALUE)
	{
		const auto errMsg = "Received an invalid BinanceNewOrder with ammount is less than zero.";
		m_logger->Error(errMsg);
		const auto ack = AckUtils::CreateErrorRejectOrderAck(order.GetSymbol(), order.GetClientOrderId(), errMsg);
		UpstreamGateWay->SendDownstreamOrderAck(ack);
		return false;
	}
	if (order.GetPrice() <= ZERO_DOUBLE_VALUE)
	{
		const auto errMsg = "Received an invalid BinanceNewOrder with price is less than zero.";
		m_logger->Error(errMsg);
		const auto ack = AckUtils::CreateErrorRejectOrderAck(order.GetSymbol(), order.GetClientOrderId(), errMsg);
		UpstreamGateWay->SendDownstreamOrderAck(ack);
		return false;
	}
	return true;
}

bool MatchingEngine::VerifyUpstreamBinanceCancelOrder(const BinanceCancelOrder& order)
{
	if (UpstreamOrderUtils::GetOrderClientId(order) != order.GetClientOrderId())
	{
		const auto errMsg = "Could not find target order to cancel with OrderClientId=" + order.GetClientOrderId();
		m_logger->Error(errMsg);
		const auto ack = AckUtils::CreateErrorRejectOrderAck(order.GetSymbol(), order.GetClientOrderId(), errMsg);
		UpstreamGateWay->SendDownstreamOrderAck(ack);
		return false;
	}
	return true;
}

bool MatchingEngine::VerifyUpstreamBinanceReplaceOrder(const BinanceReplaceOrder& order)
{
	if (UpstreamOrderUtils::GetOrderClientId(order) != order.GetClientOrderId())
	{
		const auto errMsg = "Could not find target order to replace with OrderClientId=" + order.GetClientOrderId();
		m_logger->Error(errMsg);
		const auto ack = AckUtils::CreateErrorRejectOrderAck(order.GetSymbol(), order.GetClientOrderId(), errMsg);
		UpstreamGateWay->SendDownstreamOrderAck(ack);
		return false;
	}
	return true;
}

bool MatchingEngine::VerifyUpstreamBinanceQueryOrder(const BinanceQueryOrder& order)
{
	if (UpstreamOrderUtils::GetOrderClientId(order) != order.GetClientOrderId())
	{
		const auto errMsg = "Could not find target order to query with OrderClientId=" + order.GetClientOrderId();
		m_logger->Error(errMsg);
		const auto ack = AckUtils::CreateErrorRejectOrderAck(order.GetSymbol(), order.GetClientOrderId(), errMsg);
		UpstreamGateWay->SendDownstreamOrderAck(ack);
		return false;
	}
	return true;
}

void MatchingEngine::PostProcessingMatchedNewOrder(BinanceNewOrder& order)
{
	//Update the Order Book:
	// If the order is fully matched, remove the order from the order book.
	const auto& clientOrderId = order.GetClientOrderId();
	if (order.GetOrderStatus() == BinanceNewOrderStatus::FULL_FILLED)
	{
		m_logger->Info("Full filled order info: " + order.ToStringAck());
		m_logger->Info("Sending filled fill execution ack to upstream...");
		// Notify the involved parties of the trade execution.
		const auto ack = AckUtils::CreateFilledOrderAck(order, "Filled");
		UpstreamGateWay->SendDownstreamOrderAck(ack);
	}
	// If the order is not fully matched, insert the order into back of the order book to continue fill it later.
	else if (order.GetOrderStatus() == BinanceNewOrderStatus::PRTIAL_FILLED)
	{
		m_logger->Info("Partial filled order info: " + order.ToStringAck());
		m_upstreamOrderQueueMgr->PushOrderToQueue(clientOrderId, order); // move to back of the queue
		// Notify the involved parties of the trade execution.
		m_logger->Info("Sending partial filled execution ack to upstream...");
		const auto ack = AckUtils::CreateFilledOrderAck(order, "Parital filled");
		UpstreamGateWay->SendDownstreamOrderAck(ack);
	}
	// If the order does not have liquidity, insert the order into back of the order book to continue fill it later.
	// In case order is IOC or FOK, the order will be not be pushed back to the queue.
	else if (order.GetOrderStatus() == BinanceNewOrderStatus::WAITING_FOR_FILL)
	{
		if (order.GetTimeInForce() == binapi::e_time::GTC)
		{
			m_upstreamOrderQueueMgr->PushOrderToQueue(clientOrderId, order); // move to back of the queue
			m_logger->Info("Tried to match but order has no liquidity from market, push back to order queue=" + order.ToStringOrder());
			// Notify the involved parties of the trade execution.
			const auto ack = AckUtils::CreateNewOrderAck(order, "Could not find liquidity, push back to order queue");
			UpstreamGateWay->SendDownstreamOrderAck(ack);
		}
		else
		{
			order.SetOrderStatus(BinanceNewOrderStatus::SKIPPED);
			m_logger->Info("Skipped unfilled upstream order: " + order.ToStringOrder());
			// Notify the involved parties of the trade execution.
			const auto ack = AckUtils::CreateNewOrderAck(order, "Could not find liquidity, skipped order");
			UpstreamGateWay->SendDownstreamOrderAck(ack);
		}
	}
	else
	{
		m_logger->Info("Unknown upstream order status: " + order.ToStringOrder());
	}
}

void MatchingEngine::PostProcessingMatchedCancelOrder(BinanceCancelOrder& order)
{
	const auto& clientOrderId = order.GetClientOrderId();
	if (order.GetOrderStatus() == BinanceCancelOrderStatus::FILLED)
	{
		m_logger->Info("Cancelled order info: " + order.ToStringAck());
		m_logger->Info("Sending cancelled execution ack to upstream...");
		// Notify the involved parties of the trade execution.
		const auto ack = AckUtils::CreateCancelledOrderAck(order, "Cancelled");
		UpstreamGateWay->SendDownstreamOrderAck(ack);
	}
}

void MatchingEngine::PostProcessingMatchedReplaceOrder(BinanceReplaceOrder& order)
{
	const auto& clientOrderId = order.GetClientOrderId();
	if (order.GetOrderStatus() == BinanceReplaceOrderStatus::FILLED)
	{
		m_logger->Info("Replaced order info: " + order.ToStringAck());
		m_logger->Info("Sending replaced execution ack to upstream...");
		// Notify the involved parties of the trade execution.
		const auto ack = AckUtils::CreateReplacedOrderAck(order, "Replaced");
		UpstreamGateWay->SendDownstreamOrderAck(ack);
	}
}

void MatchingEngine::PostProcessingMatchedQueryOrder(BinanceQueryOrder& order)
{
	const auto& clientOrderId = order.GetClientOrderId();
	if (order.GetOrderStatus() == BinanceQueryOrderStatus::FILLED)
	{
		m_logger->Info("Queried order info: " + order.ToStringAck());
		m_logger->Info("Sending queried execution ack to upstream...");
		// Notify the involved parties of the trade execution.
		const auto ack = AckUtils::CreateQueryOrderAck(order, "Queried");
		UpstreamGateWay->SendDownstreamOrderAck(ack);
	}
}

BinanceNewOrder MatchingEngine::ConstructUpstreamNewOrder(
	const BqtJsonMessage& message)
{
	const std::string clientOrderId = message.GetStringValueByTag(FieldLabels::ClientOrderId);
	const std::string symbol = message.GetStringValueByTag(FieldLabels::Symbol);
	const binapi::e_side side = binapi::e_side_from_string(message.GetStringValueByTag(FieldLabels::Side).c_str());
	const binapi::e_type type = binapi::e_type_from_string(message.GetStringValueByTag(FieldLabels::Type).c_str());
	const binapi::e_time time = binapi::e_time_from_string(message.GetStringValueByTag(FieldLabels::TimeInForce).c_str());
	const double amount = message.GetDoubleValueByTag(FieldLabels::Amount);
	const double price = message.GetDoubleValueByTag(FieldLabels::LimitPrice);
	const double stopPrice = message.GetDoubleValueByTag(FieldLabels::StopPrice);
	const double icebergAmount = message.GetDoubleValueByTag(FieldLabels::IcebergAmount);
	BinanceNewOrder order(
		clientOrderId,
		symbol,
		side,
		type,
		time,
		amount,
		price,
		stopPrice,
		icebergAmount,
		TradeType::TEST);

	order.SetUserAccountID(message.GetStringValueByTag(FieldLabels::UserAccountID));

	return order;
}

BinanceCancelOrder MatchingEngine::ConstructUpstreamCancelOrder(
	const BqtJsonMessage& message)
{
	const std::string symbol = message.GetStringValueByTag(FieldLabels::Symbol);
	const std::string orderId = message.GetStringValueByTag(FieldLabels::OrderId);
	const std::string origClientOrderId = message.GetStringValueByTag(FieldLabels::OrigClientOrderId);
	const std::string clientOrderId = message.GetStringValueByTag(FieldLabels::ClientOrderId);
	BinanceCancelOrder order(
		clientOrderId,
		symbol,
		std::stoull(orderId),
		origClientOrderId);

	order.SetUserAccountID(message.GetStringValueByTag(FieldLabels::UserAccountID));

	return order;
}

BinanceReplaceOrder MatchingEngine::ConstructUpstreamReplaceOrder(
	const BqtJsonMessage& message)
{
	const std::string symbol = message.GetStringValueByTag(FieldLabels::Symbol);
	const std::string orderId = message.GetStringValueByTag(FieldLabels::OrderId);
	const std::string origClientOrderId = message.GetStringValueByTag(FieldLabels::OrigClientOrderId);
	const std::string clientOrderId = message.GetStringValueByTag(FieldLabels::ClientOrderId);
	BinanceReplaceOrder order(
		clientOrderId,
		symbol,
		std::stoull(orderId),
		origClientOrderId);

	order.SetUserAccountID(message.GetStringValueByTag(FieldLabels::UserAccountID));

	return order;
}

BinanceQueryOrder MatchingEngine::ConstructUpstreamQueryOrder(
	const BqtJsonMessage& message)
{
	const std::string symbol = message.GetStringValueByTag(FieldLabels::Symbol);
	const std::string orderId = message.GetStringValueByTag(FieldLabels::OrderId);
	const std::string origClientOrderId = message.GetStringValueByTag(FieldLabels::OrigClientOrderId);
	const std::string clientOrderId = message.GetStringValueByTag(FieldLabels::ClientOrderId);
	BinanceQueryOrder order(
		clientOrderId,
		symbol,
		std::stoull(orderId),
		origClientOrderId);

	order.SetUserAccountID(message.GetStringValueByTag(FieldLabels::UserAccountID));

	return order;
}