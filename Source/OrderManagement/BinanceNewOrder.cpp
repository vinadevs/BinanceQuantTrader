/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "BinanceNewOrder.h"
#include "FieldLabels.h"
#include "TypeToStringUtils.h"

using namespace OrderManagement;
using namespace MiddlewareMQ;

BinanceNewOrder::BinanceNewOrder(
    const std::string& clientOrderId,
    const std::string& symbol,
    const binapi::e_side side,
    const binapi::e_type type,
    const binapi::e_time timeInForce,
    const double amount,
    const double price,
    const double stopPrice,
    const double icebergAmount,
    const std::string& stableCurrency,
    const BinanceNewOrderTradingType tradingType,
    const ExchangeConnectivityType exchangeConnectivityType)
    : Order(
        symbol,
        clientOrderId,
        exchangeConnectivityType == ExchangeConnectivityType::TEST ? MessageType::TEST : MessageType::NEW),
    m_side(side),
    m_type(type),
    m_timeInForce(timeInForce),
    m_amount(amount),
    m_price(price),
    m_stopPrice(stopPrice),
    m_icebergAmount(icebergAmount),
	m_stableCurrency(stableCurrency),
    m_orderTradingType(tradingType) {}

BinanceNewOrder::BinanceNewOrder(
    const std::string& clientOrderId,
    const std::string& symbol,
    const binapi::e_side side,
    const binapi::e_type type, 
    const binapi::e_time time, 
    const double amount, const double price,
    const double stopPrice, const double icebergAmount, 
    const double lerverageRatio, 
    const std::string& stableCurrency,
    const BinanceNewOrderTradingType tradingType,
    const ExchangeConnectivityType exchangeConnectivityType)
	: Order(
		symbol,
		clientOrderId,
		exchangeConnectivityType == ExchangeConnectivityType::TEST ? MessageType::TEST : MessageType::NEW),
	m_side(side),
	m_type(type),
	m_timeInForce(time),
	m_amount(amount),
	m_price(price),
	m_stopPrice(stopPrice),
	m_icebergAmount(icebergAmount),
    m_futureOrderLeverageRatio(lerverageRatio),
	m_stableCurrency(stableCurrency),
	m_orderTradingType(tradingType) {}

BinanceNewOrder::~BinanceNewOrder() {}

std::string BinanceNewOrder::ToStringOrder() const
{
    if (m_orderTradingType == BinanceNewOrderTradingType::SPOT)
    {
        return "BinanceNewOrder("
            "Symbol: " + m_symbol +
            ", UserAccountID: " + m_userAccountID +
            ", Side: " + TypeToStringUtils::ToString(m_side) +
            ", Type: " + TypeToStringUtils::ToString(m_type) +
            ", TimeInForce: " + TypeToStringUtils::ToString(m_timeInForce) +
            ", Amount: " + GetAmountStr() +
            ", LimitPrice: " + GetPriceStr() +
            ", ClientOrderId: " + m_clientOrderId +
            ", StopPrice: " + GetStopPriceStr() +
            ", IcebergAmount: " + GetIcebergAmountStr() +
            ", OrderStatus: " + GetOrderStatusStr() +
            ", TradingType: " + GetOrderTradingTypeStr() +
            ", UpdateTime: " + GetUpdateTimeStr() +
            ", StableCurrency: " + m_stableCurrency +
            ")";
    }
	else if (m_orderTradingType == BinanceNewOrderTradingType::FUTURE)
    {
        return "BinanceNewOrder("
            "Symbol: " + m_symbol +
            ", UserAccountID: " + m_userAccountID +
            ", Side: " + TypeToStringUtils::ToString(m_side) +
            ", Type: " + TypeToStringUtils::ToString(m_type) +
            ", TimeInForce: " + TypeToStringUtils::ToString(m_timeInForce) +
            ", Amount: " + GetAmountStr() +
            ", LimitPrice: " + GetPriceStr() +
            ", ClientOrderId: " + m_clientOrderId +
            ", StopPrice: " + GetStopPriceStr() +
            ", IcebergAmount: " + GetIcebergAmountStr() +
            ", OrderStatus: " + GetOrderStatusStr() +
            ", TradingType: " + GetOrderTradingTypeStr() +
            ", UpdateTime: " + GetUpdateTimeStr() +
            ", FutureLeverageRatio: " + GetFutureLeverageRatioStr() +
			", FutureIsolatedMargin: " + GetIsolatedMarginStr() +
            ", StableCurrency: " + m_stableCurrency +
            ")";
    }
    else
    {
        return "BinanceNewOrder(UNDEF)";
    }
}

