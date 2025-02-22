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
    const std::string& symbol,
    const binapi::e_side side,
    const binapi::e_type type,
    const binapi::e_time time,
    const double amount,
    const double price,
    const std::string& clientOrderId,
    const std::string& stopPrice,
    const std::string& icebergAmount)
    : Order(symbol, BinanceOrderType::NEW),
    m_side(side),
    m_type(type),
    m_time(time),
    m_amount(amount),
    m_price(price),
    m_clientOrderId(clientOrderId),
    m_stopPrice(stopPrice),
    m_icebergAmount(icebergAmount) {}

BinanceNewOrder::~BinanceNewOrder() {}

std::string BinanceNewOrder::ToString() const
{
    const std::string sideStr = (m_side == binapi::e_side::buy) ? "BUY" : "SELL";
    std::string typeStr;
    switch (m_type) 
    {
        case binapi::e_type::limit: typeStr = "LIMIT"; break;
        case binapi::e_type::market: typeStr = "MARKET"; break;
        case binapi::e_type::stop_loss: typeStr = "STOP_LOST"; break;
        case binapi::e_type::stop_loss_limit: typeStr = "STOP_LOST_LIMIT"; break;
        case binapi::e_type::take_profit: typeStr = "TAKE_PROFIT"; break;
        case binapi::e_type::take_profit_limit: typeStr = "TAKE_PROFIT_LIMIT"; break;
        case binapi::e_type::limit_maker: typeStr = "LIMIT_MAKER"; break;
    }
    std::string timeStr;
    switch (m_time) 
    {
        case binapi::e_time::GTC: timeStr = "GTC"; break;
        case binapi::e_time::IOC: timeStr = "IOC"; break;
        case binapi::e_time::FOK: timeStr = "FOK"; break;
    }
    return "BinanceNewOrder("
    "Symbol: " + m_symbol +
#if USE_TEST_TRADING
        "UserAccountID: " + m_userAccountID +
#endif
        ", Side: " + sideStr +
        ", Type: " + typeStr +
        ", Time: " + timeStr +
        ", Amount: " + GetAmountStr() +
        ", LimitPrice: " + GetPriceStr() +
        ", ClientOrderId: " + m_clientOrderId +
        ", StopPrice: " + m_stopPrice +
        ", IcebergAmount: " + m_icebergAmount +
        ", OrderStatus: " + GetOrderStatusStr() +
        ", FilledAmount: " + GetFilledAmountStr() +
        ", FilledPrice: " + GetFilledPriceStr() +
        ")";
}

#if USE_TEST_TRADING
void BinanceNewOrder::SetExecutionResult(
    const MiddlewareMQ::MiddlewareMQResult& executionResult)
{
    m_executionResult = executionResult;
}

MiddlewareMQ::BqtJsonMessage BinanceNewOrder::ToBqtJsonMessage() const
{
    MiddlewareMQ::BqtJsonMessage message;
    message.AddPair(FieldLabels::UserAccountID, m_userAccountID);
    message.AddPair(FieldLabels::BinanceOrderType, "BinanceNewOrder");
    message.AddPair(FieldLabels::Symbol, m_symbol);
    message.AddPair(FieldLabels::Side, TypeToStringUtils::ToString(m_side));
    message.AddPair(FieldLabels::Type, TypeToStringUtils::ToString(m_type));
    message.AddPair(FieldLabels::Time, TypeToStringUtils::ToString(m_time));
    message.AddPair(FieldLabels::Amount, GetAmountStr());
    message.AddPair(FieldLabels::LimitPrice, GetPriceStr());
    message.AddPair(FieldLabels::ClientOrderId, m_clientOrderId);
    message.AddPair(FieldLabels::StopPrice, m_stopPrice);
    message.AddPair(FieldLabels::IcebergAmount, m_icebergAmount);
    message.AddPair(FieldLabels::OrderStatus, GetOrderStatusStr());
    message.AddPair(FieldLabels::FilledAmount, GetFilledAmountStr());
    message.AddPair(FieldLabels::FilledPrice, GetFilledPriceStr());
    return message;
}
#else
void BinanceNewOrder::SetExecutionResult(
    const binapi::rest::api::result<binapi::rest::new_order_resp_type>& executionResult)
{
    m_executionResult = executionResult;
}
#endif

BinanceNewOrderStatus BinanceNewOrder::GetOrderStatus() const
{
    return m_orderStatus;
}

std::string OrderManagement::BinanceNewOrder::GetOrderStatusStr() const
{
    switch (m_orderStatus)
    {
    case OrderManagement::BinanceNewOrderStatus::NEW:
        return "NEW";
    case OrderManagement::BinanceNewOrderStatus::WAITING_FOR_FILL:
        return "WAITING_FOR_FILL";
    case OrderManagement::BinanceNewOrderStatus::PRTIAL_FILLED:
        return "PRTIAL_FILLED";
    case OrderManagement::BinanceNewOrderStatus::FULL_FILLED:
        return "FULL_FILLED";
    case OrderManagement::BinanceNewOrderStatus::CANCELLED:
        return "CANCELLED";
    case OrderManagement::BinanceNewOrderStatus::REPLACED:
        return "REPLACED";
    default:
        return "UNDEF";
    };
}

void BinanceNewOrder::SetOrderStatus(const BinanceNewOrderStatus status)
{
    m_orderStatus = status;
}