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
#include "../KernelTrading/double_defaults.h"

#include "BinanceOrderManager.h"

using namespace OrderManagement;

BinanceOrderManager::BinanceOrderManager()
    : m_logger{ std::make_unique<LibraryUtils::Logger>("BinanceOrderManager") } {}

BinanceOrderManager::~BinanceOrderManager() {}

void BinanceOrderManager::AddNewOrder(const std::string& clientOrderId, std::unique_ptr<BinanceNewOrder> order) {
    std::lock_guard<std::mutex> lock(m_mutex);
    const auto result = m_newOrders.try_emplace(clientOrderId, std::move(order));
    if (!result.second) { // Check if the insertion was successful
        LOG_WARNING_STREAM(m_logger, "New order with clientOrderId '" << clientOrderId << "' already exists.");
    }
}

void BinanceOrderManager::AddCancelOrder(const std::string& clientOrderId, std::unique_ptr<BinanceCancelOrder> order) {
    std::lock_guard<std::mutex> lock(m_mutex);
    const auto result = m_cancelOrders.try_emplace(clientOrderId, std::move(order));
    if (!result.second) {
        LOG_WARNING_STREAM(m_logger, "Cancel order with clientOrderId '" << clientOrderId << "' already exists.");
    }
}

void BinanceOrderManager::AddReplaceOrder(const std::string& clientOrderId, std::unique_ptr<BinanceReplaceOrder> order) {
    std::lock_guard<std::mutex> lock(m_mutex);
    const auto result = m_replaceOrders.try_emplace(clientOrderId, std::move(order));
    if (!result.second) {
        LOG_WARNING_STREAM(m_logger, "Replace order with clientOrderId '" << clientOrderId << "' already exists.");
    }
}

void BinanceOrderManager::AddQueryOrder(const std::string& clientOrderId, std::unique_ptr<BinanceQueryOrder> order)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	const auto result = m_queryOrders.try_emplace(clientOrderId, std::move(order));
	if (!result.second) {
		LOG_WARNING_STREAM(m_logger, "Query order with clientOrderId '" << clientOrderId << "' already exists.");
	}
}

// Remove an order by clientOrderId
bool BinanceOrderManager::RemoveOrder(const std::string& clientOrderId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_newOrders.find(clientOrderId);
    if (it != m_newOrders.end()) {
        m_newOrders.erase(it);
        LOG_INFO_STREAM(m_logger, "New order with clientOrderId '" << clientOrderId << "' removed successfully.");
        return true;
    }
    LOG_WARNING_STREAM(m_logger, "No new order found with clientOrderId '" << clientOrderId << "'.");
    return false;
}

bool BinanceOrderManager::RemoveAllNewOrders()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_newOrders.clear();
	LOG_INFO_STREAM(m_logger, "All new orders removed successfully.");
	return true;   
}

bool BinanceOrderManager::RemoveNewOrderBySide(const binapi::e_side side)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto it = m_newOrders.begin(); it != m_newOrders.end(); ) {
        if (it->second->GetSide() == side) {
            it = m_newOrders.erase(it);
            LOG_INFO_STREAM(m_logger, "New order with side '" << static_cast<int>(side) << "' removed successfully.");
        }
        else {
            ++it;
        }
    }
    return true;
}

// Lookup an order by clientOrderId
BinanceNewOrder* BinanceOrderManager::LookupOrder(const std::string& clientOrderId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_newOrders.find(clientOrderId);
    if (it != m_newOrders.end()) {
        return it->second.get(); // Return the raw pointer
    }
    LOG_WARNING_STREAM(m_logger, "No new order found with clientOrderId '" << clientOrderId << "'.");
    return nullptr; // Return nullptr if not found
}

// Remove a cancel order by clientOrderId
bool BinanceOrderManager::RemoveCancelOrder(const std::string& clientOrderId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_cancelOrders.find(clientOrderId);
    if (it != m_cancelOrders.end()) {
        m_cancelOrders.erase(it);
        LOG_INFO_STREAM(m_logger, "Cancel order with clientOrderId '" << clientOrderId << "' removed successfully.");
        return true;
    }
    LOG_WARNING_STREAM(m_logger, "No cancel order found with clientOrderId '" << clientOrderId << "'.");
    return false;
}

