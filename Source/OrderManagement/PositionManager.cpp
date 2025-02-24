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

#include "BinanceWorkedOrderManager.h"
#include "PositionManager.h"

using namespace OrderManagement;

PositionManager::PositionManager(BinanceWorkedOrderManager* workedOrderManager)
    : m_logger{ std::make_unique<LibraryUtils::Logger>("PositionManager") },
      m_orderCreator(std::make_unique<OrderCreator>()),
      m_workedOrderManager(workedOrderManager){}

PositionManager::~PositionManager() {}

////////////////////// postion ////////////////////////////////////////

std::unique_ptr<BinanceNewOrder> PositionManager::OpenNewPositionUpstreamOrder(
	const std::string& symbol,
    const PositionSide posSide,
	const binapi::double_type quality,
	const binapi::double_type refPrice)
{
    const double stopPrice = 0;
    const double icebergAmount = 0;
    const auto clientOrderId = GeneralUtils::GenerateUniqueID(StringDefinitions::BQTNewLongOrder);
    m_workedPositions.try_emplace(clientOrderId, posSide);
    return m_orderCreator->CreateNewBinanceTestOrderFull(
          clientOrderId
        , symbol
        , posSide == PositionSide::LONG ? binapi::e_side::buy : binapi::e_side::sell
        , binapi::e_type::limit
        , binapi::e_time::GTC
        , quality.convert_to<double>()
        , refPrice.convert_to<double>()
        , stopPrice
        , icebergAmount);
}

bool PositionManager::CloseOpenedPositionUpstreamOrder(const std::string& clientOrderId)
{
    return CloseWorkedPosition(clientOrderId) && CloseWorkeOrder(clientOrderId);
}

bool PositionManager::CloseAllOpenedPositionUpstreamOrder(const PositionSide posSide, const PositionType posType)
{
    return false;
}

bool PositionManager::CloseWorkedPosition(const std::string& clientOrderId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_workedPositions.find(clientOrderId);
    if (it != m_workedPositions.end())
    {
        m_workedPositions.erase(it);
        return true;
    }
    else
    {
        m_logger->Error("No UpstramOrder found with symbol='" + clientOrderId + "'.");
    }
    return false;
}

bool PositionManager::CloseWorkeOrder(const std::string& clientOrderId)
{
    return m_workedOrderManager->RemoveNewOrder(clientOrderId);
}
