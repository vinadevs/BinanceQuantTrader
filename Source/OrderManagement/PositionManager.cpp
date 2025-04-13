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
      m_orderCreator(std::make_unique<OrderCreator>()),
      m_workedOrderManager{ std::make_unique<BinanceOrderManager>() } {
}

PositionManager::~PositionManager() {}

////////////////////// postion ////////////////////////////////////////

std::unique_ptr<BinanceNewOrder> PositionManager::OpenNewPositionUpstreamOrder(
    const QuantitativeModel::QuantOrderParammeter& param)
{
    const double icebergAmount = 0;
    const auto clientOrderId = GeneralUtils::GenerateUniqueID(StringDefinitions::BQTNewLongOrder);
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

void PositionManager::AddNewWorkedOrder(const std::string& clientOrderId, std::unique_ptr<BinanceNewOrder> order)
{
    m_workedOrderManager->AddNewOrder(clientOrderId, std::move(order));
}

void PositionManager::AddUnworkedOrder(const std::string& clientOrderId, std::unique_ptr<BinanceNewOrder> order)
{
	m_unworkedOrderManager->AddNewOrder(clientOrderId, std::move(order));
}         

void PositionManager::AddNewCancelOrder(const std::string& clientOrderId, std::unique_ptr<BinanceCancelOrder> order)
{
	m_workedOrderManager->AddCancelOrder(clientOrderId, std::move(order));
}

void PositionManager::AddUnworkedCancelOrder(const std::string& clientOrderId, std::unique_ptr<BinanceCancelOrder> order)
{
	m_unworkedOrderManager->AddCancelOrder(clientOrderId, std::move(order));
}

bool PositionManager::CloseOpenedPositionUpstreamOrder(const std::string& clientOrderId)
{
    return m_workedOrderManager->RemoveOrder(clientOrderId);
}

bool PositionManager::CloseAllOpenedPositions()
{
    return m_workedOrderManager->RemoveAllNewOrders();
}

bool PositionManager::CloseAllOpenedPositionsBySide(const binapi::e_side posSide)
{
    return m_workedOrderManager->RemoveNewOrderBySide(posSide);
}
