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
#include <unordered_map>

#include "../SettingNConfig/tinyxml2.h"

namespace UserAccount
{
	enum class TraderType : unsigned
	{
		UNDEFINED_TRADER = 999,
		SPOT_TRADER = 0,
		FUTURE_TRADER = 1,
		HYBRID_TRADER = 2,
	};

	class DLL_CLASS_USERACCOUNT_EXPORTS TraderAndStrategy final
	{
	public:
		TraderAndStrategy() = default;

		static TraderType FromStringToTraderType(
			const std::string& traderTypeStr);

		static std::string FromTraderTypeToString(
			const TraderType traderType);

		void InitTraderAndStrategy(const tinyxml2::XMLElement* traderAndStrategyXml);

		bool IsTraderAssociatedWithStrategy(const std::string& strategyName, const TraderType traderType) const;

		TraderType GetTrader(const std::string& strategyName) const;

		std::string GetTraderAsString(const std::string& strategyName) const;
	private:
		std::unordered_map<std::string, TraderType> m_traderAndStrategyMap;
	};
} // namespace UserAccount