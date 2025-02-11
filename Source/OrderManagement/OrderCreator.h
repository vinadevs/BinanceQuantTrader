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
#include "../RestAPI/RestAPI.h"

#include "BinanceNewOrder.h"
#include "BinanceCancelOrder.h"
#include "BinanceReplaceOrder.h"

#include <string>
#include <memory>

namespace OrderManagement {

	// This class works like a helper/factory to create new orders
	class DLL_CLASS_ORDERMANAGEMENT_EXPORTS OrderCreator final
	{
	public:
		std::unique_ptr<BinanceNewOrder> CreateNewBinanceOrderFull(
			  const std::string& symbol
			, const binapi::e_side side
			, const binapi::e_type type
			, const binapi::e_time time
			, const double amount
			, const double price
			, const std::string& clientOrderId
			, const std::string& stopPrice
			, const std::string& icebergAmount);

		std::unique_ptr<BinanceNewOrder> CreateNewBinanceTestOrderFull(
			  const std::string& symbol
			, const binapi::e_side side
			, const binapi::e_type type
			, const binapi::e_time time
			, const double amount
			, const double price
			, const std::string& clientOrderId
			, const std::string& stopPrice
			, const std::string& icebergAmount);

		std::unique_ptr<BinanceCancelOrder> CreateCancelBinanceOrder(
			  const std::string& symbol
			, const std::size_t orderId
			, const std::string& origClientOrderId
			, const std::string& clientOrderId);

		std::unique_ptr<BinanceReplaceOrder> CreateCancelReplaceBinanceOrder(
			  const std::string& symbol
			, const std::size_t orderId
			, const std::string& origClientOrderId
			, const std::string& clientOrderId);
	};
};