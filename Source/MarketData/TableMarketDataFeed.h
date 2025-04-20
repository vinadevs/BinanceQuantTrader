/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"

#include "FeedIdentification.h"

#include <map>
#include <string>

namespace MarketData {

	template<typename T>
	using TableMarketData = std::map<std::string, T>;

	template <typename T>
	class TableMarketDataFeed
	{
	public:
		void SetData(const TableMarketData<T>& newData);
		const TableMarketData<T>& GetData() const;

	private:
		TableMarketData<T> m_data;
		MarketDataFeedStatus m_mkdataFeedStatus{ MarketDataFeedStatus::INIT };
	};

	template<typename T>
	inline void TableMarketDataFeed<T>::SetData(const TableMarketData<T>& newData)
	{
		m_data = newData;
		m_mkdataFeedStatus = MarketDataFeedStatus::UPDATED;
	}

	template<typename T>
	inline const TableMarketData<T>& TableMarketDataFeed<T>::GetData() const
	{
		return m_data;
	}
};