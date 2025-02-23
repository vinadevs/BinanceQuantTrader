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
#include "../ExchangeSimulator/BackTestLocalExchange.h"
#include "../ExchangeConnectivity/BinanceExchangeConnectivity.h"
#include "../RestAPI/RestAPI.h"
#include "../RestAPI/BinanceAPI.h"
#include "../StaticData/StaticDataManager.h"
#include "../OrderManagement/BinanceNewOrder.h"
#include "../OrderManagement/BinanceCancelOrder.h"
#include "../OrderManagement/BinanceReplaceOrder.h"
#include "../RiskManagement/RiskManager.h"

#include "../LibraryUtils/SourceBuildFlags.h"
#include "../LibraryUtils/StringUtils.h"
#if USE_TEST_TRADING
#include "../ExchangeConnectivity/ExchangeSimulatorConnectivity.h"
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
using namespace tinyxml2;

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
    m_logger->Info("querying Binance remote account info.");
    // Query all assets from remote Binance account and manage them locally for our trading
    m_portfolio->UpdateBinanceAccountInfo();
	m_positionManager = std::make_unique<PositionManager>();
	m_workedOrderManager = std::make_unique<BinanceWorkedOrderManager>();
	m_logger->Info("setting up config for trader.");
	SetupReporter(reportCfg);
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

bool BinanceTrader::Buy(
	const std::string& symbol,
	const binapi::double_type quality, 
	const binapi::double_type refPrice)
{
	if (m_portfolio->GetBinanceTradingPair(symbol)->GetCash(symbol) > quality * refPrice)
	{
		auto newSingleLongOrder = m_positionManager->OpenLongPositionUpstreamOrder(symbol, quality, refPrice);
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
		m_logger->Info("Sorry.. we are out of money, could not trade!");
		return false;
	}
}

bool BinanceTrader::Sell(
	const std::string& symbol,
	const binapi::double_type quality, 
	const binapi::double_type refPrice)
{
	if (m_portfolio->GetBinanceTradingPair(symbol)->GetQuantity() > 0)
	{
		auto newSingleShortOrder = m_positionManager->OpenShortPositionUpstreamOrder(symbol, quality, refPrice);
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
		m_logger->Info("Sorry.. we are out of asset, could not trade!");
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

void BinanceTrader::HandleDownstreamAckMessage(const MiddlewareMQ::BqtJsonMessage& message)
{
	m_logger->Info("Received simulator ack=" + message.SerializeMessage());
	// Updates the trader’s position and portfolio.
}
