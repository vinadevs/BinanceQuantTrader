/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "../LibraryUtils/SourceBuildFlags.h"

#if USE_TEST_TRADING
#include "../RestAPI/ApiKeyInfoManager.h"
#endif

#include "dlldefine.h"

#include <string>
#include <stdexcept>

namespace OrderManagement {
	// Type of order message which will send to the exchange/downstream/simulator
	enum class BinanceOrderType : unsigned
	{
		UNDEF,
		NEW,
		CANCEL,
		REPLACE,
		QUERY,
		TEST,
	};

	// -In trading, an "order" is an instruction given by a trader to a broker
	// or trading platform to buy or sell an asset
	// -This is base class for all Order option types (NEW/CANCEL/REPLACE)
	class DLL_CLASS_ORDERMANAGEMENT_EXPORTS Order
	{
	public:
		Order() = default;

		Order(const std::string& symbol, const BinanceOrderType binanceOrderType)
			: m_symbol(symbol), m_binanceOrderType(binanceOrderType)
		{
			UpdateOrderTypeStr();
#if USE_TEST_TRADING
			m_userAccountID = ApiKeyInfoMgr->GetApiKeyInfo().m_userID;
#endif
		}

		virtual ~Order() {}

		void SetSymbol(const std::string& symbol) { 
			m_symbol = symbol; 
		}

		const std::string& GetSymbol() const {
			return m_symbol;
		}

		BinanceOrderType GetOrderType() const {
			return m_binanceOrderType;
		}

		std::string GetOrderTypeStr() const {
			return m_binanceOrderTypeStr;
		}

#if USE_TEST_TRADING
		void SetUserAccountID(const std::string& userID) {
			m_userAccountID = userID;
		}

		const std::string& GetUserAccountID() const {
			return m_userAccountID;
		}
#endif
	protected:
		void UpdateOrderTypeStr() {
			switch (m_binanceOrderType)
			{
			case OrderManagement::BinanceOrderType::UNDEF: {
				m_binanceOrderTypeStr = "UNDEF";
				break;
			}
			case OrderManagement::BinanceOrderType::NEW: {
				m_binanceOrderTypeStr = "NEW";
				break;
			}
			case OrderManagement::BinanceOrderType::CANCEL: {
				m_binanceOrderTypeStr = "CANCEL";
				break;
			}
			case OrderManagement::BinanceOrderType::REPLACE: {
				m_binanceOrderTypeStr = "REPLACE";
				break;
			}
			case OrderManagement::BinanceOrderType::QUERY: {
				m_binanceOrderTypeStr = "QUERY";
				break;
			}
			case OrderManagement::BinanceOrderType::TEST: {
				m_binanceOrderTypeStr = "TEST";
				break;
			}
			default:
				throw std::runtime_error("BinanceOrder: undetected order type.");
			}
		}
#if USE_TEST_TRADING
		std::string m_userAccountID;
#endif
		std::string m_symbol;
		std::string m_binanceOrderTypeStr;
		BinanceOrderType m_binanceOrderType{ BinanceOrderType::UNDEF };
	};
};