std::string BinanceNewOrder::ToStringAck() const
{
    if (m_orderTradingType == BinanceNewOrderTradingType::SPOT)
    {
        return "BinanceNewOrderAck("
            "Symbol: " + m_symbol +
            ", UserAccountID: " + m_userAccountID +
            ", Side: " + TypeToStringUtils::ToString(m_side) +
            ", Type: " + TypeToStringUtils::ToString(m_type) +
            ", TimeInForce: " + TypeToStringUtils::ToString(m_timeInForce) +
            ", Amount: " + GetAmountStr() +
            ", LimitPrice: " + GetPriceStr() +
            ", ClientOrderId: " + m_clientOrderId +
            ", StopPrice: " + GetStopPriceStr() +
            ", IcebergAmount: " + GetIcebergAmountStr() +
            ", OrderStatus: " + GetOrderStatusStr() +
            ", TradingType: " + GetOrderTradingTypeStr() +
            ", FilledAmount: " + GetFilledAmountStr() +
            ", FilledPrice: " + GetFilledPriceStr() +
			", ExchangeText: " + GetExchangeText() +
            ", RemainingAmount: " + GetOrigQuoteOrderQuantityStr() +
            ", OrigQuoteOrderQuantity: " + GetOrigQuoteOrderQuantityStr() +
            ", CummulativeQuoteQty: " + GetCumulativeQuoteQuantityStr() +
            ", StableCurrency: " + m_stableCurrency +
            ", UpdateTime: " + GetUpdateTimeStr() +
            ")";
    }
    else if (m_orderTradingType == BinanceNewOrderTradingType::FUTURE)
    {
        return "BinanceNewOrderAck("
            "Symbol: " + m_symbol +
            ", UserAccountID: " + m_userAccountID +
            ", Side: " + TypeToStringUtils::ToString(m_side) +
            ", Type: " + TypeToStringUtils::ToString(m_type) +
            ", TimeInForce: " + TypeToStringUtils::ToString(m_timeInForce) +
            ", Amount: " + GetAmountStr() +
            ", LimitPrice: " + GetPriceStr() +
            ", ClientOrderId: " + m_clientOrderId +
            ", StopPrice: " + GetStopPriceStr() +
            ", IcebergAmount: " + GetIcebergAmountStr() +
            ", OrderStatus: " + GetOrderStatusStr() +
            ", TradingType: " + GetOrderTradingTypeStr() +
            ", FutureLeverageRatio: " + GetFutureLeverageRatioStr() +
            ", FilledAmount: " + GetFilledAmountStr() +
            ", FilledPrice: " + GetFilledPriceStr() +
			", ExchangeText: " + GetExchangeText() +
            ", RemainingAmount: " + GetOrigQuoteOrderQuantityStr() +
            ", OrigQuoteOrderQuantity: " + GetOrigQuoteOrderQuantityStr() +
            ", CummulativeQuoteQty: " + GetCumulativeQuoteQuantityStr() +
            ", StableCurrency: " + m_stableCurrency +
            ", FutureInitialMarginPrice: " + GetFutureInitialMarginPriceStr() +
            ", FutureMaintainingMarginPrice: " + GetFutureMaintainingMarginPriceStr() +
            ", FutureLiquidationPrice: " + GetFutureLiquidationPriceStr() +
			", FutureIsolatedMargin: " + GetIsolatedMarginStr() +
            ", UpdateTime: " + GetUpdateTimeStr() +
            ")";
    }
    else
    {
        return "BinanceNewOrderAck(UNDEF)";
    }
}

