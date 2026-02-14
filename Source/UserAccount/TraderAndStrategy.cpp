/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "Trader.h"

#include "../LibraryUtils/StringUtils.h"

UserAccount::TraderType UserAccount::TraderAndStrategy::FromStringToTraderType(const std::string& traderTypeStr)
{
	if (traderTypeStr == "SpotTrader")
	{
		return TraderType::SPOT_TRADER;
	}
	else if (traderTypeStr == "FutureTrader")
	{
		return TraderType::FUTURE_TRADER;
	}
	else if (traderTypeStr == "HybridTrader")
	{
		return TraderType::HYBRID_TRADER;
	}
	else
	{
		throw std::runtime_error("Trader::FromStringToTraderType: unsupported trader type string=" + traderTypeStr);
	}
}

std::string UserAccount::TraderAndStrategy::FromTraderTypeToString(const TraderType traderType)
{
	switch (traderType)
	{
	case TraderType::SPOT_TRADER:
		return "SpotTrader";
	case TraderType::FUTURE_TRADER:
		return "FutureTrader";
	case TraderType::HYBRID_TRADER:
		return "HybridTrader";
	default:
		throw std::runtime_error("Trader::FromTraderTypeToString: unsupported trader type enum value=" + std::to_string(static_cast<unsigned>(traderType)));
	}
}

void UserAccount::TraderAndStrategy::InitTraderAndStrategy(const tinyxml2::XMLElement* traderAndStrategyXml)
{
	if (traderAndStrategyXml)
	{
		for (const auto* child = traderAndStrategyXml->FirstChildElement();
			child; child = child->NextSiblingElement())
		{
			const auto strategyName = child->Name();
			const auto trader = child->Attribute("Trader");
			if (strategyName && trader)
			{
				m_traderAndStrategyMap[strategyName] = TraderAndStrategy::FromStringToTraderType(trader);
			}
			else
			{
				throw std::runtime_error("TraderAndStrategy: invalid TraderAndStrategyXml config.");
			}
		}
	}
	else
	{
		throw std::runtime_error("TraderAndStrategy: TraderAndStrategyXml config does not exist.");
	}
}

bool UserAccount::TraderAndStrategy::IsTraderAssociatedWithStrategy(const std::string& strategyName, const TraderType traderType) const
{
	auto it = m_traderAndStrategyMap.find(strategyName);
	if (it != m_traderAndStrategyMap.end())
	{
		return it->second == traderType;
	}
	else
	{
		return false;
	}
}

UserAccount::TraderType UserAccount::TraderAndStrategy::GetTrader(const std::string& strategyName) const
{
	auto it = m_traderAndStrategyMap.find(strategyName);
	if (it != m_traderAndStrategyMap.end())
	{
		return it->second;
	}
	else
	{
		throw std::runtime_error("TraderAndStrategy: could not find trader for strategy=" + strategyName);
	}
}

std::string UserAccount::TraderAndStrategy::GetTraderAsString(const std::string& strategyName) const
{
	auto it = m_traderAndStrategyMap.find(strategyName);
	if (it != m_traderAndStrategyMap.end())
	{
		return FromTraderTypeToString(it->second);
	}
	else
	{
		throw std::runtime_error("TraderAndStrategy: could not find trader for strategy=" + strategyName);
	}
}


