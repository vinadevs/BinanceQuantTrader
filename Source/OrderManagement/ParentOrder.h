/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "../RestAPI/ApiKeyInfoManager.h"
#include "../LibraryUtils/TimeUtils.h"

#include "dlldefine.h"

#include <string>

namespace OrderManagement {

	enum class ParentOrderSide : unsigned
	{
		BUY,
		SELL
	};

	enum class ChildOrderType : unsigned
	{
		MARKET,
		LIMIT,
		STOP_MARKET,
		STOP_LIMIT,
		TARGET_LIMIT,
		TRAILING_STOP_MARKET
	};

	enum class ParentOrderType : unsigned
	{
		NEW,
		CANCEL,
		AMEND
	};

	enum class ParentOrderStatus : unsigned
	{
		NEW,
		PARTIALLY_FILLED,
		FILLED,
		CANCELLED,
		REJECTED,
		EXPIRED
	};

	// Base class for Parent Orders
	// Inherit from this class to create specific types of parent orders
	// (e.g., NewParentOrder, CancelParentOrder, AmendParentOrder)
	class ParentOrder
	{
	public:
		ParentOrder() = default;

		ParentOrder(
			const std::string& symbol,
			const std::string& parentOrderId,
			const double price,
			const double quantity,
			const std::string& targetStrategy,
			const std::string& startTime,
			const std::string& endTime,
			const ParentOrderSide side,
			const ChildOrderType childOrderType,
			const ParentOrderType parentOrderType,
			const std::string& originalParentOrderId = "")
			: m_symbol(symbol),
			m_parentOrderId(parentOrderId),
			m_price(price),
			m_quantity(quantity),
			m_tartgetStrategy(targetStrategy),
			m_startTime(startTime),
			m_endTime(endTime),
			m_originalParentOrderId(originalParentOrderId),
			m_side(side),
			m_childOrdertype(childOrderType),
			m_parentOrderType(parentOrderType)
		{
			m_userAccountID = ApiKeyInfoMgr->GetApiKeyInfo().m_userID;
			m_updateTime = TimeUtils::GetEpochTimeTickNow(); // Initialize update time to current time
		}

		virtual ~ParentOrder() {}

		void SetSymbol(const std::string& symbol) {
			m_symbol = symbol;
		}

		const std::string& GetSymbol() const {
			return m_symbol;
		}

		const std::string& GetParentOrderId() const { 
			return m_parentOrderId;
		}

		void SetParentOrderId(const std::string& clientOrderId) { m_parentOrderId = clientOrderId; }


		void SetUserAccountID(const std::string& userID) {
			m_userAccountID = userID;
		}

		const std::string& GetUserAccountID() const {
			return m_userAccountID;
		}

		void SetUpdateTime(const std::size_t updateTime) {
			m_updateTime = updateTime;
		}

		std::string GetUpdateTimeStr() const
		{
			return TimeUtils::ConvertEpochTickToTimeString(m_updateTime);
		}

		std::size_t GetUpdateTime() const { return m_updateTime; }

		void SetSide(const ParentOrderSide side) {
			m_side = side;
		}

		ParentOrderSide GetSide() const {
			return m_side;
		}

		void SetChildOrderType(const ChildOrderType type) {
			m_childOrdertype = type;
		}

		ChildOrderType GetChildOrderType() const {
			return m_childOrdertype;
		}

		void SetParentOrderType(const ParentOrderType type) {
			m_parentOrderType = type;
		}

		ParentOrderType GetParentOrderType() const {
			return m_parentOrderType;
		}

		void SetParentOrderStatus(const ParentOrderStatus status) {
			m_parentOrderStatus = status;
		}

		ParentOrderStatus GetParentOrderStatus() const {
			return m_parentOrderStatus;
		}

		void SetPrice(const double price) {
			m_price = price;
		}

		double GetPrice() const { return m_price; }

		void SetQuantity(const double quantity) {
			m_quantity = quantity;
		}

		double GetQuantity() const { return m_quantity; }
		
