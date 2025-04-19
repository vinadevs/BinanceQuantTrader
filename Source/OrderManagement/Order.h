/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "../KernelTrading/double_type.h"
#include "../LibraryUtils/SourceBuildFlags.h"
#include "../RestAPI/ApiKeyInfoManager.h"
#include "../RestAPI/RestAPI.h"
#include "../MiddlewareMQ/MessageDelivery.h"
#include "../MiddlewareMQ/BqtJsonMessage.h"
#include "../LibraryUtils/TimeUtils.h"

#include "dlldefine.h"

#include <string>
#include <stdexcept>

namespace OrderManagement {
	// Type of order message which will send to the exchange/downstream/simulator
	enum class MessageType : unsigned
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
	// -This is base class for all Order option types (NEW/CANCEL/REPLACE/QUERRY/TEST)
	class DLL_CLASS_ORDERMANAGEMENT_EXPORTS Order
	{
	public:
		Order() = default;

		Order(const std::string& symbol, const std::string& clientOrderId, const MessageType binanceOrderType)
			: m_symbol(symbol), m_clientOrderId(clientOrderId), m_binanceOrderType(binanceOrderType)
		{
			UpdateOrderTypeStr();
			m_userAccountID = ApiKeyInfoMgr->GetApiKeyInfo().m_userID;
		}

		virtual ~Order() {}

		void SetSymbol(const std::string& symbol) { 
			m_symbol = symbol; 
		}

		const std::string& GetSymbol() const {
			return m_symbol;
		}

		MessageType GetOrderType() const {
			return m_binanceOrderType;
		}

		const std::string& GetOrderTypeStr() const {
			return m_binanceOrderTypeStr;
		}

		const std::string& GetClientOrderId() const { return m_clientOrderId; }

		void SetClientOrderId(const std::string& clientOrderId) { m_clientOrderId = clientOrderId; }

		std::size_t GetOrderId() const { return m_orderId; }

		void SetOrderId(const std::size_t orderId) { m_orderId = orderId; }

		std::string GetOrderIdString() const { return std::to_string(m_orderId); }

		void SetUserAccountID(const std::string& userID) {
			m_userAccountID = userID;
		}

		const std::string& GetUserAccountID() const {
			return m_userAccountID;
		}

		void SetSendingOrderResult(const MiddlewareMQ::MiddlewareMQResult& sendingOrderResult)
		{
			m_sendingSimulatorOrderResult = sendingOrderResult;
		}

		MiddlewareMQ::MiddlewareMQResult GetSendingSimulatorOrderResult() const {
			return m_sendingSimulatorOrderResult;
		}
	
		void SetUpdateTime(const std::size_t updateTime) {
			m_updateTime = updateTime;
		}

		std::string GetUpdateTimeStr() const
		{
			return TimeUtils::ConvertEpochTickToTimeString(m_updateTime);
		}

		std::size_t GetUpdateTime() const { return m_updateTime; }

	protected:
		void UpdateOrderTypeStr() {
			switch (m_binanceOrderType)
			{
			case OrderManagement::MessageType::UNDEF: {
				m_binanceOrderTypeStr = "UNDEF";
				break;
			}
			case OrderManagement::MessageType::NEW: {
				m_binanceOrderTypeStr = "NEW";
				break;
			}
			case OrderManagement::MessageType::CANCEL: {
				m_binanceOrderTypeStr = "CANCEL";
				break;
			}
			case OrderManagement::MessageType::REPLACE: {
				m_binanceOrderTypeStr = "REPLACE";
				break;
			}
			case OrderManagement::MessageType::QUERY: {
				m_binanceOrderTypeStr = "QUERY";
				break;
			}
			case OrderManagement::MessageType::TEST: {
				m_binanceOrderTypeStr = "TEST";
				break;
			}
			default:
				throw std::runtime_error("BinanceOrder: undetected order type.");
			}
		}
		std::string m_userAccountID;
		std::string m_symbol;
		std::size_t m_orderId{ 0 }; // m_orderId is order ID from Binance
		std::string m_clientOrderId; // m_clientOrderId is new order ID from us
		std::string m_binanceOrderTypeStr;
		MessageType m_binanceOrderType{ MessageType::UNDEF };
		std::size_t m_updateTime{ 0 }; // Time order changed status
		// Execution Result
		MiddlewareMQ::MiddlewareMQResult m_sendingSimulatorOrderResult;
	};
};