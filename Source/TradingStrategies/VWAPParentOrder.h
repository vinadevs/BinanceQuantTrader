/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "../OrderManagement/ExternalParentOrder.h"
#include "../TradingStrategies/VWAPVolumeProfile.h"

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

namespace TradingStrategies {

	using ParentOrderId = std::string;

	// Components required for VWAP calculation and tracking
	struct VWAPComponents final
	{
		std::unique_ptr<VWAPVolumeProfile> m_vwapVolumeProfilier; // VWAP volume profile calculator
		std::unordered_map<std::size_t, double> m_executedVolume;
		std::vector<double> m_executedPrices;
		std::vector<double> m_vwapPrices;
		std::vector<double> m_slippageSeries;
		std::vector<double> m_pnlSeries;
		double m_targetVWAPAmount{ 0.0 };
		double m_cumPriceVolume{ 0.0 };
		double m_totalMarketVolume{ 0.0 };
	};

	// VWAPParentOrder class to manage parent order and VWAP components
	// This class encapsulates the relationship between a parent order and its associated VWAP calculation components.
	// It provides methods to access and modify the parent order and VWAP components.

	class VWAPParentOrder final
	{
	public:
		VWAPParentOrder() {}

		OrderManagement::NewExternalParentOrder* GetParentOrder() const 
		{
			return m_parentOrder;
		}

		void SetParentOrder(OrderManagement::NewExternalParentOrder* parentOrder) 
		{
			m_parentOrder = parentOrder;
		}

		VWAPComponents& GetVWAPComponents() 
		{
			return m_vwapComponents;
		}
	private:
		OrderManagement::NewExternalParentOrder* m_parentOrder{ nullptr };
		VWAPComponents m_vwapComponents;
	};
} // namespace TradingStrategies
