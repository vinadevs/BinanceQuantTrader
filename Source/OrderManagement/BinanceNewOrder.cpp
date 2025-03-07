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

BinanceNewOrder::BinanceNewOrder(
    const std::string& clientOrderId,
    const std::string& symbol,
    const binapi::e_side side,
    const binapi::e_type type,
    const binapi::e_time time,
    const double amount,
    const double price,
    const double stopPrice,
    const double icebergAmount)
    : Order(symbol, clientOrderId, BinanceOrderType::NEW),
    m_side(side),
    m_type(type),
    m_time(time),
    m_amount(amount),
    m_price(price),
    m_stopPrice(stopPrice),
    m_icebergAmount(icebergAmount) {}

BinanceNewOrder::~BinanceNewOrder() {}

std::string BinanceNewOrder::ToStringOrder() const
{
    return "BinanceNewOrder("
    "Symbol: " + m_symbol +
#if USE_BACK_TEST_TRADING
        "UserAccountID: " + m_userAccountID +
#endif
        ", Side: " + TypeToStringUtils::ToString(m_side) +
        ", Type: " + TypeToStringUtils::ToString(m_type) +
        ", Time: " + TypeToStringUtils::ToString(m_time) +
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
#if USE_BACK_TEST_TRADING
        "UserAccountID: " + m_userAccountID +
#endif
        ", Side: " + TypeToStringUtils::ToString(m_side) +
        ", Type: " + TypeToStringUtils::ToString(m_type) +
        ", Time: " + TypeToStringUtils::ToString(m_time) +
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

#if USE_BACK_TEST_TRADING
void BinanceNewOrder::SetSendingOrderResult(
    const MiddlewareMQ::MiddlewareMQResult& sendingOrderResult)
{
    m_sendingOrderResult = sendingOrderResult;
}

MiddlewareMQ::BqtJsonMessage BinanceNewOrder::ToBqtJsonMessageOrder() const
{
    MiddlewareMQ::BqtJsonMessage message;
    message.AddPair(FieldLabels::UserAccountID, m_userAccountID);
    message.AddPair(FieldLabels::BinanceOrderType, "BinanceNewOrder");
    message.AddPair(FieldLabels::Symbol, m_symbol);
    message.AddPair(FieldLabels::Side, TypeToStringUtils::ToString(m_side));
    message.AddPair(FieldLabels::Type, TypeToStringUtils::ToString(m_type));
    message.AddPair(FieldLabels::TimeInForce, TypeToStringUtils::ToString(m_time));
    message.AddPair(FieldLabels::Amount, GetAmountStr());
    message.AddPair(FieldLabels::LimitPrice, GetPriceStr());
    message.AddPair(FieldLabels::ClientOrderId, m_clientOrderId);
    message.AddPair(FieldLabels::StopPrice, GetStopPriceStr());
    message.AddPair(FieldLabels::IcebergAmount, GetIcebergAmountStr());
    message.AddPair(FieldLabels::OrderStatus, GetOrderStatusStr());
    message.AddPair(FieldLabels::UpdateTime, GetUpdateTimeStr());
    return message;
}

MiddlewareMQ::BqtJsonMessage BinanceNewOrder::ToBqtJsonMessageAck() const
{
    MiddlewareMQ::BqtJsonMessage message;
    message.AddPair(FieldLabels::UserAccountID, m_userAccountID);
    message.AddPair(FieldLabels::BinanceOrderType, "BinanceNewOrderAck");
    message.AddPair(FieldLabels::Symbol, m_symbol);
    message.AddPair(FieldLabels::Side, TypeToStringUtils::ToString(m_side));
    message.AddPair(FieldLabels::Type, TypeToStringUtils::ToString(m_type));
    message.AddPair(FieldLabels::TimeInForce, TypeToStringUtils::ToString(m_time));
    message.AddPair(FieldLabels::Amount, GetAmountStr());
    message.AddPair(FieldLabels::LimitPrice, GetPriceStr());
    message.AddPair(FieldLabels::ClientOrderId, m_clientOrderId);
    message.AddPair(FieldLabels::StopPrice, GetStopPriceStr());
    message.AddPair(FieldLabels::IcebergAmount, GetIcebergAmountStr());
    message.AddPair(FieldLabels::OrderStatus, GetOrderStatusStr());
    message.AddPair(FieldLabels::FilledAmount, GetFilledAmountStr());
    message.AddPair(FieldLabels::FilledPrice, GetFilledPriceStr());
    message.AddPair(FieldLabels::OrigQuoteOrderQuantity, GetOrigQuoteOrderQuantityStr());
    message.AddPair(FieldLabels::CummulativeQuoteQty, GetFilledAmountStr());
    message.AddPair(FieldLabels::UpdateTime, GetUpdateTimeStr());
    return message;
}
#else
void BinanceNewOrder::SetSendingOrderResult(
    const binapi::rest::api::result<binapi::rest::new_order_resp_type>& sendingOrderResult)
{
    m_sendingOrderResult = sendingOrderResult;
}
#endif

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
    case BinanceNewOrderStatus::CANCELLED:
        return "CANCELLED";
    case BinanceNewOrderStatus::REPLACED:
        return "REPLACED";
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
    else if (status == "CANCELLED") return BinanceNewOrderStatus::CANCELLED;
    else if (status == "REPLACED") return BinanceNewOrderStatus::REPLACED;
    return BinanceNewOrderStatus::UNDEF;
}

void BinanceNewOrder::SetOrderStatus(const BinanceNewOrderStatus status)
{
    m_orderStatus = status;
}