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

#include "../LibraryUtils/SourceBuildFlags.h"
#include "../LibraryUtils/StringUtils.h"
#if USE_TEST_TRADING
#include "../ExchangeConnectivity/ExchangeSimulatorConnectivity.h"
#include "../ExchangeSimulator/DownstreamOrderAck.h"
#endif

#include "BinanceTrader.h"

#include <exception>

using namespace UserAccount;
using namespace PortfolioManager;
using namespace RiskManagement;
using namespace OrderManagement;
using namespace RestAPI;
using namespace StaticData;
using namespace ExchangeConnectivity;
#if USE_TEST_TRADING
using namespace ExchangeSimulator;
#endif
using namespace tinyxml2;

static constexpr double ZERO_DOUBLE_VALUE = 0;

BinanceTrader::BinanceTrader(
	const XMLElement* reportCfg,
	PortfolioInvestmentBinance* portfolio,
	RiskManager* riskManager)
    : m_portfolio(portfolio),
	  m_riskManager(riskManager),
	  m_reporter{ std::make_unique<BinanceReporter>() }
{
    m_logger = std::make_unique<LibraryUtils::Logger>("BinanceTrader");
    m_logger->Info("using BinanceTrader.");
	m_binanceAccountInfo = std::make_unique<binapi::rest::account_info_t>();
	m_workedOrderManager = std::make_unique<BinanceWorkedOrderManager>();
	m_positionManager = std::make_unique<PositionManager>(m_workedOrderManager.get());
	m_logger->Info("querying Binance remote account info...");
	// Query all assets from remote Binance account and manage them locally for our trading
	m_portfolio->SetUserAccountInfo(m_binanceAccountInfo.get());
	m_portfolio->UpdateBinanceAccountInfo();
	m_logger->Info("querying account info finished.");
	SetupReporter(reportCfg);
	m_logger->Info("setting up trading reporter finished.");
}

////////////// UPSTREAM PROCESSING /////////////////////////////

void BinanceTrader::SetupReporter(const XMLElement* reportCfg)
{
	if (reportCfg->BoolAttribute("TradeReport"))
	{
		m_enableTradeReporter = true;
	}
	if (reportCfg->BoolAttribute("OrderReport"))
	{
		m_enableOpenOrderReporter = true;
	}
	if (reportCfg->BoolAttribute("BalanceReport"))
	{
		m_enableBalanceReporter = true;
	}
}

binapi::double_type BinanceTrader::CalculateTradeValue(
	const binapi::double_type quality,
	const binapi::double_type refPrice)
{
	return quality * refPrice;
}

bool BinanceTrader::CreateLongPosition(
	const std::string& symbol,
	const binapi::double_type quality, 
	const binapi::double_type refPrice)
{
	if (m_binanceAccountInfo->stableCoinAmount > CalculateTradeValue(quality, refPrice))
	{
		auto newSingleLongOrder = m_positionManager->OpenNewPositionUpstreamOrder(symbol, PositionSide::LONG, quality, refPrice);
#if USE_TEST_TRADING
		const auto result = ExchangeSimulatorGateWay->SendNewSimulatorOrderFull(newSingleLongOrder.get());
#else
		const auto result = BinanceExchangeGateWay->SendNewBinanceOrderFull(newSingleLongOrder.get());
#endif
		newSingleLongOrder->SetSendingOrderResult(result);
		const auto clientOrderId = newSingleLongOrder->GetClientOrderId();
		m_workedOrderManager->AddNewOrder(clientOrderId, std::move(newSingleLongOrder));
		return static_cast<bool>(result);
	}
	else
	{
		m_logger->Info("User account has no stable coin available, could not trade!");
		return false;
	}
}

bool BinanceTrader::CreateShortPosition(
	const std::string& symbol,
	const binapi::double_type quality, 
	const binapi::double_type refPrice)
{
	if (m_portfolio->GetBinanceTradingPair(symbol)->GetQuantity() > ZERO_DOUBLE_VALUE)
	{
		auto newSingleShortOrder = m_positionManager->OpenNewPositionUpstreamOrder(symbol, PositionSide::SHORT, quality, refPrice);
#if USE_TEST_TRADING
		const auto result = ExchangeSimulatorGateWay->SendNewSimulatorOrderFull(newSingleShortOrder.get());
#else
		const auto result = BinanceExchangeGateWay->SendNewBinanceOrderFull(newSingleShortOrder.get());
#endif
		newSingleShortOrder->SetSendingOrderResult(result);
		const auto clientOrderId = newSingleShortOrder->GetClientOrderId();
		m_workedOrderManager->AddNewOrder(clientOrderId, std::move(newSingleShortOrder));
		return static_cast<bool>(result);
	}
	else
	{
		m_logger->Info("User account has no asset available, could not trade!");
		return false;
	}
}

void BinanceTrader::ReportTradeData(const std::string& symbol)
{
	if (m_enableTradeReporter)
	{
		m_reporter->ReportTrades(symbol);
	}
	if (m_enableOpenOrderReporter)
	{
		m_reporter->ReportOpenOrders(symbol);
	}
	if (m_enableBalanceReporter)
	{
		m_reporter->ReportAccountBalance(symbol);
	}
}

void BinanceTrader::UpdateAccountInfo()
{
	m_portfolio->UpdateBinanceAccountInfo();
}

////////////// DOWNSTREAM PROCESSING /////////////////////////////

#if USE_TEST_TRADING  
void BinanceTrader::HandleDownstreamAckMessage(const MiddlewareMQ::BqtJsonMessage& message)
{
	m_logger->Info("Received simulator ack=" + message.SerializeMessage());
	const std::string simulatorAckType = message.GetStringValueByTag(FieldLabels::SimulatorAck::AckType);
	if (simulatorAckType == DownstreamAckTypes::NewOrderAck)
	{
		
	}
	else if (simulatorAckType == DownstreamAckTypes::CancelOrderAck)
	{

	}
	else if (simulatorAckType == DownstreamAckTypes::ReplaceOrderAck)
	{

	}
	else if (simulatorAckType == DownstreamAckTypes::QueryOrderAck)
	{

	}
	else if (simulatorAckType == DownstreamAckTypes::FilledNewOrderAck)
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
			// Updates PNL report.

		}
	}
	else if (simulatorAckType == DownstreamAckTypes::CancelledOrderAck)
	{

	}
	else if (simulatorAckType == DownstreamAckTypes::ReplacedOrderAck)
	{

	}
	else if (simulatorAckType == DownstreamAckTypes::ErrorOrderAck)
	{

	}
	m_logger->Error("Received simulator ack with unknown type=" + simulatorAckType);
}
#endif
