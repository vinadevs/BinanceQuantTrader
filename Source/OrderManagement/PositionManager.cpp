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

#include <boost/lexical_cast.hpp>

#include "PositionManager.h"

using namespace OrderManagement;

PositionManager::PositionManager() 
    : m_logger{ std::make_unique<LibraryUtils::Logger>("PositionManager") },
      m_orderCreator(std::make_unique<OrderManagement::OrderCreator>()) {}

PositionManager::~PositionManager() {}

////////////////////// postion ////////////////////////////////////////

std::unique_ptr<BinanceNewOrder> PositionManager::CreateLongPositionOrder(
	const std::string& symbol,
	const binapi::double_type quality,
	const binapi::double_type refPrice)
{
    const std::string stopPrice = "";
    const std::string icebergAmount = "";
    return m_orderCreator->CreateNewBinanceTestOrderFull(
          symbol
        , binapi::e_side::buy
        , binapi::e_type::limit
        , binapi::e_time::GTC
        , quality.convert_to<double>()
        , refPrice.convert_to<double>()
        , GeneralUtils::GenerateUniqueID(StringDefinitions::BQTNewLongOrder)
        , stopPrice
        , icebergAmount);
}

std::unique_ptr<BinanceNewOrder> PositionManager::CreateShortPositionOrder(
	const std::string& symbol,
	const binapi::double_type quality,
	const binapi::double_type refPrice)
{
    const std::string stopPrice = "";
    const std::string icebergAmount = "";
    return m_orderCreator->CreateNewBinanceTestOrderFull(
		  symbol
		, binapi::e_side::sell
		, binapi::e_type::limit
		, binapi::e_time::GTC
		, quality.convert_to<double>()
		, refPrice.convert_to<double>()
		, GeneralUtils::GenerateUniqueID(StringDefinitions::BQTNewShortOrder)
		, stopPrice
		, icebergAmount);
}

/////////////////////// order ///////////////////////////////////////

void PositionManager::AddNewOrder(const std::string& clientOrderId, std::unique_ptr<BinanceNewOrder> order) {
    std::lock_guard<std::mutex> lock(m_mutex);
    const auto result = m_newOrders.try_emplace(clientOrderId, std::move(order));
    if (!result.second) { // Check if the insertion was successful
        LOG_WARNING_STREAM(m_logger, "Order with clientOrderId '" << clientOrderId << "' already exists.");
    }
}

void PositionManager::AddCancelOrder(const std::string& clientOrderId, std::unique_ptr<BinanceCancelOrder> order) {
    std::lock_guard<std::mutex> lock(m_mutex);
    const auto result = m_cancelOrders.try_emplace(clientOrderId, std::move(order));
    if (!result.second) {
        LOG_WARNING_STREAM(m_logger, "Cancel order with clientOrderId '" << clientOrderId << "' already exists.");
    }
}

void PositionManager::AddReplaceOrder(const std::string& clientOrderId, std::unique_ptr<BinanceReplaceOrder> order) {
    std::lock_guard<std::mutex> lock(m_mutex);
    const auto result = m_replaceOrders.try_emplace(clientOrderId, std::move(order));
    if (!result.second) {
        LOG_WARNING_STREAM(m_logger, "Replace order with clientOrderId '" << clientOrderId << "' already exists.");
    }
}

// Remove an order by clientOrderId
void PositionManager::RemoveNewOrder(const std::string& clientOrderId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_newOrders.find(clientOrderId);
    if (it != m_newOrders.end()) {
        m_newOrders.erase(it);
        LOG_INFO_STREAM(m_logger, "Order with clientOrderId '" << clientOrderId << "' removed successfully.");
    }
    else {
        LOG_WARNING_STREAM(m_logger, "No order found with clientOrderId '" << clientOrderId << "'.");
    }
}

// Lookup an order by clientOrderId
BinanceNewOrder* PositionManager::LookupOrder(const std::string& clientOrderId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_newOrders.find(clientOrderId);
    if (it != m_newOrders.end()) {
        return it->second.get(); // Return the raw pointer
    }
    LOG_WARNING_STREAM(m_logger, "No order found with clientOrderId '" << clientOrderId << "'.");
    return nullptr; // Return nullptr if not found
}

