/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../OrderManagement/OrderCreator.h"
#include "../LibraryUtils/Logger.h"
#include "../LibraryUtils/GeneralUtils.h"
#include "../LibraryUtils/StringDefinitions.h"

#include "PositionManager.h"

using namespace OrderManagement;

PositionManager::PositionManager() 
    : m_logger{ std::make_unique<LibraryUtils::Logger>("PositionManager") },
      m_orderCreator(std::make_unique<OrderManagement::OrderCreator>()) {}

PositionManager::~PositionManager() {}

////////////////////// postion ////////////////////////////////////////

std::unique_ptr<BinanceNewOrder> PositionManager::OpenLongPositionUpstreamOrder(
	const std::string& symbol,
	const binapi::double_type quality,
	const binapi::double_type refPrice)
{
    const double stopPrice = 0;
    const double icebergAmount = 0;
    const auto clientOrderId = GeneralUtils::GenerateUniqueID(StringDefinitions::BQTNewLongOrder);
    m_workedPositions.try_emplace(clientOrderId, PositionSide::LONG);
    return m_orderCreator->CreateNewBinanceTestOrderFull(
          clientOrderId
        , symbol
        , binapi::e_side::buy
        , binapi::e_type::limit
        , binapi::e_time::GTC
        , quality.convert_to<double>()
        , refPrice.convert_to<double>()
        , stopPrice
        , icebergAmount);
}

std::unique_ptr<BinanceNewOrder> PositionManager::OpenShortPositionUpstreamOrder(
	const std::string& symbol,
	const binapi::double_type quality,
	const binapi::double_type refPrice)
{
    const double stopPrice = 0;
    const double icebergAmount = 0;
    const auto clientOrderId = GeneralUtils::GenerateUniqueID(StringDefinitions::BQTNewLongOrder);
    m_workedPositions.try_emplace(clientOrderId, PositionSide::SHORT);
    return m_orderCreator->CreateNewBinanceTestOrderFull(
          clientOrderId
        , symbol
        , binapi::e_side::buy
        , binapi::e_type::limit
        , binapi::e_time::GTC
        , quality.convert_to<double>()
        , refPrice.convert_to<double>()
        , stopPrice
        , icebergAmount);
}
