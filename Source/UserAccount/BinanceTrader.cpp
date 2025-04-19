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
#include "../RestAPI/BinanceAPI.h"
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
		reportCfg, m_binanceAccountInfo.get(), m_tradingRules->GetExchangeProfileMgr(), m_portfolio);
#else
	m_exchangeReporter = std::make_unique<BinanceReporter>(
		reportCfg, m_binanceAccountInfo.get(), m_tradingRules->GetExchangeProfileMgr(), m_portfolio);
#endif
}

////////////// UPSTREAM PROCESSING /////////////////////////////

double BinanceTrader::CalculateTradeValue(
	const double quality,
	const double refPrice)
{
	return quality * refPrice;
}

bool BinanceTrader::CreateNewPosition(const QuantitativeModel::QuantOrderParammeter& param)
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
			return isSendingOrderSucceeded;
		}
		else
		{
			m_logger->Warning("User account has no stable coin available, could not create long (buy) position!");
			return false;
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
			return isSendingOrderSucceeded;
		}
		else
		{
			m_logger->Warning("User account has no asset available, could not create short (sell) position!");
			return false;
		}
	}
	return false;
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
	m_logger->Info("querying Binance remote account info...");
	// Query all assets from remote Binance account and manage them locally for our trading
	m_portfolio->SetUserAccountInfo(m_binanceAccountInfo.get());
	m_portfolio->UpdateBinanceAccountInfo();
	m_logger->Info("querying account info finished.");
}

////////////// DOWNSTREAM PROCESSING /////////////////////////////

#if USE_BACK_TEST_TRADING  
void BinanceTrader::HandleDownstreamAckMessage(const MiddlewareMQ::BqtJsonMessage& message)
{
	m_logger->Info("Received simulator ack=" + message.SerializeMessage());
	const std::string simulatorAckType = message.GetStringValueByTag(FieldLabels::SimulatorAck::AckType);
	if (simulatorAckType == FieldLabels::DownstreamAckTypes::NewOrderAck)
	{
		
	}
	else if (simulatorAckType == FieldLabels::DownstreamAckTypes::CancelOrderAck)
	{

	}
	else if (simulatorAckType == FieldLabels::DownstreamAckTypes::ReplaceOrderAck)
	{

	}
	else if (simulatorAckType == FieldLabels::DownstreamAckTypes::QueryOrderAck)
	{

	}
	else if (simulatorAckType == FieldLabels::DownstreamAckTypes::FilledNewOrderAck)
	{
		const std::string clientOrderId = message.GetStringValueByTag(FieldLabels::ClientOrderId);
		const std::string orderStatus = message.GetStringValueByTag(FieldLabels::OrderStatus);
		// Updates the trader’s position.
		if (BinanceNewOrder::GetOrderStatusEnum(orderStatus) == BinanceNewOrderStatus::FULL_FILLED &&
			m_positionManager->CloseOpenedPositionUpstreamOrder(clientOrderId))
		{
			m_logger->Info("Closed full filled upstream position for clientOrderId=" + clientOrderId);
			// Updates the trader’s portfolio, balancing asset holding.
			m_portfolio->UpdateBinanceAccountInfo();
		}
	}
	else if (simulatorAckType == FieldLabels::DownstreamAckTypes::CancelledOrderAck)
	{

	}
	else if (simulatorAckType == FieldLabels::DownstreamAckTypes::ReplacedOrderAck)
	{

	}
	else if (simulatorAckType == FieldLabels::DownstreamAckTypes::ErrorOrderAck)
	{

	}
	else m_logger->Error("Received simulator ack with unknown type=" + simulatorAckType);
}
#endif

