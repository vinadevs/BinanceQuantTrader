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
#include "../UserAccount/BinanceTrader.h"

using namespace TradingStrategies;
using namespace UserAccount;
using namespace MiddlewareMQ;
using namespace MarketData;
using namespace LibraryUtils;
using namespace tinyxml2;

TradingStrategyBase::TradingStrategyBase(
	const std::string& strategyName,
	const std::string& strategyDescription,
	const std::string& strategyCfgPath,
	MarketData::RealTimeMarketData* marketData,
	UserAccount::BinanceTrader* trader,
	ComplianceNRegulatory::BinanceTradingRules* tradingRules)
	: OrdersPerTenSecondsChecker(AlarmSystem::AlarmMode::REPEAT),
	m_strategyName(strategyName),
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
	// This is Binance compliances and rules checker
	OrdersPerTenSecondsChecker::Start();
	m_logger->Info("OrdersPerTenSecondsChecker started.");
}

TradingStrategyBase::~TradingStrategyBase() {}

StrategyRunStatus TradingStrategyBase::GetStrategyRunStatus() const
{
	return m_strategyRunStatus;
}

StrategyType TradingStrategyBase::GetStrategyType() const
{
	return m_strategyType;
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
}

bool TradingStrategyBase::IsNotIsNotExceededTradingRules() const
{
	if (m_tradingRules->IsNotExceededOrdersPerTenSeconds())
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
	return false;
}

void TradingStrategyBase::IncreaseOrderCounter() 
{
	m_tradingRules->IncreaseOrdersPerTenSeconds();
	if (m_StrategyLifeTime != StrategyLifeTime::INTRA_DAY)
	{
		m_tradingRules->IncreaseOrdersPerTwentyFourHours();
	}
}

void TradingStrategyBase::OnAlarmTriggered(const int passToDerived)
{
	// NOTE: PLEASE DO NOT CALL REST API UPDATES MANY TIMES/SECONDS
	// AS BINANCE WILL BAN THE LOCAL IP FOR THAT SPAM
	// PLEASE CHECK IN ComplianceNRegulatory CODE
	// REMEMBER ALWAY IMPLEMENT A CHECKER BEFORE CALL REST API
	m_tradingRules->ResetOrdersPerTenSecondsCounter();
}

#if USE_TEST_TRADING  
void TradingStrategyBase::OnHandlingReceivedSimulatorMessage(const BqtJsonMessage& message)
{
	// forward order acks to trader, they know what to do with it not strategy
	m_trader->HandleDownstreamAckMessage(message);
}
#endif