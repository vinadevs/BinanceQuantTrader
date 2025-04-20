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
    const TradeType tradeType)
    : Order(symbol, clientOrderId,
        tradeType == TradeType::TEST ? MessageType::TEST : MessageType::NEW),
    m_side(side),
    m_type(type),
    m_timeInForce(timeInForce),
    m_amount(amount),
    m_price(price),
    m_stopPrice(stopPrice),
    m_icebergAmount(icebergAmount) {}

BinanceNewOrder::~BinanceNewOrder() {}

std::string BinanceNewOrder::ToStringOrder() const
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
        ", UpdateTime: " + GetUpdateTimeStr() +
        ")";
}

std::string BinanceNewOrder::ToStringAck() const
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
        ", FilledAmount: " + GetFilledAmountStr() +
        ", FilledAmount: " + GetFilledAmountStr() +
        ", FilledPrice: " + GetFilledPriceStr() +
        ", RemainingAmount: " + GetRemainingAmountStr() +
        ")";
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
    message.AddPair(FieldLabels::UpdateTime, GetUpdateTimeStr());
    return message;
}

BqtJsonMessage BinanceNewOrder::ToBqtJsonMessageAck() const
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
    message.AddPair(FieldLabels::FilledAmount, GetFilledAmountStr());
    message.AddPair(FieldLabels::FilledPrice, GetFilledPriceStr());
    message.AddPair(FieldLabels::OrigQuoteOrderQuantity, GetOrigQuoteOrderQuantityStr());
    message.AddPair(FieldLabels::CummulativeQuoteQty, GetCumulativeQuoteQuantityStr());
    message.AddPair(FieldLabels::UpdateTime, GetUpdateTimeStr());
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