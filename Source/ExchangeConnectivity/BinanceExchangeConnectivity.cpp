/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../OrderManagement/BinanceNewOrder.h"
#include "../OrderManagement/BinanceCancelOrder.h"
#include "../OrderManagement/BinanceReplaceOrder.h"
#include "../RestAPI/BinanceAPI.h"
#include "../LibraryUtils/StringDefinitions.h"
#include "../LibraryUtils/Logger.h"

#include "BinanceExchangeConnectivity.h"

using namespace ExchangeConnectivity;
using namespace RestAPI;

BinanceExchangeConnectivity::BinanceExchangeConnectivity()
    : m_logger{ std::make_unique<LibraryUtils::Logger>("BinanceExchangeConnectivity") } {}

BinanceExchangeConnectivity::~BinanceExchangeConnectivity() {}

BinanceExchangeConnectivity* BinanceExchangeConnectivity::GetInstance()
{
    static BinanceExchangeConnectivity instance;
    return &instance;
}

binapi::rest::api::result<binapi::rest::new_order_resp_type>
BinanceExchangeConnectivity::SendNewBinanceOrderFull(
    OrderManagement::BinanceNewOrder* newOrder)
{
    const auto newOrderResult = BinanceApiGateWay->new_order(
        newOrder->GetSymbol()
        , newOrder->GetSide()
        , newOrder->GetType()
        , newOrder->GetTime()
        , binapi::e_trade_resp_type::FULL
        , newOrder->GetAmountStr()
        , newOrder->GetPriceStr()
        , newOrder->GetClientOrderId()
        , newOrder->GetStopPrice()
        , newOrder->GetIcebergAmount()
    );
    if (!static_cast<bool>(newOrderResult) && !newOrderResult.v.is_full_response_type())
    {
        LOG_ERROR_STREAM(m_logger, "could not place a new order=" << newOrderResult.v << ", reason=" << newOrderResult.errmsg);
    }
    else
    {
        LOG_INFO_STREAM(m_logger, "placed a new order=" << newOrderResult.v);
    }
    return newOrderResult;
}

binapi::rest::api::result<binapi::rest::new_order_resp_type>
BinanceExchangeConnectivity::SendNewBinanceTestOrderFull(
    OrderManagement::BinanceNewOrder* newTestOrder)
{
    const auto newTestOrderResult = BinanceApiGateWay->new_test_order(
        newTestOrder->GetSymbol()
        , newTestOrder->GetSide()
        , newTestOrder->GetType()
        , newTestOrder->GetTime()
        , binapi::e_trade_resp_type::FULL
        , newTestOrder->GetAmountStr()
        , newTestOrder->GetPriceStr()
        , newTestOrder->GetClientOrderId()
        , newTestOrder->GetStopPrice()
        , newTestOrder->GetIcebergAmount()
    );
    if (!static_cast<bool>(newTestOrderResult) && !newTestOrderResult.v.is_full_response_type())
    {
        LOG_ERROR_STREAM(m_logger, "could not place a new test order="
            << newTestOrderResult.v << ", reason=" << newTestOrderResult.errmsg);
    }
    else
    {
        LOG_INFO_STREAM(m_logger, "placed a new test order=" << newTestOrderResult.v);
    }
    return newTestOrderResult;
}

binapi::rest::api::result<binapi::rest::my_trades_info_t>
BinanceExchangeConnectivity::QueryBinanceOrderStatus(const std::string& symbol)
{
    return binapi::rest::api::result<binapi::rest::my_trades_info_t>();
}

binapi::rest::api::result<binapi::rest::cancel_order_info_t>
BinanceExchangeConnectivity::SendCancelBinanceOrder(
    OrderManagement::BinanceCancelOrder* cancelOrder)
{
    const auto cancelOrderResult = BinanceApiGateWay->cancel_order(
        cancelOrder->GetSymbol(),
        cancelOrder->GetOrderId(),
        cancelOrder->GetOrigClientOrderId(),
        cancelOrder->GetClientOrderId());
    if (!static_cast<bool>(cancelOrderResult) && cancelOrderResult.v.status != StringDefinitions::BinanceExchangeCancelledStatus)
    {
        LOG_ERROR_STREAM(m_logger, "could not cancel order="
            << cancelOrderResult.v << ", reason=" << cancelOrderResult.errmsg);
    }
    else
    {
        LOG_INFO_STREAM(m_logger, "cancelled order=" << cancelOrderResult.v);
    }
    return cancelOrderResult;
}

binapi::rest::api::result<binapi::rest::cancel_order_info_t>
BinanceExchangeConnectivity::SendCancelReplaceBinanceOrder(
    OrderManagement::BinanceReplaceOrder* replaceOrder)
{
    const auto replaceOrderResult = BinanceApiGateWay->cancel_order(
        replaceOrder->GetSymbol(),
        replaceOrder->GetOrderId(),
        replaceOrder->GetOrigClientOrderId(),
        replaceOrder->GetClientOrderId());
    if (!static_cast<bool>(replaceOrder) && replaceOrderResult.v.status != StringDefinitions::BinanceExchangeCancelledStatus)
    {
        LOG_ERROR_STREAM(m_logger, "could not cancel order="
            << replaceOrderResult.v << ", reason=" << replaceOrderResult.errmsg);
    }
    else
    {
        LOG_INFO_STREAM(m_logger, "cancelled order=" << replaceOrderResult.v);
    }
    return replaceOrderResult;
}
