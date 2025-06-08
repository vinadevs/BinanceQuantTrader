/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "FeedIdentification.h"

#include <vector>

#pragma warning(disable : 4244)
#pragma warning(disable : 4251)

namespace MarketData {

	template<typename T>
	using ArrayMarketData = std::vector<T>;

	template <typename T>
	class ArrayMarketDataFeed
	{
	public:
		void SetData(const ArrayMarketData<T>& newData);
		const ArrayMarketData<T>& GetData() const;

	private:
		ArrayMarketData<T> m_data;
		MarketDataFeedStatus m_mkdataFeedStatus{ MarketDataFeedStatus::INIT };
	};

	template<typename T>
	inline void ArrayMarketDataFeed<T>::SetData(const ArrayMarketData<T>& newData)
	{
		m_data.clear();
		m_data = newData;
	}

	template<typename T>
	inline const ArrayMarketData<T>& ArrayMarketDataFeed<T>::GetData() const
	{
		return m_data;
	}

};