// Lookup a cancel order by clientOrderId
BinanceCancelOrder* BinanceOrderManager::LookupCancelOrder(const std::string& clientOrderId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_cancelOrders.find(clientOrderId);
    if (it != m_cancelOrders.end()) {
        return it->second.get(); // Return the raw pointer
    }
    LOG_WARNING_STREAM(m_logger, "No cancel order found with clientOrderId '" << clientOrderId << "'.");
    return nullptr; // Return nullptr if not found
}

// Remove a replace order by clientOrderId
bool BinanceOrderManager::RemoveReplaceOrder(const std::string& clientOrderId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_replaceOrders.find(clientOrderId);
    if (it != m_replaceOrders.end()) {
        m_replaceOrders.erase(it);
        LOG_INFO_STREAM(m_logger, "Replace order with clientOrderId '" << clientOrderId << "' removed successfully.");
        return true;
    }
    LOG_WARNING_STREAM(m_logger, "No replace order found with clientOrderId '" << clientOrderId << "'.");
    return false;
}

// Lookup a replace order by clientOrderId
BinanceReplaceOrder* BinanceOrderManager::LookupReplaceOrder(const std::string& clientOrderId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_replaceOrders.find(clientOrderId);
    if (it != m_replaceOrders.end()) {
        return it->second.get(); // Return the raw pointer
    }
    LOG_WARNING_STREAM(m_logger, "No replace order found with clientOrderId '" << clientOrderId << "'.");
    return nullptr; // Return nullptr if not found
}

BinanceQueryOrder* BinanceOrderManager::LookupQueryOrder(const std::string& clientOrderId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	auto it = m_queryOrders.find(clientOrderId);
	if (it != m_queryOrders.end()) {
		return it->second.get(); // Return the raw pointer
	}
	LOG_WARNING_STREAM(m_logger, "No query order found with clientOrderId '" << clientOrderId << "'.");
	return nullptr; // Return nullptr if not found
}

bool BinanceOrderManager::RemoveQueryOrder(const std::string& clientOrderId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	auto it = m_queryOrders.find(clientOrderId);
	if (it != m_queryOrders.end()) {
		m_queryOrders.erase(it);
		LOG_INFO_STREAM(m_logger, "Query order with clientOrderId '" << clientOrderId << "' removed successfully.");
		return true;
	}
	LOG_WARNING_STREAM(m_logger, "No query order found with clientOrderId '" << clientOrderId << "'.");
	return false;
}

// Clear all orders
void BinanceOrderManager::ClearAll() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_newOrders.clear();
    m_cancelOrders.clear();
    m_replaceOrders.clear();
	m_queryOrders.clear();
    LOG_INFO_STREAM(m_logger, "All orders cleared successfully.");
}

void BinanceOrderManager::ClearAllNewOrders()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_newOrders.clear();
    LOG_INFO_STREAM(m_logger, "All new orders cleared successfully.");
}

// Clear all cancel orders
void BinanceOrderManager::ClearAllCancelOrders() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_cancelOrders.clear();
    LOG_INFO_STREAM(m_logger, "All cancel orders cleared successfully.");
}

// Clear all replace orders
void BinanceOrderManager::ClearAllReplaceOrders() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_replaceOrders.clear();
    LOG_INFO_STREAM(m_logger, "All replace orders cleared successfully.");
}

