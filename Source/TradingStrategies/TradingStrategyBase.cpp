/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "TradingStrategyBase.h"

#include "../MarketData/RealTimeMarketData.h"
#include "../LibraryUtils/GeneralUtils.h"
#include "../LibraryUtils/StringUtils.h"
#include "../SettingNConfig/tinyxml2.h"
#include "../ComplianceNRegulatory/BinanceTradingRules.h"
#include "../ComplianceNRegulatory/HardTradingLimits.h"
#include "../UserAccount/BinanceTrader.h"
#include "../UserAccount/FutureTrader.h"
#include "../UserAccount/HybridTrader.h"

using namespace TradingStrategies;
using namespace UserAccount;
using namespace MarketData;
using namespace LibraryUtils;
using namespace tinyxml2;

#if USE_BACK_TEST_TRADING 
using namespace MiddlewareMQ;
#endif

TradingStrategyBase::TradingStrategyBase(
	const std::string& strategyName,
	const std::string& strategyDescription,
	const std::string& strategyCfgPath,
	MarketData::RealTimeMarketData* marketData,
	UserAccount::Trader* trader,
	ComplianceNRegulatory::BinanceTradingRules* tradingRules)
	: m_strategyName(strategyName),
	m_strategyDescription(strategyDescription),
	m_strategyCfgPath(strategyCfgPath),
	m_marketData(marketData),
	m_trader(trader),
	m_tradingRules(tradingRules),
	m_strategyID(GeneralUtils::GenerateUniqueID(m_strategyName))
{
	m_logger = std::make_unique<Logger>(m_strategyName);
	m_logger->Info("Trading strategy name=" + m_strategyName);
	m_logger->Info("Trading strategy description=" + m_strategyDescription);
	if (m_strategyType != StrategyType::ADVISING) // advising strategy does not need trader or parent order manager
	{
		InitQuantStrategist();
		InitParentOrderManager();
	}
}

TradingStrategyBase::TradingStrategyBase(
	const std::string& strategyName,
	const std::string& strategyDescription,
	const std::string& strategyCfgPath,
	MarketData::RealTimeMarketData* marketData)
	: m_strategyName(strategyName),
	m_strategyDescription(strategyDescription),
	m_strategyCfgPath(strategyCfgPath),
	m_marketData(marketData)
{
	m_logger = std::make_unique<Logger>(m_strategyName);
	m_logger->Info("Trading strategy name=" + m_strategyName);
	m_logger->Info("Trading strategy description=" + m_strategyDescription);
}

TradingStrategyBase::~TradingStrategyBase() {}

void TradingStrategyBase::LogTradingHardLimits()
{
    m_logger->Info("[Binance Hard Limitation] Orders per ten seconds limit=" 
		+ std::to_string(m_tradingRules->GetTradingLimits()->m_maxOrdersPerTenSeconds));
    m_logger->Info("[Binance Hard Limitation] Request weight per minute limit=" 
		+ std::to_string(m_tradingRules->GetTradingLimits()->m_maxRequestWeightPerMinute));
	m_logger->Info("[Binance Hard Limitation] Orders per twenty-four hours limit=" 
		+ std::to_string(m_tradingRules->GetTradingLimits()->m_maxOrdersPerTwentyFourHours));
}

void TradingStrategyBase::InitQuantStrategist()
{
	if (m_trader)
	{
		if (m_spotTrader = dynamic_cast<UserAccount::BinanceTrader*>(m_trader);
			m_spotTrader->GetTraderAndStrategyMapping().IsTraderAssociatedWithStrategy(m_strategyName, m_spotTrader->GetTraderType()))
		{
			m_logger->Info("SpotTrader is set up successfully.");
		}
		else if (m_futureTrader = dynamic_cast<UserAccount::FutureTrader*>(m_trader);
			m_futureTrader->GetTraderAndStrategyMapping().IsTraderAssociatedWithStrategy(m_strategyName, m_futureTrader->GetTraderType()))
		{
			m_logger->Info("FutureTrader is set up successfully.");
		}
		else if (m_hybridTrader = dynamic_cast<UserAccount::HybridTrader*>(m_trader);
			m_hybridTrader->GetTraderAndStrategyMapping().IsTraderAssociatedWithStrategy(m_strategyName, m_hybridTrader->GetTraderType()))
		{
			m_logger->Info("HybridTrader is set up successfully.");
		}
		else
		{
			const auto trader = m_spotTrader->GetTraderAndStrategyMapping().GetTraderAsString(m_strategyName);
			throw std::runtime_error("TradingStrategyBase: Trader must be " + trader + " for strategy=" + m_strategyName);
		}
		// register this strategy with trader to trade
		m_logger->Info("Quant trader will use strategy=" + m_strategyName);
		m_trader->UseThisStrategyToTrade(this);
	}
	else
	{
		throw std::runtime_error("TradingStrategyBase: Trader is null.");
	}
}

void TradingStrategyBase::InitParentOrderManager()
{
	m_parentOrderManager = std::make_unique<OrderManagement::ParentOrderManager>();
	ExternalRequestReceiver::SetParentOrderManager(m_parentOrderManager.get());
	ExternalRequestReceiver::SetUpstreamReceiver(m_strategyName);
	m_logger->Info("Parent order manager is initialized.");
}

