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
#include "../CurlAPI/BinanceFutureApiGateway.h"
#include "../StaticData/StaticDataManager.h"
#include "../OrderManagement/BinanceNewOrder.h"
#include "../OrderManagement/BinanceCancelOrder.h"
#include "../OrderManagement/BinanceReplaceOrder.h"
#include "../OrderManagement/FieldLabels.h"
#include "../RiskManagement/RiskManager.h"
#include "../ComplianceNRegulatory/BinanceTradingRules.h"
#include "../LibraryUtils/SourceBuildFlags.h"
#include "../LibraryUtils/StringUtils.h"
#include "../KernelTrading/user_future_account.h"
#if USE_BACK_TEST_TRADING
#include "../ExchangeConnectivity/ExchangeSimulatorConnectivity.h"
#include "../ExchangeSimulator/DownstreamOrderAck.h"
#include "BackTestReporter.h"
#else
#include "BinanceReporter.h"
#endif

#include "FutureTrader.h"

#include <exception>

using namespace UserAccount;
using namespace PortfolioManager;
using namespace ComplianceNRegulatory;
using namespace RiskManagement;
using namespace OrderManagement;
using namespace StaticData;
using namespace ExchangeConnectivity;
#if USE_BACK_TEST_TRADING
using namespace ExchangeSimulator;
#endif
using namespace tinyxml2;

static constexpr double ZERO_DOUBLE_VALUE = 0;

FutureTrader::FutureTrader(
	const XMLElement* reportCfg,
	PortfolioInvestmentBinance* portfolio,
	BinanceTradingRules* tradingRules,
	RiskManager* riskManager)
	: m_portfolio(portfolio),
	m_tradingRules(tradingRules),
	m_riskManager(riskManager)
{
	m_logger = std::make_unique<LibraryUtils::Logger>("FutureTrader");
	m_logger->Info("using FutureTrader.");
	m_binanceAccountInfo = std::make_unique<KernelTrading::UserFutureAccount>();
	m_positionManager = std::make_unique<PositionManager>();
//#if USE_BACK_TEST_TRADING
//	m_exchangeReporter = std::make_unique<BackTestReporter>(
//		reportCfg, m_binanceAccountInfo.get(), m_tradingRules->GetExchangeProfileMgr(), m_positionManager.get());
//#else
//	m_exchangeReporter = std::make_unique<BinanceReporter>(
//		reportCfg, m_binanceAccountInfo.get(), m_tradingRules->GetExchangeProfileMgr(), m_positionManager.get());
//#endif
}

////////////// UPSTREAM PROCESSING /////////////////////////////

double FutureTrader::CalculateTradeValue(
	const double quality,
	const double refPrice)
{
	return quality * refPrice;
}

bool FutureTrader::CreateNewPosition(const QuantitativeModel::QuantOrderParammeter& param)
{
#if USE_BACK_TEST_TRADING
	if (param.m_side == binapi::e_side::buy)
	{
		if (m_binanceAccountInfo->CanTrade() &&
			m_binanceAccountInfo->GetTotalWalletBalance()
			> CalculateTradeValue(param.m_amount, param.m_price))
		{
			auto newSingleLongOrder = m_positionManager->OpenNewPositionUpstreamOrder(param);
			const auto result = ExchangeSimulatorGateWay->SendNewSimulatorOrderFull(newSingleLongOrder.get());

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
			m_logger->Warning("User future account has no stable coin available, could not create long (buy) position for=" + param.m_symbol);
			return false;
		}
	}
	else if (param.m_side == binapi::e_side::sell)
	{
		if (m_portfolio->GetBinanceFuturePositionInfo(param.m_symbol).positionAmt > ZERO_DOUBLE_VALUE)
		{
			auto newSingleShortOrder = m_positionManager->OpenNewPositionUpstreamOrder(param);
			const auto result = ExchangeSimulatorGateWay->SendNewSimulatorOrderFull(newSingleShortOrder.get());

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
			m_logger->Warning("User future has no asset available, could not create short (sell) position for=" + param.m_symbol);
			return false;
		}
	}
#endif
	return false;
}

bool FutureTrader::CancelAllOpenPositions(const std::string& symbol)
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

void FutureTrader::UpdateAccountInfo()
{
	m_portfolio->UpdateBinanceFutureAccountInfo();
}

void FutureTrader::ReportTradeResults(const std::string& symbol)
{
	//m_exchangeReporter->DoTradeExecutionReport(symbol);
}

void FutureTrader::CreatePortfolioManagement(const std::vector<std::string>&targetTradeSymbols)
{
	m_logger->Info("querying Binance remote future account info...");
	// Query all assets from remote Binance account and manage them locally for our trading
	m_portfolio->SetUserFutureAccountInfo(m_binanceAccountInfo.get());
	m_portfolio->UpdateBinanceAccountInfo();
	m_logger->Info("querying account info finished.");
}

////////////// DOWNSTREAM PROCESSING /////////////////////////////

#if USE_BACK_TEST_TRADING  
void FutureTrader::HandleDownstreamAckMessage(const MiddlewareMQ::BqtJsonMessage & message)
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
		const auto orderStatus = BinanceNewOrder::GetOrderStatusEnum(
			message.GetStringValueByTag(FieldLabels::OrderStatus));
		m_positionManager->UpdateNewOrderExecutionStatus(
			clientOrderId, symbol, filledAmount, filledPrice, remainingAmount, updateTime, orderStatus);
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
		const auto orderStatus = BinanceCancelOrder::GetOrderStatusEnum(
			message.GetStringValueByTag(FieldLabels::OrderStatus));
		m_positionManager->UpdateOrderCancellingStatus(
			clientOrderId, symbol, updateTime, orderStatus);
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

