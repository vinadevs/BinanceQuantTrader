/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "OrderCreator.h"

#include "../RestAPI/BinanceAPI.h"

#include <iostream>

using namespace OrderManagement;
using namespace RestAPI;

std::unique_ptr<BinanceNewOrder> OrderCreator::CreateNewBinanceOrderFull(
      const std::string& symbol
    , const binapi::e_side side
    , const binapi::e_type type
    , const binapi::e_time time
    , const double amount
    , const double price
    , const std::string& clientOrderId
    , const std::string& stopPrice
    , const std::string& icebergAmount)
{
    return std::make_unique<BinanceNewOrder>(symbol, side, type, time, amount, price, clientOrderId, stopPrice, icebergAmount);
}

std::unique_ptr<BinanceNewOrder> OrderCreator::CreateNewBinanceTestOrderFull(
      const std::string& symbol
    , const binapi::e_side side
    , const binapi::e_type type
    , const binapi::e_time time
    , const double amount
    , const double price
    , const std::string& clientOrderId
    , const std::string& stopPrice
    , const std::string& icebergAmount)
{
    return std::make_unique<BinanceNewOrder>(symbol, side, type, time, amount, price, clientOrderId, stopPrice, icebergAmount);
}

std::unique_ptr<BinanceCancelOrder> OrderCreator::CreateCancelBinanceOrder(
      const std::string& symbol
    , const std::size_t orderId
    , const std::string& origClientOrderId
    , const std::string& clientOrderId)
{
    return std::make_unique<BinanceCancelOrder>(symbol, orderId, origClientOrderId, clientOrderId);
}

std::unique_ptr<BinanceReplaceOrder> OrderCreator::CreateCancelReplaceBinanceOrder(
      const std::string& symbol
    , const std::size_t orderId
    , const std::string& origClientOrderId
    , const std::string& clientOrderId)
{
    return std::make_unique<BinanceReplaceOrder>(symbol, orderId, origClientOrderId, clientOrderId);
}
