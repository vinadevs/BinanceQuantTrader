/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "OrderCreator.h"

#include "../RestAPI/BinanceSpotApiGateWay.h"

#include <iostream>

using namespace OrderManagement;
using namespace RestAPI;

std::unique_ptr<BinanceNewOrder> OrderCreator::CreateNewBinanceOrderFull(
      const std::string& clientOrderId
    , const std::string& symbol
    , const binapi::e_side side
    , const binapi::e_type type
    , const binapi::e_time time
    , const double amount
    , const double price
    , const double stopPrice
    , const double icebergAmount
	, const std::string& stableCurrency
    , const BinanceNewOrderTradingType tradingType)
{
    // last param is true mean this is real order
    return std::make_unique<BinanceNewOrder>(clientOrderId, symbol, side, type, time,
        amount, price, stopPrice, icebergAmount, stableCurrency, tradingType, ExchangeConnectivityType::REAL);
}

std::unique_ptr<BinanceNewOrder> OrderCreator::CreateNewBinanceFutureOrderFull(
    const std::string& clientOrderId, 
    const std::string& symbol,
    const binapi::e_side side,
    const binapi::e_type type,
    const binapi::e_time time,
    const double amount, 
    const double price,
    const double stopPrice,
    const double icebergAmount,
    const double leverage,
	const std::string& stableCurrency,
    const BinanceNewOrderTradingType tradingType)
{
	// last param is true mean this is real order
	return std::make_unique<BinanceNewOrder>(clientOrderId, symbol, side, type, time,
		amount, price, stopPrice, icebergAmount, leverage, stableCurrency, tradingType, ExchangeConnectivityType::REAL);
}

std::unique_ptr<BinanceNewOrder> OrderCreator::CreateNewBinanceTestOrderFull(
      const std::string& clientOrderId
    , const std::string& symbol
    , const binapi::e_side side
    , const binapi::e_type type
    , const binapi::e_time time
    , const double amount
    , const double price
    , const double stopPrice
    , const double icebergAmount
	, const std::string& stableCurrency
    , const BinanceNewOrderTradingType tradingType)
{
    // last param is false mean this is test order
    return std::make_unique<BinanceNewOrder>(clientOrderId, symbol, side, type, time,
        amount, price, stopPrice, icebergAmount, stableCurrency, tradingType, ExchangeConnectivityType::TEST);
}

std::unique_ptr<BinanceCancelOrder> OrderCreator::CreateCancelBinanceOrder(
      const std::string& clientOrderId
    , const std::string& symbol
    , const std::size_t orderId
    , const std::string& origClientOrderId)
{
    return std::make_unique<BinanceCancelOrder>(clientOrderId, symbol, orderId, origClientOrderId);
}

std::unique_ptr<BinanceReplaceOrder> OrderCreator::CreateReplaceBinanceOrder(
      const std::string& clientOrderId
    , const std::string& symbol
    , const std::size_t orderId
    , const std::string& origClientOrderId)
{
    return std::make_unique<BinanceReplaceOrder>(clientOrderId, symbol, orderId, origClientOrderId);
}

std::unique_ptr<BinanceQueryOrder> OrderCreator::CreateBinanceQueryOrder(
    const std::string& clientOrderId
    , const std::string& symbol
    , const std::size_t orderId
    , const std::string& origClientOrderId)
{
    return std::make_unique<BinanceQueryOrder>(clientOrderId, symbol, orderId, origClientOrderId);
}

