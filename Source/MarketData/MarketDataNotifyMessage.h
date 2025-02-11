/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

namespace MarketData {
	class MarketDataNotifyMessage
	{
	public:
		enum class ChangeType : unsigned
		{
			IBTD,
		};
	};
};