		void SetTargetStrategy(const std::string& targetStrategy) {
			m_tartgetStrategy = targetStrategy;
		}

		const std::string& GetTargetStrategy() const { return m_tartgetStrategy; }

		void SetStartTime(const std::string& startTime) {
			m_startTime = startTime;
		}

		const std::string& GetStartTime() const { return m_startTime; }

		void SetEndTime(const std::string& endTime) {
			m_endTime = endTime;
		}

		const std::string& GetEndTime() const { return m_endTime; }

		void SetOriginalParentOrderId(const std::string& originalParentOrderId) {
			m_originalParentOrderId = originalParentOrderId;
		}

		const std::string& GetOriginalParentOrderId() const { return m_originalParentOrderId; }

		static ParentOrderSide ConvertStringToParentOrderSide(const std::string& sideStr) {
			if (sideStr == "BUY") {
				return ParentOrderSide::BUY;
			}
			else if (sideStr == "SELL") {
				return ParentOrderSide::SELL;
			}
			else {
				throw std::invalid_argument("Invalid ParentOrderSide string: " + sideStr);
			}
		}

		static ChildOrderType ConvertStringToChildOrderType(const std::string& typeStr) {
			if (typeStr == "MARKET") {
				return ChildOrderType::MARKET;
			}
			else if (typeStr == "LIMIT") {
				return ChildOrderType::LIMIT;
			}
			else if (typeStr == "STOP_MARKET") {
				return ChildOrderType::STOP_MARKET;
			}
			else if (typeStr == "STOP_LIMIT") {
				return ChildOrderType::STOP_LIMIT;
			}
			else if (typeStr == "TARGET_LIMIT") {
				return ChildOrderType::TARGET_LIMIT;
			}
			else if (typeStr == "TRAILING_STOP_MARKET") {
				return ChildOrderType::TRAILING_STOP_MARKET;
			}
			else {
				throw std::invalid_argument("Invalid ChildOrderType string: " + typeStr);
			}
		}

		static ParentOrderType ConvertStringToParentOrderType(const std::string& typeStr) {
			if (typeStr == "NEW") {
				return ParentOrderType::NEW;
			}
			else if (typeStr == "CANCEL") {
				return ParentOrderType::CANCEL;
			}
			else if (typeStr == "AMEND") {
				return ParentOrderType::AMEND;
			}
			else {
				throw std::invalid_argument("Invalid ParentOrderType string: " + typeStr);
			}
		}

		static ParentOrderStatus ConvertStringToParentOrderStatus(const std::string& statusStr) {
			if (statusStr == "NEW") {
				return ParentOrderStatus::NEW;
			}
			else if (statusStr == "PARTIALLY_FILLED") {
				return ParentOrderStatus::PARTIALLY_FILLED;
			}
			else if (statusStr == "FILLED") {
				return ParentOrderStatus::FILLED;
			}
			else if (statusStr == "CANCELLED") {
				return ParentOrderStatus::CANCELLED;
			}
			else if (statusStr == "REJECTED") {
				return ParentOrderStatus::REJECTED;
			}
			else if (statusStr == "EXPIRED") {
				return ParentOrderStatus::EXPIRED;
			}
			else {
				throw std::invalid_argument("Invalid ParentOrderStatus string: " + statusStr);
			}
		}

	protected:
		std::string m_userAccountID;
		std::string m_symbol;
		std::string m_parentOrderId; // m_parentOrderId is external parent order ID
		std::size_t m_updateTime{ 0 }; // Time order changed status
		double m_price{ 0 };
		double m_quantity{ 0 };
		std::string m_tartgetStrategy;
		std::string m_startTime;
		std::string m_endTime;
		std::string m_originalParentOrderId;
		ParentOrderSide m_side{ ParentOrderSide::BUY };
		ChildOrderType m_childOrdertype{ ChildOrderType::MARKET };
		ParentOrderType m_parentOrderType{ ParentOrderType::NEW };
		ParentOrderStatus m_parentOrderStatus{ ParentOrderStatus::NEW };
	};
};