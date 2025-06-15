/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "TraderFactory.h"

#include "../UserAccount/BinanceTrader.h"
#include "../PortfolioManager/PortfolioInvestmentBinance.h"
#include "../RiskManagement/RiskManager.h"
#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/StringUtils.h"

#include <assert.h>

using namespace BinanceQuantTrader;
using namespace PortfolioManager;
using namespace ComplianceNRegulatory;
using namespace RiskManagement;
using namespace UserSpotAccount;
using namespace tinyxml2;

std::unique_ptr<BinanceTrader> TraderFactory::CreateSmartTrader(
	PortfolioInvestmentBinance* portfolio, 
	BinanceTradingRules* tradingRules,
	RiskManager* riskManager,
	const XMLElement* traderXmlCfg)
{
	assert(traderXmlCfg);
	const auto* usingTraderXml = traderXmlCfg->FirstChildElement("UsingTrader");
	assert(usingTraderXml);
	if (StringUtils::IsConfigAttributeMatched(usingTraderXml->Attribute("Type"), "BinanceTrader"))
	{
		const auto* reportXml = traderXmlCfg->FirstChildElement("Report");
		assert(reportXml);
		return std::make_unique<UserSpotAccount::BinanceTrader>(reportXml, portfolio, tradingRules, riskManager);
	}
	else
	{
		throw std::runtime_error("TraderFactory: unsupported UsingTrader config");
	}
}
