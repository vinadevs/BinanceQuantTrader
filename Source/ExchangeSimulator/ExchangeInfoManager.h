/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "../KernelTrading/types.h"

#include <memory>
#include <string>

namespace tinyxml2 {
	class XMLElement;
};

namespace ExchangeSimulator {
	
	// The ExchangeInfoManager class is responsible for managing exchange information.
	// It initializes with a configuration element and provides access to exchange information.
	class ExchangeInfoManager final {
	public:
		ExchangeInfoManager(const tinyxml2::XMLElement* exchangeInfoManagerCfg);
		~ExchangeInfoManager();
		const binapi::rest::exchange_info_t& GetExchangeInfo() const { return m_exchangeInfo; }
	private:
		binapi::rest::exchange_info_t m_exchangeInfo;
	};
} // ns ExchangeSimulator