// Remove a cancel order by clientOrderId
void PositionManager::RemoveCancelOrder(const std::string& clientOrderId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_cancelOrders.find(clientOrderId);
    if (it != m_cancelOrders.end()) {
        m_cancelOrders.erase(it);
        LOG_INFO_STREAM(m_logger, "Cancel order with clientOrderId '" << clientOrderId << "' removed successfully.");
    }
    else {
        LOG_WARNING_STREAM(m_logger, "No cancel order found with clientOrderId '" << clientOrderId << "'.");
    }
}

// Lookup a cancel order by clientOrderId
BinanceCancelOrder* PositionManager::LookupCancelOrder(const std::string& clientOrderId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_cancelOrders.find(clientOrderId);
    if (it != m_cancelOrders.end()) {
        return it->second.get(); // Return the raw pointer
    }
    LOG_WARNING_STREAM(m_logger, "No cancel order found with clientOrderId '" << clientOrderId << "'.");
    return nullptr; // Return nullptr if not found
}

// Remove a replace order by clientOrderId
void PositionManager::RemoveReplaceOrder(const std::string& clientOrderId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_replaceOrders.find(clientOrderId);
    if (it != m_replaceOrders.end()) {
        m_replaceOrders.erase(it);
        LOG_INFO_STREAM(m_logger, "Replace order with clientOrderId '" << clientOrderId << "' removed successfully.");
    }
    else {
        LOG_WARNING_STREAM(m_logger, "No replace order found with clientOrderId '" << clientOrderId << "'.");
    }
}

// Lookup a replace order by clientOrderId
BinanceReplaceOrder* PositionManager::LookupReplaceOrder(const std::string& clientOrderId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_replaceOrders.find(clientOrderId);
    if (it != m_replaceOrders.end()) {
        return it->second.get(); // Return the raw pointer
    }
    LOG_WARNING_STREAM(m_logger, "No replace order found with clientOrderId '" << clientOrderId << "'.");
    return nullptr; // Return nullptr if not found
}

// Clear all orders
void PositionManager::ClearAll() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_newOrders.clear();
    m_cancelOrders.clear();
    m_replaceOrders.clear();
    LOG_INFO_STREAM(m_logger, "All orders cleared successfully.");
}

// Clear all cancel orders
void PositionManager::ClearAllCancelOrders() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_cancelOrders.clear();
    LOG_INFO_STREAM(m_logger, "All cancel orders cleared successfully.");
}

// Clear all replace orders
void PositionManager::ClearAllReplaceOrders() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_replaceOrders.clear();
    LOG_INFO_STREAM(m_logger, "All replace orders cleared successfully.");
}

// Example function to show all stored orders
void PositionManager::ShowOrders() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_newOrders.empty()) {
        LOG_INFO_STREAM(m_logger, "No orders available.");
        return;
    }
    for (const auto& pair : m_newOrders) {
        LOG_INFO_STREAM(m_logger, "Order clientOrderId: " << pair.first);
    }
}

// Example function to show all cancel orders
void PositionManager::ShowCancelOrders() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_cancelOrders.empty()) {
        LOG_INFO_STREAM(m_logger, "No cancel orders available.");
        return;
    }
    for (const auto& pair : m_cancelOrders) {
        LOG_INFO_STREAM(m_logger, "Cancel Order clientOrderId: " << pair.first);
    }
}

// Example function to show all replace orders
void PositionManager::ShowReplaceOrders() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_replaceOrders.empty()) {
        LOG_INFO_STREAM(m_logger, "No replace orders available.");
        return;
    }
    for (const auto& pair : m_replaceOrders) {
        LOG_INFO_STREAM(m_logger, "Replace Order clientOrderId: " << pair.first);
    }
}

// Getters to return the maps
const OrderList<BinanceNewOrder>& PositionManager::GetOrders() const {
    return m_newOrders;
}

const OrderList<BinanceCancelOrder>& PositionManager::GetCancelOrders() const {
    return m_cancelOrders;
}

const OrderList<BinanceReplaceOrder>& PositionManager::GetReplaceOrders() const {
    return m_replaceOrders;
}