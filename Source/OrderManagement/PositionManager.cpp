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
    const QuantitativeModel::QuantOrderParammeter& param)
{
    const double icebergAmount = 0;
    const auto clientOrderId = GeneralUtils::GenerateUniqueID(StringDefinitions::BQTNewLongOrder);
    m_workedPositions.try_emplace(clientOrderId, param.m_side);
    return m_orderCreator->CreateNewBinanceOrderFull(
          clientOrderId
        , param.m_symbol
        , param.m_side
        , param.m_type
        , param.m_time
        , param.m_amount
        , param.m_price
        , param.m_stopPrice
        , icebergAmount);
}

std::unique_ptr<BinanceNewOrder> PositionManager::OpenNewTestPositionUpstreamOrder(
    const QuantitativeModel::QuantOrderParammeter& param)
{
    const double icebergAmount = 0;
    const auto clientOrderId = GeneralUtils::GenerateUniqueID(StringDefinitions::BQTNewLongOrder);
    m_workedPositions.try_emplace(clientOrderId, param.m_side);
    return m_orderCreator->CreateNewBinanceTestOrderFull(
        clientOrderId
        , param.m_symbol
        , param.m_side
        , param.m_type
        , param.m_time
        , param.m_amount
        , param.m_price
        , param.m_stopPrice
        , icebergAmount);
}

bool PositionManager::CloseOpenedPositionUpstreamOrder(const std::string& clientOrderId)
{
    return CloseWorkedPosition(clientOrderId) && CloseWorkedOrder(clientOrderId);
}

bool PositionManager::CloseAllOpenedPositionUpstreamOrder(
    const binapi::e_side posSide, const PositionType posType)
{
	for (auto it = m_workedPositions.begin(); it != m_workedPositions.end();)
	{
		if (it->second == posSide)
		{
			CloseOpenedPositionUpstreamOrder(it->first);
			it = m_workedPositions.erase(it);
		}
		else
		{
			++it;
		}
	}
    return true;
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

bool PositionManager::CloseWorkedOrder(const std::string& clientOrderId)
{
    return m_workedOrderManager->RemoveOrder(clientOrderId);
}
