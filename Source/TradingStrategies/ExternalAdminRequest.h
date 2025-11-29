/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include <string>

namespace TradingStrategies {
	// This class represents an external administrative request
	// to control trading strategies, such as starting, stopping,
	// pausing, or resuming a strategy.
	class ExternalAdminRequest {
	public:
		enum class RequestType {
			START_STRATEGY,
			STOP_STRATEGY,
			PAUSE_STRATEGY,
			RESUME_STRATEGY,
			UNKNOWN
		};

		ExternalAdminRequest(RequestType type, const std::string& strategyName)
			: m_type(type), m_strategyName(strategyName) {
		}

		RequestType GetType() const { return m_type; }

		const std::string& GetStrategyName() const { return m_strategyName; }

	private:
		RequestType m_type{ RequestType::UNKNOWN };
		std::string m_strategyName;
	};
};