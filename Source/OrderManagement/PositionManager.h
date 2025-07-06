/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include "../QuantitativeModel/QuantOrderParammeter.h"

#include "BinanceOrderManager.h"

#include <string>
#include <memory>

namespace LibraryUtils {
    class Logger;
};

namespace OrderManagement {

	//- In trading, a position represents a trader's exposure to an asset
	// or financial instrument. It indicates whether a trader has 
	// bought (or is long on) or sold (or is short on) a certain 
	// quantity of an asset. Positions reflect the trader's stance
	// in the market and play a crucial role in determining potential
	// profits or losses as asset prices fluctuate.
	// -This class will manage all worked trading postions
	// -It can create new, cancel, replace, amend, an order... 

	class OrderCreator;
	class BinanceOrderManager;

	class DLL_CLASS_ORDERMANAGEMENT_EXPORTS PositionManager final
	{
	public:
		PositionManager();
		~PositionManager();

        ////////////////////// postion ////////////////////////////////////////

        // Create new trading postion
        std::unique_ptr<BinanceNewOrder> OpenNewPositionUpstreamOrder(
			const QuantitativeModel::QuantOrderParammeter& param);

		std::unique_ptr<BinanceNewOrder> OpenNewTestPositionUpstreamOrder(
			const QuantitativeModel::QuantOrderParammeter& param);

		// Create new future position upstream order
		std::unique_ptr<BinanceNewOrder> OpenNewFuturePositionUpstreamOrder(
			const QuantitativeModel::QuantOrderParammeter& param);

		std::unique_ptr<BinanceCancelOrder> CancelPositionUpstreamOrder(
			const BinanceNewOrder* originalOrder);

		// worked order (orders sent to exchange successfully)
		void AddNewWorkedOrder(const std::string& clientOrderId, std::unique_ptr<BinanceNewOrder> order);

		// unworked order (orders not sent to exchange)
		void AddUnworkedOrder(const std::string& clientOrderId, std::unique_ptr<BinanceNewOrder> order);

		// worked cancel order (cancel orders sent to exchange successfully)
		void AddNewCancelOrder(const std::string& clientOrderId, std::unique_ptr<BinanceCancelOrder> order);

		// unworked cancel order (cancel orders not sent to exchange)
		void AddUnworkedCancelOrder(const std::string& clientOrderId, std::unique_ptr<BinanceCancelOrder> order);

		bool CloseOpenedPositionUpstreamOrder(const std::string& clientOrderId);
		bool CloseAllOpenedPositionsBySide(const binapi::e_side posSide);
		bool CloseAllOpenedPositions();

		OrderManagement::BinanceOrderManager* GetWorkedOrderManager() const
		{
			return m_workedOrderManager.get();
		}

		OrderManagement::BinanceOrderManager* GetUnworkedOrderManager() const
		{
			return m_unworkedOrderManager.get();
		}

		binapi::double_type GetWeightedAveragePrice(
			const std::string& symbol,
			const binapi::e_side posSide) const
		{
			return m_workedOrderManager->GetWeightedAveragePrice(symbol, posSide);
		}

		// Execution of all new worked orders
		void UpdateNewOrderExecutionStatus(
			const std::string& clientOrderId,
			const std::string& symbol,
			const double filledAmount,
			const double filledPrice,
			const double remainingAmount,
			const std::size_t updateTime,
			const BinanceNewOrderStatus orderStatus);

		// Execution of all cancel orders
		void UpdateOrderCancellingStatus(
			const std::string& clientOrderId,
			const std::string& symbol,
			const std::size_t updateTime,
			const BinanceCancelOrderStatus orderStatus);

	private:
        std::unique_ptr<LibraryUtils::Logger> m_logger;
		std::unique_ptr<OrderCreator> m_orderCreator;
		// Order list before sending to exchange
		std::unique_ptr<OrderManagement::BinanceOrderManager> m_workedOrderManager;
		// Order list after sending failed to exchange
		std::unique_ptr<OrderManagement::BinanceOrderManager> m_unworkedOrderManager;
	};
};