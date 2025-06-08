/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include "../SettingNConfig/tinyxml2.h"

#include "ExchangeRuleAndCompliance.h"

#include <cassert>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using namespace ExchangeSimulator;
using namespace tinyxml2;

ExchangeRuleAndCompliance::~ExchangeRuleAndCompliance() {}

ExchangeRuleAndCompliance* ExchangeRuleAndCompliance::GetInstance()
{
    static ExchangeRuleAndCompliance instance;
    return &instance;
}

void ExchangeRuleAndCompliance::SetRuleAndCompliance(const tinyxml2::XMLElement* exchangeRuleConfigXml)
{
    assert(exchangeRuleConfigXml);
    const auto* rulesXml = exchangeRuleConfigXml->FirstChildElement("TradeCommissions");
    assert(rulesXml);
    m_makerCommission = rulesXml->DoubleAttribute("MakerCommission");
    m_takerCommission = rulesXml->DoubleAttribute("TakerCommission");
    m_buyerCommission = rulesXml->DoubleAttribute("BuyerCommission");
    m_sellerCommission = rulesXml->DoubleAttribute("SellerCommission");
    if (m_makerCommission <= 0 || m_takerCommission <= 0)
    {
        throw std::runtime_error("MakerCommission/TakerCommission are invalid.");
    }
	m_futureMakerCommission = rulesXml->DoubleAttribute("FutureMakerCommission");
	m_futureTakerCommission = rulesXml->DoubleAttribute("FutureTakerCommission");
	if (m_futureMakerCommission <= 0 || m_futureTakerCommission <= 0)
	{
		throw std::runtime_error("FutureMakerCommission/FutureTakerCommission are invalid.");
	}
	// Load margin rate info from JSON file
	const auto* marginRateInfoXml = exchangeRuleConfigXml->FirstChildElement("MarginRateInfo");
	if (marginRateInfoXml) {
		const char* filename = marginRateInfoXml->Attribute("FileName");
		if (filename) {
			LoadLeverageBracketsFromFile(filename);
		}
		else {
			std::cerr << "No filename specified for MarginRateInfo.\n";
		}
	}
}

RateCommission ExchangeRuleAndCompliance::GetMakerCommission() const
{
    return m_makerCommission;
}

RateCommission ExchangeRuleAndCompliance::GetTakerCommission() const
{
    return m_takerCommission;
}

RateCommission ExchangeRuleAndCompliance::GetBuyerCommission() const
{
    return m_buyerCommission;
}

RateCommission ExchangeRuleAndCompliance::GetSellerCommission() const
{
    return m_sellerCommission;
}

RateCommission ExchangeRuleAndCompliance::GetFutureMakerCommission() const
{
	return m_futureMakerCommission;
}

RateCommission ExchangeRuleAndCompliance::GetFutureTakerCommission() const
{
	return m_futureTakerCommission;
}

const CurlAPI::SymbolMarginRateInfo& ExchangeRuleAndCompliance::GetFutureMarginRateInfo(const std::string& symbol) const
{
	auto it = m_symbolMarginRateInfos.find(symbol);
	if (it != m_symbolMarginRateInfos.end()) {
		return it->second;
	}
	else {
		std::cerr << "No margin rate info found for symbol: " << symbol << "\n";
		return CurlAPI::SymbolMarginRateInfo(symbol);
	}
}

const CurlAPI::LeverageBracket& ExchangeRuleAndCompliance::GetFutureLeverageBracketByNotional(
    const std::string& symbol,
    const double postitionNotional)
{
	const auto& marginRateInfo = GetFutureMarginRateInfo(symbol);
	for (const auto& bracket : marginRateInfo.m_Brackets) {
		if (postitionNotional <= bracket.m_NotionalCap) {
			return bracket;
		}
	}
	throw std::runtime_error("No leverage bracket found for symbol: " + symbol + " with notional: " + std::to_string(postitionNotional));
}

void ExchangeRuleAndCompliance::LoadLeverageBracketsFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return;
    }
    nlohmann::json j;
    try {
        file >> j;

        for (const auto& item : j) {
            std::string symbol = item.at("symbol");
            CurlAPI::SymbolMarginRateInfo info(symbol);

            for (const auto& b : item.at("leverageBrackets")) {
                const int tier = b.at("tier");
                const double cap = b.at("notionalCap");
                const double mmr = b.at("maintenanceMarginRate");
                const double imr = b.at("initialMarginRate");
                const int leverage = b.at("maxLeverage");

                info.m_Brackets.emplace_back(tier, cap, mmr, imr, leverage);
            }
            m_symbolMarginRateInfos[symbol] = std::move(info);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << "\n";
    }
}
