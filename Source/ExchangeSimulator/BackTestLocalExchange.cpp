/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "BackTestLocalExchange.h"

#include <exception>

using namespace PortfolioManager;
using namespace OrderManagement;
using namespace ExchangeSimulator;

BackTestLocalExchange::~BackTestLocalExchange() {}

BackTestLocalExchange* BackTestLocalExchange::GetInstance()
{
    static BackTestLocalExchange instance;
    return &instance;
}

CryptoAssetLocalTest BackTestLocalExchange::ExecuteTestOrder(
    const TestOrder& buyOrder,
    const CryptoAssetLocalTest& currentUserAsset)
{
    // Very simple fill logic, just + - * / maths
    if (buyOrder.GetOrderSide() == TestOrderSide::BUY_SIDE)
    {
        const binapi::double_type buyAssetCash
            = buyOrder.GetAssetReferencePrice() * buyOrder.GetAssetQty();
        binapi::double_type newAssetCashBalance 
            = currentUserAsset.GetAssetCashBalance() - buyAssetCash;

        const binapi::double_type newAssetQty
            = currentUserAsset.GetAssetQty() + buyOrder.GetAssetQty();

        return CryptoAssetLocalTest(buyOrder.GetSymbol(), newAssetQty, newAssetCashBalance);
    }
    else if (buyOrder.GetOrderSide() == TestOrderSide::SELL_SIDE)
    {
        const binapi::double_type sellAssetCash
            = buyOrder.GetAssetReferencePrice() * buyOrder.GetAssetQty();
        binapi::double_type newAssetCashBalance
            = currentUserAsset.GetAssetCashBalance() + sellAssetCash;

        const binapi::double_type newAssetQty
            = currentUserAsset.GetAssetQty() - buyOrder.GetAssetQty();

        return CryptoAssetLocalTest(buyOrder.GetSymbol(), newAssetQty, newAssetCashBalance);
    }
    throw std::runtime_error("BackTestLocalExchange: Order side is invalid!");
}
