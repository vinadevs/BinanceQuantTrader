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
#include "../OrderManagement/BinanceQueryOrder.h"
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
    // IMPORTANT NOTES: NEVER USE 0 FOR NUMBERIC FIELDS, IF YOU DON'T WANT TO 
    // USE IT THEN JUST NEEDS TO SET IT'S STRING BE EMPTY NOT 0
    // BINANCE COULD REJECT ORDER THAT USE 0 
    const auto newOrderResult = BinanceApiGateWay->new_order(
        newOrder->GetSymbol().c_str()
        , newOrder->GetSide()
        , newOrder->GetType()
        , newOrder->GetTimeInForce()
        , binapi::e_trade_resp_type::FULL
        , newOrder->GetAmount() == 0 ? nullptr : newOrder->GetAmountStr().c_str()
        , newOrder->GetPrice() == 0 ? nullptr : newOrder->GetPriceStr().c_str()
        , newOrder->GetClientOrderId().c_str()
        , newOrder->GetStopPrice() == 0 ? nullptr : newOrder->GetStopPriceStr().c_str()
        , newOrder->GetIcebergAmount() == 0 ? nullptr : newOrder->GetIcebergAmountStr().c_str()
    );
    if (!static_cast<bool>(newOrderResult) 
        && !newOrderResult.v.is_full_response_type())
    {
        LOG_ERROR_STREAM(m_logger, "could NOT place a new [REAL] order="
            << *newOrder << ", reason=" << newOrderResult.errmsg);
    }
    else
    {
        LOG_INFO_STREAM(m_logger, "successfully placed a new [REAL] order=" << newOrderResult.v);
    }
    return newOrderResult;
}

// [BinanceExchangeConnectivity] [INFO] 
//	successfully placed a new [TEST] order = 
// { "standardCommissionForOrder":{"maker":0.001","taker":0.001",},
// "taxCommissionForOrder" : {"maker":0","taker":0",},"discount" : 
// {"discountAsset":BNB","discount":0.75",},"enabledForAccount" : 1",},
// "enabledForSymbol":1", }, }

binapi::rest::api::result<binapi::rest::new_order_resp_type>
BinanceExchangeConnectivity::SendNewBinanceTestOrderFull(
    OrderManagement::BinanceNewOrder* newTestOrder)
{
    // IMPORTANT NOTES: NEVER USE 0 FOR NUMBERIC FIELDS, IF YOU DON'T WANT TO 
    // USE IT THEN JUST NEEDS TO SET IT'S STRING BE EMPTY NOT 0
    // BINANCE COULD REJECT ORDER THAT USE 0 
    // Ensure that param is a valid const char*. 
    // If you are passing an object that isn't a C-style string, it might be an crashing issue.
    const auto newTestOrderResult = BinanceApiGateWay->new_test_order(
        newTestOrder->GetSymbol().c_str()
        , newTestOrder->GetSide()
        , newTestOrder->GetType()
        , newTestOrder->GetTimeInForce()
        , binapi::e_trade_resp_type::FULL
        , newTestOrder->GetAmount() == 0 ? nullptr : newTestOrder->GetAmountStr().c_str()
        , newTestOrder->GetPrice() == 0 ? nullptr : newTestOrder->GetPriceStr().c_str()
        , newTestOrder->GetClientOrderId().c_str()
        , newTestOrder->GetStopPrice() == 0 ? nullptr : newTestOrder->GetStopPriceStr().c_str()
        , newTestOrder->GetIcebergAmount() == 0 ? nullptr : newTestOrder->GetIcebergAmountStr().c_str()
    );
    if (!static_cast<bool>(newTestOrderResult) 
        && !newTestOrderResult.v.is_full_response_type())
    {
        LOG_ERROR_STREAM(m_logger, "could NOT place a new [TEST] order="
            << *newTestOrder << ", reason=" << newTestOrderResult.errmsg);
    }
    else
    {
        LOG_INFO_STREAM(m_logger, "successfully placed a new [TEST] order=" << newTestOrderResult.v);
    }
    return newTestOrderResult;
}

binapi::rest::api::result<binapi::rest::order_info_t>
BinanceExchangeConnectivity::QueryBinanceOrderStatus(
    OrderManagement::BinanceQueryOrder* queryOrder)
{
    const auto queryOrderResult = BinanceApiGateWay->order_info(
        queryOrder->GetSymbol(),
        queryOrder->GetOrderId(),
        queryOrder->GetOrigClientOrderId());
    if (!static_cast<bool>(queryOrderResult) 
        && queryOrderResult.v.status != StringDefinitions::BinanceExchangeCancelledStatus)
    {
        LOG_ERROR_STREAM(m_logger, "could NOT query [REAL] order="
            << queryOrder << ", reason=" << queryOrderResult.errmsg);
    }
    else
    {
        LOG_INFO_STREAM(m_logger, "successfully queried [REAL] order=" << queryOrderResult.v);
    }
    return queryOrderResult;
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
    if (!static_cast<bool>(cancelOrderResult) 
        && cancelOrderResult.v.status != StringDefinitions::BinanceExchangeCancelledStatus)
    {
        LOG_ERROR_STREAM(m_logger, "could NOT cancel [REAL] order="
            << cancelOrder << ", reason=" << cancelOrderResult.errmsg);
    }
    else
    {
        LOG_INFO_STREAM(m_logger, "successfully cancelled [REAL] order=" << cancelOrderResult.v);
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
    if (!static_cast<bool>(replaceOrder) 
        && replaceOrderResult.v.status != StringDefinitions::BinanceExchangeCancelledStatus)
    {
        LOG_ERROR_STREAM(m_logger, "could NOT replace [REAL] order="
            << replaceOrder << ", reason=" << replaceOrderResult.errmsg);
    }
    else
    {
        LOG_INFO_STREAM(m_logger, "successfully replaced [REAL] order=" << replaceOrderResult.v);
    }
    return replaceOrderResult;
}
