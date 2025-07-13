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
            ", RemainingAmount: " + GetOrigQuoteOrderQuantityStr() +
            ", OrigQuoteOrderQuantity: " + GetOrigQuoteOrderQuantityStr() +
            ", CummulativeQuoteQty: " + GetCumulativeQuoteQuantityStr() +
            ", StableCurrency: " + m_stableCurrency +
            ", FutureInitialMarginPrice: " + GetFutureInitialMarginPriceStr() +
            ", FutureMaintainingMarginPrice: " + GetFutureMaintainingMarginPriceStr() +
            ", FutureLiquidationPrice: " + GetFutureLiquidationPriceStr() +
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
    message.AddPair(FieldLabels::UserAccountID, m_userAccountID);
    message.AddPair(FieldLabels::MessageType, "BinanceNewOrder");
    message.AddPair(FieldLabels::Symbol, m_symbol);
    message.AddPair(FieldLabels::Side, TypeToStringUtils::ToString(m_side));
    message.AddPair(FieldLabels::Type, TypeToStringUtils::ToString(m_type));
    message.AddPair(FieldLabels::TimeInForce, TypeToStringUtils::ToString(m_timeInForce));
    message.AddPair(FieldLabels::Amount, GetAmountStr());
    message.AddPair(FieldLabels::LimitPrice, GetPriceStr());
    message.AddPair(FieldLabels::ClientOrderId, m_clientOrderId);
    message.AddPair(FieldLabels::StopPrice, GetStopPriceStr());
    message.AddPair(FieldLabels::IcebergAmount, GetIcebergAmountStr());
    message.AddPair(FieldLabels::OrderStatus, GetOrderStatusStr());
    message.AddPair(FieldLabels::TradingType, GetOrderTradingTypeStr());
	message.AddPair(FieldLabels::StableCurrency, m_stableCurrency);
    message.AddPair(FieldLabels::UpdateTime, GetUpdateTimeStr());
    if (m_orderTradingType == BinanceNewOrderTradingType::FUTURE)
    {
        message.AddPair(FieldLabels::FutureLeverageRatio, GetFutureLeverageRatioStr());
    }
    return message;
}

BqtJsonMessage BinanceNewOrder::ToBqtJsonMessageOrderAck() const
{
    BqtJsonMessage message;
    message.AddPair(FieldLabels::UserAccountID, m_userAccountID);
    message.AddPair(FieldLabels::MessageType, "BinanceNewOrderAck");
    message.AddPair(FieldLabels::Symbol, m_symbol);
    message.AddPair(FieldLabels::Side, TypeToStringUtils::ToString(m_side));
    message.AddPair(FieldLabels::Type, TypeToStringUtils::ToString(m_type));
    message.AddPair(FieldLabels::TimeInForce, TypeToStringUtils::ToString(m_timeInForce));
    message.AddPair(FieldLabels::Amount, GetAmountStr());
    message.AddPair(FieldLabels::LimitPrice, GetPriceStr());
    message.AddPair(FieldLabels::ClientOrderId, m_clientOrderId);
    message.AddPair(FieldLabels::StopPrice, GetStopPriceStr());
    message.AddPair(FieldLabels::IcebergAmount, GetIcebergAmountStr());
    message.AddPair(FieldLabels::OrderStatus, GetOrderStatusStr());
	message.AddPair(FieldLabels::TradingType, GetOrderTradingTypeStr());
    message.AddPair(FieldLabels::FilledAmount, GetFilledAmountStr());
    message.AddPair(FieldLabels::FilledPrice, GetFilledPriceStr());
    message.AddPair(FieldLabels::RemainingAmount, GetRemainingAmountStr());
    message.AddPair(FieldLabels::OrigQuoteOrderQuantity, GetOrigQuoteOrderQuantityStr());
    message.AddPair(FieldLabels::CummulativeQuoteQty, GetCumulativeQuoteQuantityStr());
	message.AddPair(FieldLabels::StableCurrency, m_stableCurrency);
    message.AddPair(FieldLabels::UpdateTime, GetUpdateTimeStr());
	if (m_orderTradingType == BinanceNewOrderTradingType::FUTURE)
	{
		message.AddPair(FieldLabels::FutureLeverageRatio, GetFutureLeverageRatioStr());
		message.AddPair(FieldLabels::FutureInitialMarginPrice, GetFutureInitialMarginPriceStr());
		message.AddPair(FieldLabels::FutureMaintainingMarginPrice, GetFutureMaintainingMarginPriceStr());
		message.AddPair(FieldLabels::FutureLiquidationPrice, GetFutureLiquidationPriceStr());
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
    case BinanceNewOrderStatus::PRTIAL_FILLED:
        return "PRTIAL_FILLED";
    case BinanceNewOrderStatus::FULL_FILLED:
        return "FULL_FILLED";
    default:
        return "UNDEF";
    };
}

BinanceNewOrderStatus BinanceNewOrder::GetOrderStatusEnum(const std::string status)
{
    if (status == "NEW") return BinanceNewOrderStatus::NEW;
    else if (status == "WAITING_FOR_FILL") return BinanceNewOrderStatus::WAITING_FOR_FILL;
    else if (status == "PRTIAL_FILLED") return BinanceNewOrderStatus::PRTIAL_FILLED;
    else if (status == "FULL_FILLED") return BinanceNewOrderStatus::FULL_FILLED;
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