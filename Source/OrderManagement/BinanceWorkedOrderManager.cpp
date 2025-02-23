/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include "BinanceNewOrder.h"
#include "BinanceCancelOrder.h"
#include "BinanceReplaceOrder.h"
#include "BinanceQueryOrder.h"

#include "../LibraryUtils/Logger.h"

#include "BinanceWorkedOrderManager.h"

using namespace OrderManagement;

BinanceWorkedOrderManager::BinanceWorkedOrderManager()
    : m_logger{ std::make_unique<LibraryUtils::Logger>("BinanceWorkedOrderManager") } {}

BinanceWorkedOrderManager::~BinanceWorkedOrderManager() {}

void BinanceWorkedOrderManager::AddNewOrder(const std::string& clientOrderId, std::unique_ptr<BinanceNewOrder> order) {
    std::lock_guard<std::mutex> lock(m_mutex);
    const auto result = m_newWorkedOrders.try_emplace(clientOrderId, std::move(order));
    if (!result.second) { // Check if the insertion was successful
        LOG_WARNING_STREAM(m_logger, "Order with clientOrderId '" << clientOrderId << "' already exists.");
    }
}

void BinanceWorkedOrderManager::AddCancelOrder(const std::string& clientOrderId, std::unique_ptr<BinanceCancelOrder> order) {
    std::lock_guard<std::mutex> lock(m_mutex);
    const auto result = m_workedCancelOrders.try_emplace(clientOrderId, std::move(order));
    if (!result.second) {
        LOG_WARNING_STREAM(m_logger, "Cancel order with clientOrderId '" << clientOrderId << "' already exists.");
    }
}

void BinanceWorkedOrderManager::AddReplaceOrder(const std::string& clientOrderId, std::unique_ptr<BinanceReplaceOrder> order) {
    std::lock_guard<std::mutex> lock(m_mutex);
    const auto result = m_workedReplaceOrders.try_emplace(clientOrderId, std::move(order));
    if (!result.second) {
        LOG_WARNING_STREAM(m_logger, "Replace order with clientOrderId '" << clientOrderId << "' already exists.");
    }
}

// Remove an order by clientOrderId
void BinanceWorkedOrderManager::RemoveNewOrder(const std::string& clientOrderId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_newWorkedOrders.find(clientOrderId);
    if (it != m_newWorkedOrders.end()) {
        m_newWorkedOrders.erase(it);
        LOG_INFO_STREAM(m_logger, "Order with clientOrderId '" << clientOrderId << "' removed successfully.");
    }
    else {
        LOG_WARNING_STREAM(m_logger, "No order found with clientOrderId '" << clientOrderId << "'.");
    }
}

// Lookup an order by clientOrderId
BinanceNewOrder* BinanceWorkedOrderManager::LookupOrder(const std::string& clientOrderId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_newWorkedOrders.find(clientOrderId);
    if (it != m_newWorkedOrders.end()) {
        return it->second.get(); // Return the raw pointer
    }
    LOG_WARNING_STREAM(m_logger, "No order found with clientOrderId '" << clientOrderId << "'.");
    return nullptr; // Return nullptr if not found
}

// Remove a cancel order by clientOrderId
void BinanceWorkedOrderManager::RemoveCancelOrder(const std::string& clientOrderId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_workedCancelOrders.find(clientOrderId);
    if (it != m_workedCancelOrders.end()) {
        m_workedCancelOrders.erase(it);
        LOG_INFO_STREAM(m_logger, "Cancel order with clientOrderId '" << clientOrderId << "' removed successfully.");
    }
    else {
        LOG_WARNING_STREAM(m_logger, "No cancel order found with clientOrderId '" << clientOrderId << "'.");
    }
}

// Lookup a cancel order by clientOrderId
BinanceCancelOrder* BinanceWorkedOrderManager::LookupCancelOrder(const std::string& clientOrderId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_workedCancelOrders.find(clientOrderId);
    if (it != m_workedCancelOrders.end()) {
        return it->second.get(); // Return the raw pointer
    }
    LOG_WARNING_STREAM(m_logger, "No cancel order found with clientOrderId '" << clientOrderId << "'.");
    return nullptr; // Return nullptr if not found
}

// Remove a replace order by clientOrderId
void BinanceWorkedOrderManager::RemoveReplaceOrder(const std::string& clientOrderId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_workedReplaceOrders.find(clientOrderId);
    if (it != m_workedReplaceOrders.end()) {
        m_workedReplaceOrders.erase(it);
        LOG_INFO_STREAM(m_logger, "Replace order with clientOrderId '" << clientOrderId << "' removed successfully.");
    }
    else {
        LOG_WARNING_STREAM(m_logger, "No replace order found with clientOrderId '" << clientOrderId << "'.");
    }
}

// Lookup a replace order by clientOrderId
BinanceReplaceOrder* BinanceWorkedOrderManager::LookupReplaceOrder(const std::string& clientOrderId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_workedReplaceOrders.find(clientOrderId);
    if (it != m_workedReplaceOrders.end()) {
        return it->second.get(); // Return the raw pointer
    }
    LOG_WARNING_STREAM(m_logger, "No replace order found with clientOrderId '" << clientOrderId << "'.");
    return nullptr; // Return nullptr if not found
}

// Clear all orders
void BinanceWorkedOrderManager::ClearAll() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_newWorkedOrders.clear();
    m_workedCancelOrders.clear();
    m_workedReplaceOrders.clear();
    LOG_INFO_STREAM(m_logger, "All orders cleared successfully.");
}

// Clear all cancel orders
void BinanceWorkedOrderManager::ClearAllCancelOrders() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_workedCancelOrders.clear();
    LOG_INFO_STREAM(m_logger, "All cancel orders cleared successfully.");
}

// Clear all replace orders
void BinanceWorkedOrderManager::ClearAllReplaceOrders() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_workedReplaceOrders.clear();
    LOG_INFO_STREAM(m_logger, "All replace orders cleared successfully.");
}

// Example function to show all stored orders
void BinanceWorkedOrderManager::ShowOrders() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_newWorkedOrders.empty()) {
        LOG_INFO_STREAM(m_logger, "No orders available.");
        return;
    }
    for (const auto& pair : m_newWorkedOrders) {
        LOG_INFO_STREAM(m_logger, "Order clientOrderId: " << pair.first);
    }
}

// Example function to show all cancel orders
void BinanceWorkedOrderManager::ShowCancelOrders()  {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_workedCancelOrders.empty()) {
        LOG_INFO_STREAM(m_logger, "No cancel orders available.");
        return;
    }
    for (const auto& pair : m_workedCancelOrders) {
        LOG_INFO_STREAM(m_logger, "Cancel Order clientOrderId: " << pair.first);
    }
}

// Example function to show all replace orders
void BinanceWorkedOrderManager::ShowReplaceOrders() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_workedReplaceOrders.empty()) {
        LOG_INFO_STREAM(m_logger, "No replace orders available.");
        return;
    }
    for (const auto& pair : m_workedReplaceOrders) {
        LOG_INFO_STREAM(m_logger, "Replace Order clientOrderId: " << pair.first);
    }
}

// Getters to return the maps
const OrderWorkedList<BinanceNewOrder>& BinanceWorkedOrderManager::GetWorkedOrders() const {
    return m_newWorkedOrders;
}

const OrderWorkedList<BinanceCancelOrder>& BinanceWorkedOrderManager::GetWorkedCancelOrders() const {
    return m_workedCancelOrders;
}

const OrderWorkedList<BinanceReplaceOrder>& BinanceWorkedOrderManager::GetWorkedReplaceOrders() const {
    return m_workedReplaceOrders;
}