BqtJsonMessage BinanceNewOrder::ToBqtJsonMessageOrder() const
{
    BqtJsonMessage message;
    message.AddTag(FieldLabels::MessageType, "BinanceNewOrder");
    message.AddTag(FieldLabels::UserAccountID, m_userAccountID);
    message.AddTag(FieldLabels::Symbol, m_symbol);
    message.AddTag(FieldLabels::Side, TypeToStringUtils::ToString(m_side));
    message.AddTag(FieldLabels::Type, TypeToStringUtils::ToString(m_type));
    message.AddTag(FieldLabels::TimeInForce, TypeToStringUtils::ToString(m_timeInForce));
    message.AddTag(FieldLabels::Amount, GetAmountStr());
    message.AddTag(FieldLabels::LimitPrice, GetPriceStr());
    message.AddTag(FieldLabels::ClientOrderId, m_clientOrderId);
    message.AddTag(FieldLabels::StopPrice, GetStopPriceStr());
    message.AddTag(FieldLabels::IcebergAmount, GetIcebergAmountStr());
    message.AddTag(FieldLabels::OrderStatus, GetOrderStatusStr());
    message.AddTag(FieldLabels::TradingType, GetOrderTradingTypeStr());
	message.AddTag(FieldLabels::StableCurrency, m_stableCurrency);
    message.AddTag(FieldLabels::UpdateTime, GetUpdateTimeStr());
    if (m_orderTradingType == BinanceNewOrderTradingType::FUTURE)
    {
        message.AddTag(FieldLabels::FutureLeverageRatio, GetFutureLeverageRatioStr());
		message.AddTag(FieldLabels::FutureIsolatedMargin, GetIsolatedMarginStr());
    }
    return message;
}

BqtJsonMessage BinanceNewOrder::ToBqtJsonMessageOrderAck() const
{
    BqtJsonMessage message;
    message.AddTag(FieldLabels::MessageType, "BinanceNewOrderAck");
    message.AddTag(FieldLabels::UserAccountID, m_userAccountID);
    message.AddTag(FieldLabels::Symbol, m_symbol);
    message.AddTag(FieldLabels::Side, TypeToStringUtils::ToString(m_side));
    message.AddTag(FieldLabels::Type, TypeToStringUtils::ToString(m_type));
    message.AddTag(FieldLabels::TimeInForce, TypeToStringUtils::ToString(m_timeInForce));
    message.AddTag(FieldLabels::Amount, GetAmountStr());
    message.AddTag(FieldLabels::LimitPrice, GetPriceStr());
    message.AddTag(FieldLabels::ClientOrderId, m_clientOrderId);
    message.AddTag(FieldLabels::StopPrice, GetStopPriceStr());
    message.AddTag(FieldLabels::IcebergAmount, GetIcebergAmountStr());
    message.AddTag(FieldLabels::OrderStatus, GetOrderStatusStr());
	message.AddTag(FieldLabels::TradingType, GetOrderTradingTypeStr());
    message.AddTag(FieldLabels::FilledAmount, GetFilledAmountStr());
    message.AddTag(FieldLabels::FilledPrice, GetFilledPriceStr());
    message.AddTag(FieldLabels::RemainingAmount, GetRemainingAmountStr());
    message.AddTag(FieldLabels::OrigQuoteOrderQuantity, GetOrigQuoteOrderQuantityStr());
    message.AddTag(FieldLabels::CummulativeQuoteQty, GetCumulativeQuoteQuantityStr());
	message.AddTag(FieldLabels::StableCurrency, m_stableCurrency);
    message.AddTag(FieldLabels::UpdateTime, GetUpdateTimeStr());
	if (m_orderTradingType == BinanceNewOrderTradingType::FUTURE)
	{
		message.AddTag(FieldLabels::FutureLeverageRatio, GetFutureLeverageRatioStr());
		message.AddTag(FieldLabels::FutureInitialMarginPrice, GetFutureInitialMarginPriceStr());
		message.AddTag(FieldLabels::FutureMaintainingMarginPrice, GetFutureMaintainingMarginPriceStr());
		message.AddTag(FieldLabels::FutureLiquidationPrice, GetFutureLiquidationPriceStr());
		message.AddTag(FieldLabels::FutureIsolatedMargin, GetIsolatedMarginStr());
	}
    return message;
}

