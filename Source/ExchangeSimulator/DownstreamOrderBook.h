/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "../LibraryUtils/BQTDeque.h"

#include <map>
#include <memory>
#include <string>
#include <mutex>

namespace LibraryUtils {
	class Logger;
};

namespace ExchangeSimulator {

	// Participant order
	struct DownstreamOrder final
	{
		DownstreamOrder(const double price, const double quantity)
			: m_price(price), m_quantity(quantity) {}
		double m_price{ 0 };
		double m_quantity{ 0 };
	};

	// Exchange order book to store participant orders
	class OrderBook final : public MultipleThreads::BQTDeque<DownstreamOrder>
	{
	public:
		friend std::ostream& operator<<(std::ostream& os, const OrderBook& book)
		{
			// Print book table header
			std::cout << std::setw(10) << "Price"
				<< std::setw(15) << "Quantity"
				<< '\n';
			std::cout << std::string(25, '-') << '\n';
			// Print each price level in the book
			for (const auto& [price, quantity] : book.GetAllItems())
			{
				std::cout << std::setw(10) << price
					<< std::setw(15) << quantity
					<< '\n';
			}
		}
	};

	using OrderBooks = std::map<std::string, std::unique_ptr<OrderBook>>;

	/**
	 * @class DownstreamOrderBook
	 * @brief Represents an order book used for downstream (exchange) processing of trading orders.
	 *
	 * The DownstreamOrderBook class handles the storage and management of trading orders,
	 * including bid and ask orders, for a specific trading pair. It provides methods to
	 * insert, update, and remove orders as well as to retrieve the best bid/ask prices.
	 *
	 */

	class DownstreamOrderBook final
	{
	public:
		DownstreamOrderBook();
		~DownstreamOrderBook();

		void CreateNewOrderBook(const std::string& symbol);

		void RemoveOrderBook(const std::string& symbol);

		OrderBook* LookupOrderBook(const std::string& symbol);

		void ClearAll();

		const OrderBooks& GetOrderBooks();
	private:
		std::unique_ptr<LibraryUtils::Logger> m_logger;
		std::mutex m_mutex;
		OrderBooks m_orderBooks;
	};
};