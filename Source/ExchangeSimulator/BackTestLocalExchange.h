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
#include "../LibraryUtils/MacroUtils.h"
#include "../OrderManagement/TestOrder.h"
#include "../PortfolioManager/CryptoAssetLocalTest.h"

#include <string>

// Test trade system only, can not get money here :D
// Connecting to Local's exchange by this SINGLETON class allows us
// to place and queries orders 

namespace ExchangeSimulator {
	class DLL_CLASS_EXCHANGESIMULATOR_EXPORTS BackTestLocalExchange
	{
	public:
		BackTestLocalExchange() = default;
		~BackTestLocalExchange();

		DISABLE_COPY_AND_MOVE(BackTestLocalExchange);

		static BackTestLocalExchange* GetInstance();

		PortfolioManager::CryptoAssetLocalTest ExecuteTestOrder(const OrderManagement::TestOrder& buyOrder,
										           const PortfolioManager::CryptoAssetLocalTest& currentUserAsset);
	private:
		binapi::double_type m_BtcMarketVolume{ 0.0 };
	};
};