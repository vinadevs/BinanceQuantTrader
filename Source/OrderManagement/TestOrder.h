/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"
#include "../KernelTrading/double_type.h"
#include "Order.h"

namespace OrderManagement {
	enum class TestOrderSide : unsigned
	{
		BUY_SIDE,
		SELL_SIDE,
		UNKNOWN,
	};

	class DLL_CLASS_ORDERMANAGEMENT_EXPORTS
		TestOrder final : public Order
	{
	public:
		TestOrder(
			const std::string& clientOrderId,
			const std::string& symbol,
			const binapi::double_type quality,
			const binapi::double_type referencePrice,
			const TestOrderSide side);

		const std::string& GetSymbol() const { return m_symbol; }
		binapi::double_type GetAssetQty() const { return m_assetQty; }
		binapi::double_type GetAssetReferencePrice() const { return m_assetReferencePrice; }
		TestOrderSide GetOrderSide() const { return m_orderSide; }
	private:
		binapi::double_type m_assetQty{ 0.0 };
		binapi::double_type m_assetReferencePrice{ 0.0 };
		TestOrderSide m_orderSide{ TestOrderSide::UNKNOWN };
	};
};