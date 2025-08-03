/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../SettingNConfig/tinyxml2.h"
#include "../PortfolioManager/PortfolioInvestmentBinance.h"
#include "../ExchangeConnectivity/BinanceExchangeConnectivity.h"
#include "../RestAPI/RestAPI.h"
#include "../RestAPI/BinanceSpotApiGateWay.h"
#include "../StaticData/StaticDataManager.h"
#include "../OrderManagement/BinanceNewOrder.h"
#include "../OrderManagement/BinanceCancelOrder.h"
#include "../OrderManagement/BinanceReplaceOrder.h"
#include "../OrderManagement/FieldLabels.h"
#include "../RiskManagement/RiskManager.h"
#include "../ComplianceNRegulatory/BinanceTradingRules.h"
#include "../LibraryUtils/SourceBuildFlags.h"
#include "../LibraryUtils/StringUtils.h"
#if USE_BACK_TEST_TRADING
#include "../ExchangeConnectivity/ExchangeSimulatorConnectivity.h"
#include "../ExchangeSimulator/DownstreamOrderAck.h"
#include "BackTestReporter.h"
#else
#include "BinanceReporter.h"
#endif

#include "BinanceTrader.h"

#include <exception>

using namespace UserAccount;
using namespace PortfolioManager;
using namespace ComplianceNRegulatory;
using namespace RiskManagement;
using namespace OrderManagement;
using namespace RestAPI;
using namespace StaticData;
using namespace ExchangeConnectivity;
#if USE_BACK_TEST_TRADING
using namespace ExchangeSimulator;
#endif
using namespace tinyxml2;

static constexpr double ZERO_DOUBLE_VALUE = 0;

BinanceTrader::BinanceTrader(
	const XMLElement* reportCfg,
	PortfolioInvestmentBinance* portfolio,
	BinanceTradingRules* tradingRules,
	RiskManager* riskManager)
    : m_portfolio(portfolio),
	  m_tradingRules(tradingRules),
	  m_riskManager(riskManager)
{
    m_logger = std::make_unique<LibraryUtils::Logger>("BinanceTrader");
    m_logger->Info("using BinanceTrader.");
	m_binanceAccountInfo = std::make_unique<binapi::rest::account_info_t>();
	m_positionManager = std::make_unique<PositionManager>();
#if USE_BACK_TEST_TRADING
	m_exchangeReporter = std::make_unique<BackTestReporter>(
		reportCfg, m_binanceAccountInfo.get(), m_tradingRules->GetExchangeProfileMgr(), m_positionManager.get());
#else
	m_exchangeReporter = std::make_unique<BinanceReporter>(
		reportCfg, m_binanceAccountInfo.get(), m_tradingRules->GetExchangeProfileMgr(), m_positionManager.get());
#endif
}

////////////// UPSTREAM PROCESSING /////////////////////////////

double BinanceTrader::CalculateTradeValue(
	const double quality,
	const double refPrice)
{
	return quality * refPrice;
}