BinanceNewOrderStatus BinanceNewOrder::GetOrderStatus() const
{
    return m_orderStatus;
}

std::string BinanceNewOrder::GetOrderStatusStr() const
{
    switch (m_orderStatus)
    {
    case BinanceNewOrderStatus::NEW:
        return "NEW";
    case BinanceNewOrderStatus::WAITING_FOR_FILL:
        return "WAITING_FOR_FILL";
    case BinanceNewOrderStatus::PARTIAL_FILLED:
        return "PARTIAL_FILLED";
    case BinanceNewOrderStatus::FULL_FILLED:
        return "FULL_FILLED";
	case BinanceNewOrderStatus::LIQUIDATED_POSITION:
        return "LIQUIDATED_POSITION";
	case BinanceNewOrderStatus::MARGIN_CALL_POSITION:
		return "MARGIN_CALL_POSITION";
	case BinanceNewOrderStatus::OPENING_POSITION:
		return "OPENING_POSITION";
	case BinanceNewOrderStatus::CLOSED_POSITION:
		return "CLOSED_POSITION";
	case BinanceNewOrderStatus::REJECTED:
		return "REJECTED";
	case BinanceNewOrderStatus::EXPIRED:
		return "EXPIRED";
	case BinanceNewOrderStatus::CANCELED:
		return "CANCELED";
	case BinanceNewOrderStatus::SKIPPED:
		return "SKIPPED";
    default:
        return "UNDEF";
    };
}

BinanceNewOrderStatus BinanceNewOrder::GetOrderStatusEnum(const std::string status)
{
    if (status == "NEW") return BinanceNewOrderStatus::NEW;
    else if (status == "WAITING_FOR_FILL") return BinanceNewOrderStatus::WAITING_FOR_FILL;
    else if (status == "PARTIAL_FILLED") return BinanceNewOrderStatus::PARTIAL_FILLED;
    else if (status == "FULL_FILLED") return BinanceNewOrderStatus::FULL_FILLED;
	else if (status == "LIQUIDATED_POSITION") return BinanceNewOrderStatus::LIQUIDATED_POSITION;
	else if (status == "MARGIN_CALL_POSITION") return BinanceNewOrderStatus::MARGIN_CALL_POSITION;
	else if (status == "OPENING_POSITION") return BinanceNewOrderStatus::OPENING_POSITION;
	else if (status == "CLOSED_POSITION") return BinanceNewOrderStatus::CLOSED_POSITION;
	else if (status == "REJECTED") return BinanceNewOrderStatus::REJECTED;
	else if (status == "EXPIRED") return BinanceNewOrderStatus::EXPIRED;
	else if (status == "CANCELED") return BinanceNewOrderStatus::CANCELED;
	else if (status == "SKIPPED") return BinanceNewOrderStatus::SKIPPED;
    return BinanceNewOrderStatus::UNDEF;
}

void BinanceNewOrder::SetOrderStatus(const BinanceNewOrderStatus status)
{
    m_orderStatus = status;
}

std::string BinanceNewOrder::GetOrderTradingTypeStr() const
{
	switch (m_orderTradingType)
	{
	case BinanceNewOrderTradingType::SPOT:
		return "SPOT";
	case BinanceNewOrderTradingType::FUTURE:
		return "FUTURE";
	default:
		return "UNDEF";
	};
}