/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

// -All Indicator/Signals will follow this base class
// -Indicator/Signals will only analyze trading data to give hints
// to Strategy so they should not do any trading operations...
// -Each indicator/Signal should only handle a type of data for ex: book ticker,
// trade, kline,...

#include "dlldefine.h"

#include <list>
#include <memory>

#include "../LibraryUtils/Logger.h"
#include "TradingHints.h"

namespace tinyxml2 {
	class XMLElement;
};

namespace IndicatorNSignals {

class DLL_CLASS_INDICATORNSIGNALS_EXPORTS IndicatorAndSignals
{
public:
	IndicatorAndSignals(const std::string& m_symbol);
	virtual ~IndicatorAndSignals();
	// These 2 functions must be called at the class side
	// to receive trading hints
	void RegisterTradingHintsListener(TradingHintsListener* listener);
	void UnregisterTradingHintsListener(TradingHintsListener* listener);
	// return int as we need to count number of succeeded updates
	int NotifyTradingHintsToListeners();
	const std::string& GetTargetSymbol() const;

	virtual void InitParameters(const tinyxml2::XMLElement* parametersXml) = 0;
	virtual void VerifyParameters() = 0;
protected:
	std::unique_ptr<LibraryUtils::Logger> m_logger;
	using ListTradingHintsListener = std::list<TradingHintsListener*>;
	ListTradingHintsListener m_tradingHintsListeners;
	std::unique_ptr<TradingHints> m_tradingHints;
};
};