// Example function to show all stored orders
void BinanceOrderManager::ShowOrders() {
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
void BinanceOrderManager::ShowCancelOrders()  {
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
void BinanceOrderManager::ShowReplaceOrders() {
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
const OrderList<BinanceNewOrder>& BinanceOrderManager::GetOrders() const {
    return m_newOrders;
}

OrderList<BinanceNewOrder> BinanceOrderManager::GetOrdersByStatus(const BinanceNewOrderStatus orderStatus)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    OrderList<BinanceNewOrder> filteredOrders;
    for (const auto& order : m_newOrders)
    {
        if (order.second->GetOrderStatus() == orderStatus)
        {
            filteredOrders.try_emplace(order.first, std::make_unique<BinanceNewOrder>(*order.second));
        }
    }
    return filteredOrders;
}

const OrderList<BinanceCancelOrder>& BinanceOrderManager::GetCancelOrders() const {
    return m_cancelOrders;
}

const OrderList<BinanceReplaceOrder>& BinanceOrderManager::GetReplaceOrders() const {
    return m_replaceOrders;
}

const OrderList<BinanceQueryOrder>& BinanceOrderManager::GetQueryOrders() const
{
	return m_queryOrders;
}

std::vector<BinanceNewOrder*> BinanceOrderManager::GetOrdersOfSymbol(const std::string& symbol)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::vector<BinanceNewOrder*> ordersOfSymbol;
	for (const auto& order : m_newOrders)
	{
		if (order.second->GetSymbol() == symbol)
		{
			ordersOfSymbol.emplace_back(order.second.get());
		}
	}
	return ordersOfSymbol;
}

binapi::double_type BinanceOrderManager::GetWeightedAveragePrice(
    const std::string& symbol,
    const binapi::e_side side)
{
    std::lock_guard<std::mutex> lock(m_mutex);
	binapi::double_type totalPrice = 0.0;
	binapi::double_type totalAmount = 0.0;
	for (const auto& order : m_newOrders)
	{
		if (order.second->GetSymbol() == symbol && order.second->GetSide() == side)
		{
			const auto filledPrice = order.second->GetFilledPrice();
			const auto filledAmount = order.second->GetFilledAmount();
			totalPrice += filledPrice * filledAmount;
			totalAmount += filledAmount;
		}
	}
	if (totalAmount > 0)
	{
		return totalPrice / totalAmount;
	}
	return INVALID_PRICE;
}

BinanceNewOrder* BinanceOrderManager::UpdateNewOrderExecutionStatus(
    const std::string& clientOrderId, 
    const std::string& symbol, 
    const double filledAmount, 
    const double filledPrice, 
    const double remainingAmount,
    const std::size_t updateTime,
    const BinanceNewOrderStatus orderStatus,
    const std::string& exchangeText)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_newOrders.find(clientOrderId);
    if (it != m_newOrders.end()) 
    {
        auto& order = it->second;
        if (order->GetSymbol() == symbol)
        {
            order->SetFilledAmount(filledAmount);
            order->SetFilledPrice(filledPrice);
            order->SetRemainingAmount(remainingAmount);
            order->SetUpdateTime(updateTime);
            order->SetOrderStatus(orderStatus);
			order->SetExchangeText(exchangeText);
            LOG_INFO_STREAM(m_logger, "Order with clientOrderId '" << clientOrderId <<
                ", symbol '" << symbol
                << "' updated successfully.");
        } 
        else 
        {
            LOG_WARNING_STREAM(m_logger, "Symbol mismatch for order with clientOrderId '" 
                << ", symbol '" << symbol << clientOrderId << "'. Update skipped.");
        }
        return it->second.get();
    } 
    else 
    {
        LOG_WARNING_STREAM(m_logger, "No new order found with clientOrderId '" 
            << clientOrderId << "'. Update failed.");
    }
	return nullptr;
}

BinanceCancelOrder* OrderManagement::BinanceOrderManager::UpdateOrderCancellingStatus(
    const std::string& clientOrderId, 
    const std::string& symbol,
    const std::size_t updateTime,
    const BinanceCancelOrderStatus orderStatus,
    const std::string& exchangeText)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_cancelOrders.find(clientOrderId);
    if (it != m_cancelOrders.end()) 
    {
        auto& order = it->second;
        if (order->GetSymbol() == symbol) 
        {
            order->SetUpdateTime(updateTime);
            order->SetOrderStatus(orderStatus);
			order->SetExchangeText(exchangeText);
            LOG_INFO_STREAM(m_logger, "Cancel order with clientOrderId '" << clientOrderId
                << "', symbol '" << symbol
                << "' updated successfully.");
        } 
        else 
        {
            LOG_WARNING_STREAM(m_logger, "Symbol mismatch for cancel order with clientOrderId '"
                << clientOrderId << "', symbol '" << symbol << "'. Update skipped.");
        }
        return it->second.get();
    } 
    else 
    {
        LOG_WARNING_STREAM(m_logger, "No cancel order found with clientOrderId '"
            << clientOrderId << "'. Update failed.");
    }
	return nullptr;
}
