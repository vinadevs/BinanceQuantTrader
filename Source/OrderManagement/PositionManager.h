/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include "../KernelTrading/double_type.h"

#include <string>
#include <memory>
#include <unordered_map>

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

	class DLL_CLASS_ORDERMANAGEMENT_EXPORTS PositionManager final
	{
	public:
		PositionManager();
		~PositionManager();

        enum class PositionSide : unsigned
        {
            LONG,
            SHORT,
        };

        ////////////////////// postion ////////////////////////////////////////

        // Add new long postion
        std::unique_ptr<BinanceNewOrder> OpenLongPositionUpstreamOrder(
			const std::string& symbol,
			const binapi::double_type quality,
			const binapi::double_type refPrice);

        // Add new short postion
        std::unique_ptr<BinanceNewOrder> OpenShortPositionUpstreamOrder(
			const std::string& symbol,
			const binapi::double_type quality,
			const binapi::double_type refPrice);
	private:
        std::unordered_map <std::string, PositionSide> m_workedPositions;
        std::unique_ptr<LibraryUtils::Logger> m_logger;
		std::unique_ptr<OrderCreator> m_orderCreator;
	};
};