WorkedOrderIdentification BinanceTrader::CreateNewPosition(const QuantitativeModel::QuantOrderParammeter& param)
{
	if (param.m_side == binapi::e_side::buy)
	{
#if USE_BACK_TEST_TRADING
		if (m_binanceAccountInfo->stableCoinAmount.convert_to<double>() 
			> CalculateTradeValue(param.m_amount, param.m_price))
		{
#else
		if (m_portfolio->GetBinanceTradingPair(param.m_symbol))
		{
#endif
			auto newSingleLongOrder = m_positionManager->OpenNewPositionUpstreamOrder(param);
#if USE_BACK_TEST_TRADING
			const auto result = ExchangeSimulatorGateWay->SendNewSimulatorOrderFull(newSingleLongOrder.get());
#elif USE_BINANCE_TEST_TRADING
			const auto result = BinanceExchangeGateWay->SendNewBinanceTestOrderFull(newSingleLongOrder.get());
#else
			const auto result = BinanceExchangeGateWay->SendNewBinanceOrderFull(newSingleLongOrder.get());
#endif
			newSingleLongOrder->SetSendingOrderResult(result);
			const auto isSendingOrderSucceeded = static_cast<bool>(result);
			const auto clientOrderId = newSingleLongOrder->GetClientOrderId();
			if (isSendingOrderSucceeded)
			{
				newSingleLongOrder->SetOrderStatus(BinanceNewOrderStatus::WAITING_FOR_FILL);
				m_positionManager->AddNewWorkedOrder(clientOrderId, std::move(newSingleLongOrder));
			}
			else
			{
				m_positionManager->AddUnworkedOrder(clientOrderId, std::move(newSingleLongOrder));
			}
			return { isSendingOrderSucceeded, clientOrderId };
		}
		else
		{
			m_logger->Warning("User spot account has no stable coin available, could not create long (buy) position for=" + param.m_symbol);
			return { false , "" };
		}
	}
	else if (param.m_side == binapi::e_side::sell)
	{
		if (m_portfolio->GetBinanceTradingPair(param.m_symbol)->GetQuantity() > ZERO_DOUBLE_VALUE)
		{
			auto newSingleShortOrder = m_positionManager->OpenNewPositionUpstreamOrder(param);
#if USE_BACK_TEST_TRADING
			const auto result = ExchangeSimulatorGateWay->SendNewSimulatorOrderFull(newSingleShortOrder.get());
#elif USE_BINANCE_TEST_TRADING
			const auto result = BinanceExchangeGateWay->SendNewBinanceTestOrderFull(newSingleShortOrder.get());
#else
			const auto result = BinanceExchangeGateWay->SendNewBinanceOrderFull(newSingleShortOrder.get());
#endif
			newSingleShortOrder->SetSendingOrderResult(result);
			const auto isSendingOrderSucceeded = static_cast<bool>(result);
			const auto clientOrderId = newSingleShortOrder->GetClientOrderId();
			if (isSendingOrderSucceeded)
			{
				newSingleShortOrder->SetOrderStatus(BinanceNewOrderStatus::WAITING_FOR_FILL);
				m_positionManager->AddNewWorkedOrder(clientOrderId, std::move(newSingleShortOrder));
			}
			else
			{
				m_positionManager->AddUnworkedOrder(clientOrderId, std::move(newSingleShortOrder));
			}
			return { isSendingOrderSucceeded, clientOrderId };
		}
		else
		{
			m_logger->Warning("User spot account has no asset available, could not create short (sell) position for=" + param.m_symbol);
			return { false , "" };
		}
	}
	return { false , "" };
}

bool BinanceTrader::CancelAllOpenPositions(const std::string& symbol)
{
	const auto workedOrderManager = m_positionManager->GetWorkedOrderManager();
	if (workedOrderManager)
	{
		for (const auto* order : workedOrderManager->GetOrdersOfSymbol(symbol))
		{
			auto newCancelOrder = m_positionManager->CancelPositionUpstreamOrder(order);
#if USE_BACK_TEST_TRADING
			const auto result = ExchangeSimulatorGateWay->SendCancelSimulatorOrder(newCancelOrder.get());
#else
			const auto result = BinanceExchangeGateWay->SendCancelBinanceOrder(newCancelOrder.get());
#endif
			newCancelOrder->SetSendingOrderResult(result);
			const auto isSendingOrderSucceeded = static_cast<bool>(result);
			const auto clientOrderId = newCancelOrder->GetClientOrderId();
			if (isSendingOrderSucceeded)
			{
				newCancelOrder->SetOrderStatus(BinanceCancelOrderStatus::WAITING_FOR_CANCEL);
				m_positionManager->AddNewCancelOrder(clientOrderId, std::move(newCancelOrder));
				m_positionManager->CloseOpenedPositionUpstreamOrder(clientOrderId);
			}
			else
			{
				m_positionManager->AddUnworkedCancelOrder(clientOrderId, std::move(newCancelOrder));
			}
		}
	}
	else
	{
		m_logger->Warning("No open positions available to cancel.");
		return false;
	}
	return true;
}

WorkedOrderIdentification BinanceTrader::CancelOpenPosition(const std::string& clientOrderId)
{
	const auto workedOrderManager = m_positionManager->GetWorkedOrderManager();
	if (workedOrderManager)
	{
		auto* order = workedOrderManager->LookupOrder(clientOrderId);
		if (order)
		{
			auto newCancelOrder = m_positionManager->CancelPositionUpstreamOrder(order);
#if USE_BACK_TEST_TRADING
			const auto result = ExchangeSimulatorGateWay->SendCancelSimulatorOrder(newCancelOrder.get());
#else
			const auto result = BinanceExchangeGateWay->SendCancelBinanceOrder(newCancelOrder.get());
#endif
			newCancelOrder->SetSendingOrderResult(result);
			const auto isSendingOrderSucceeded = static_cast<bool>(result);
			const auto cancelClientOrderId = newCancelOrder->GetClientOrderId();
			if (isSendingOrderSucceeded)
			{
				newCancelOrder->SetOrderStatus(BinanceCancelOrderStatus::WAITING_FOR_CANCEL);
				m_positionManager->AddNewCancelOrder(cancelClientOrderId, std::move(newCancelOrder));
				m_positionManager->CloseOpenedPositionUpstreamOrder(cancelClientOrderId);
			}
			else
			{
				m_positionManager->AddUnworkedCancelOrder(cancelClientOrderId, std::move(newCancelOrder));
			}
			return { isSendingOrderSucceeded, cancelClientOrderId };
		}
		else
		{
			m_logger->Warning("No open position found for client order ID: " + clientOrderId);
			return { false , "" };
		}
	}
	else
	{
		m_logger->Warning("No open positions available to cancel.");
		return { false , "" };
	}
}

void BinanceTrader::UpdateAccountInfo()
{
	m_portfolio->UpdateBinanceAccountInfo();
}

void BinanceTrader::ReportTradeResults(const std::string& symbol)
{
	m_exchangeReporter->DoTradeExecutionReport(symbol);
}

void BinanceTrader::CreatePortfolioManagement(const std::vector<std::string>& targetTradeSymbols)
{
	for (const auto& symbol : targetTradeSymbols)
	{
		m_portfolio->AddNewAssetToManage(symbol);
	}
	m_logger->Info("querying Binance remote spot account info...");
	// Query all assets from remote Binance account and manage them locally for our trading
	m_portfolio->SetUserSpotAccountInfo(m_binanceAccountInfo.get());
	m_portfolio->UpdateBinanceAccountInfo();
	m_logger->Info("querying account info finished.");
}

////////////// DOWNSTREAM PROCESSING /////////////////////////////

#if USE_BACK_TEST_TRADING  
void BinanceTrader::HandleDownstreamAckMessage(const MiddlewareMQ::BqtJsonMessage& message)
{
	m_logger->Info("Received simulator ack=" + message.SerializeMessage());
	const std::string simulatorAckType = message.GetStringValueByTag(FieldLabels::SimulatorAck::AckType);
	if (simulatorAckType == FieldLabels::DownstreamAckTypes::NewOrderAck ||
		simulatorAckType == FieldLabels::DownstreamAckTypes::FilledNewOrderAck)
	{
		// Updates the trader’s position.
		const auto clientOrderId = message.GetStringValueByTag(FieldLabels::ClientOrderId);
		const auto symbol = message.GetStringValueByTag(FieldLabels::Symbol);
		const auto filledAmount = message.GetDoubleValueByTag(FieldLabels::FilledAmount);
		const auto filledPrice = message.GetDoubleValueByTag(FieldLabels::FilledPrice);
		const auto remainingAmount = message.GetDoubleValueByTag(FieldLabels::RemainingAmount);
		const auto updateTime = message.GetIntValueByTag(FieldLabels::UpdateTime);
		const auto exchangeText = message.GetStringValueByTag(FieldLabels::SimulatorAck::ExchangeText);
		const auto orderStatus = BinanceNewOrder::GetOrderStatusEnum(
			message.GetStringValueByTag(FieldLabels::OrderStatus));
		auto* ackOrder = m_positionManager->UpdateNewOrderExecutionStatus(
			clientOrderId, symbol, filledAmount, filledPrice, remainingAmount, updateTime, orderStatus, exchangeText);

		if (!ackOrder)
		{
			m_logger->Error("Failed to update new order execution status for clientOrderId=" + clientOrderId + ", symbol=" + symbol);
			return;
		}

		if (ackOrder->GetOrderStatus() == BinanceNewOrderStatus::FULL_FILLED)
		{
			m_logger->Info("Received liquidated order ack for clientOrderId=" + clientOrderId + ", symbol=" + symbol);
		}
		else if (ackOrder->GetOrderStatus() == BinanceNewOrderStatus::PARTIAL_FILLED)
		{
			m_logger->Info("Received margin call order ack for clientOrderId=" + clientOrderId + ", symbol=" + symbol);
		}
		else if (ackOrder->GetOrderStatus() == BinanceNewOrderStatus::REJECTED)
		{
			m_logger->Info("Received rejected order ack for clientOrderId=" + clientOrderId + ", symbol=" + symbol);
		}

		// Updates the portfolio manager’s account information.
		m_portfolio->UpdateBinanceAccountInfo();
	}
	else if (simulatorAckType == FieldLabels::DownstreamAckTypes::CancelOrderAck ||
			 simulatorAckType == FieldLabels::DownstreamAckTypes::CancelledOrderAck)
	{
		// Updates the trader’s position.
		const auto clientOrderId = message.GetStringValueByTag(FieldLabels::ClientOrderId);
		const auto symbol = message.GetStringValueByTag(FieldLabels::Symbol);
		const auto updateTime = message.GetIntValueByTag(FieldLabels::UpdateTime);
		const auto exchangeText = message.GetStringValueByTag(FieldLabels::SimulatorAck::ExchangeText);
		const auto orderStatus = BinanceCancelOrder::GetOrderStatusEnum(
			message.GetStringValueByTag(FieldLabels::OrderStatus));
		auto* ackOrder = m_positionManager->UpdateOrderCancellingStatus(
			clientOrderId, symbol, updateTime, orderStatus, exchangeText);

		if (!ackOrder)
		{
			m_logger->Error("Failed to update new order execution status for clientOrderId=" + clientOrderId + ", symbol=" + symbol);
			return;
		}
	}
	else if (simulatorAckType == FieldLabels::DownstreamAckTypes::ReplaceOrderAck ||
			 simulatorAckType == FieldLabels::DownstreamAckTypes::ReplacedOrderAck)
	{
	}
	else if (simulatorAckType == FieldLabels::DownstreamAckTypes::QueryOrderAck ||
			 simulatorAckType == FieldLabels::DownstreamAckTypes::QueriedOrderAck)
	{
	}
	else if (simulatorAckType == FieldLabels::DownstreamAckTypes::ErrorOrderAck)
	{
	}
	else m_logger->Error("Received simulator ack with unknown type=" + simulatorAckType);
}
#endif