StrategyRunStatus TradingStrategyBase::GetStrategyRunStatus() const
{
	return m_strategyRunStatus.load(std::memory_order_acquire);
}

StrategyType TradingStrategyBase::GetStrategyType() const
{
	return m_strategyType;
}

StrategyOrderScheduler TradingStrategyBase::GetStrategyOrderScheduler() const
{
	return m_strategyOrderScheduler;
}

void TradingStrategyBase::SetStrategyType(const StrategyType strategyType)
{
	m_strategyType = strategyType;
	m_logger->Info("Strategy type=" + GetStrategyTypeStr(m_strategyType));
}

std::string TradingStrategyBase::GetStrategyTypeStr(const StrategyType strategyType)
{
	switch (strategyType)
	{
	case StrategyType::FULL_AUTO:
		return "FULL_AUTO";
	case StrategyType::SEMI_AUTO:
		return "SEMI_AUTO";
	case StrategyType::ADVISING:
		return "ADVISING";
	default:
		return "UnknownStrategyType";
	}
}

const std::string& TradingStrategyBase::GetStrategyName() const
{
	return m_strategyName;
}

const std::string& TradingStrategyBase::GetStrategyID() const
{
	return m_strategyID;
}

void TradingStrategyBase::SetupStrategyLifeTime(tinyxml2::XMLDocument* strategyCfgPathXml)
{
	const XMLElement* generalConfigXml = strategyCfgPathXml->FirstChildElement("GeneralConfig");
	assert(generalConfigXml);
	const XMLElement* lifeTimeXml = generalConfigXml->FirstChildElement("LifeTime");
	assert(lifeTimeXml);
	if (StringUtils::IsConfigAttributeMatched(lifeTimeXml->Attribute("LifeTimeForStrategy"), "IntraDay"))
	{
		m_StrategyLifeTime = StrategyLifeTime::INTRA_DAY;
	}
	else if (StringUtils::IsConfigAttributeMatched(lifeTimeXml->Attribute("LifeTimeForStrategy"), "IntraWeek"))
	{
		m_StrategyLifeTime = StrategyLifeTime::INTRA_WEEK;
	}
	else if (StringUtils::IsConfigAttributeMatched(lifeTimeXml->Attribute("LifeTimeForStrategy"), "IntraMonth"))
	{
		m_StrategyLifeTime = StrategyLifeTime::INTRA_MONTH;
	}
	else
	{
		throw std::runtime_error("TradingStrategyBase: unsupported StrategyLifeTime config");
	}
	const XMLElement* enableComplianceCheckerXml = generalConfigXml->FirstChildElement("EnableComplianceChecker");
	assert(enableComplianceCheckerXml);
	if (enableComplianceCheckerXml->BoolAttribute("Enable")) // should always enable compliance checker for real trading
	{
		LogTradingHardLimits();
		m_compilanceChecker = std::make_unique<CompilanceChecker>();
		m_compilanceChecker->StartAlarmOnTradingRules(
			m_tradingRules,
			AlarmSystem::AlarmMode::REPEAT,
			m_StrategyLifeTime != StrategyLifeTime::INTRA_DAY ? true : false);
	}
}

bool TradingStrategyBase::IsNotIsNotExceededTradingRules() const
{
	if (m_tradingRules->IsNotExceededOrdersPerTenSeconds())
	{
		if (m_tradingRules->IsNotExceededRequestWeightPerMinute())
		{
			if (m_StrategyLifeTime == StrategyLifeTime::INTRA_DAY)
			{
				return true;
			}
			else if (m_tradingRules->IsNotExceededOrdersPerTwentyFourHours())
			{
				return true;
			}
		}
	}
	return false;
}

void TradingStrategyBase::IncreaseComplianceRestAPIRequestCounter(const size_t noOfRequests)
{
	m_tradingRules->IncreaseOrdersPerTenSeconds(noOfRequests);
	m_logger->Info("CompilanceChecker: current number of orders per ten seconds="
		+ std::to_string(m_tradingRules->GetOrdersPerTenSecondsCounter()) + ".");
	m_tradingRules->IncreaseRequestWeightPerMinute(noOfRequests);
	m_logger->Info("CompilanceChecker: current number of requests per minute="
		+ std::to_string(m_tradingRules->GetRequestWeightPerMinuteCounter()) + ".");
	if (m_StrategyLifeTime != StrategyLifeTime::INTRA_DAY)
	{
		m_tradingRules->IncreaseOrdersPerTwentyFourHours(noOfRequests);
	}
}

bool TradingStrategyBase::IsStrategyWellInitiated() const
{
	return m_isStrategyWellInitiated;
}

const OrderManagement::ParentOrderManager* TradingStrategyBase::GetParentOrderManager() const
{
	return m_parentOrderManager.get();
}

#if USE_BACK_TEST_TRADING  
void TradingStrategyBase::OnHandlingReceivedSimulatorMessage(const BqtJsonMessage& message)
{
	// forward order acks to trader, they know what to do with it not strategy
	m_trader->HandleDownstreamAckMessage(message);
}
#endif