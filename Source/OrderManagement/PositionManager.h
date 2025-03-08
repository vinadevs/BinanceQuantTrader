/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include <string>
#include <memory>
#include <unordered_map>
#include <mutex>

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

	enum class PositionSide : unsigned
	{
		LONG, // buy side
		SHORT, // sell side
	};

	enum class PositionType : unsigned
	{
		OPENING, // new positions
		FILLING, // position in middle of trade (filled, partial filled)
		REJECTED, // positions were rejected by downstream
	};

	class OrderCreator;
	class BinanceWorkedOrderManager;

	class DLL_CLASS_ORDERMANAGEMENT_EXPORTS PositionManager final
	{
	public:
		PositionManager(BinanceWorkedOrderManager* workedOrderManager);
		~PositionManager();

        ////////////////////// postion ////////////////////////////////////////

        // Create new trading postion
        std::unique_ptr<BinanceNewOrder> OpenNewPositionUpstreamOrder(
			const std::string& symbol,
			const PositionSide posSide,
			const double quality,
			const double refPrice);

		std::unique_ptr<BinanceNewOrder> OpenNewTestPositionUpstreamOrder(
			const std::string& symbol,
			const PositionSide posSide,
			const double quality,
			const double refPrice);

		bool CloseOpenedPositionUpstreamOrder(const std::string& clientOrderId);
		bool CloseAllOpenedPositionUpstreamOrder(const PositionSide posSide, const PositionType posType);
	private:
		bool CloseWorkedPosition(const std::string& clientOrderId);
		bool CloseWorkeOrder(const std::string& clientOrderId);

		std::mutex m_mutex;
        std::unordered_map <std::string, PositionSide> m_workedPositions;
        std::unique_ptr<LibraryUtils::Logger> m_logger;
		std::unique_ptr<OrderCreator> m_orderCreator;
		BinanceWorkedOrderManager* m_workedOrderManager {nullptr};
	};
};