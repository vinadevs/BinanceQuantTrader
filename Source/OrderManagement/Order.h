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
#include "OrderDefinitions.h"

#include <string>
#include <stdexcept>

namespace OrderManagement {

	// -In trading, an "order" is an instruction given by a trader to a broker
	// or trading platform to buy or sell an asset
	// -This is base class for all Order option types (NEW/CANCEL/REPLACE/QUERRY/TEST)
	class DLL_CLASS_ORDERMANAGEMENT_EXPORTS Order
	{
	public:
		Order() = default;

		Order(const std::string& symbol, const std::string& clientOrderId, const MessageType binanceMessageType)
			: m_symbol(symbol), m_clientOrderId(clientOrderId), m_binanceMessageType(binanceMessageType)
		{
			UpdateOrderTypeStr();
			m_userAccountID = ApiKeyInfoMgr->GetApiKeyInfo().m_userID;
			m_updateTime = TimeUtils::GetEpochTimeTickNow(); // Initialize update time to current time
		}

		virtual ~Order() {}

		void SetSymbol(const std::string& symbol) {
			m_symbol = symbol;
		}

		const std::string& GetSymbol() const {
			return m_symbol;
		}

		MessageType GetOrderMessageType() const {
			return m_binanceMessageType;
		}

		const std::string& GetOrderMessageTypeStr() const {
			return m_binanceMessageTypeStr;
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

		void SetExchangeText(const std::string& exchangeText) {
			m_exchangeText = exchangeText;
		}

		const std::string& GetExchangeText() const {
			return m_exchangeText;
		}
	protected:
		void UpdateOrderTypeStr() {
			switch (m_binanceMessageType)
			{
			case OrderManagement::MessageType::UNDEF: {
				m_binanceMessageTypeStr = "UNDEF";
				break;
			}
			case OrderManagement::MessageType::NEW: {
				m_binanceMessageTypeStr = "NEW";
				break;
			}
			case OrderManagement::MessageType::CANCEL: {
				m_binanceMessageTypeStr = "CANCEL";
				break;
			}
			case OrderManagement::MessageType::REPLACE: {
				m_binanceMessageTypeStr = "REPLACE";
				break;
			}
			case OrderManagement::MessageType::QUERY: {
				m_binanceMessageTypeStr = "QUERY";
				break;
			}
			case OrderManagement::MessageType::TEST: {
				m_binanceMessageTypeStr = "TEST";
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
		std::string m_binanceMessageTypeStr; // String representation of the message type
		std::string m_exchangeText; // Text message from the exchange, e.g., "Order accepted", "Order rejected", etc.
		MessageType m_binanceMessageType{ MessageType::UNDEF };
		std::size_t m_updateTime{ 0 }; // Time order changed status
		// Execution Result
		MiddlewareMQ::MiddlewareMQResult m_sendingSimulatorOrderResult;
	};
};