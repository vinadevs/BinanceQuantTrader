/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include <string>

namespace LibraryUtils {
	class Logger;
};

namespace tinyxml2 {
	class XMLElement;
};

namespace UserAccount {
	class BinanceReporter
	{
	public:
		BinanceReporter(const tinyxml2::XMLElement* reportConfigXml);
		BinanceReporter() = default;
		~BinanceReporter();

		void ReportTrades(const std::string& symbol);
		void ReportOpenOrders(const std::string& symbol);
		void ReportAccountBalance(const std::string& symbol);
	private:
		std::unique_ptr<LibraryUtils::Logger> m_logger;
	};
};

