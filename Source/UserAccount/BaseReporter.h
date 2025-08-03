/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "../RestAPI/RestAPI.h"

#include <memory>

namespace tinyxml2 {
	class XMLElement;
};

namespace LibraryUtils {
	class Logger;
};

namespace UserAccount {

	enum class ReportChannel : unsigned
	{
		CONSOLE, // print report to console cmd
		EXTERNAL_FILE, // log report to file
		GUI_APP, // send report to external Gui app
		UNDEF,
	};

	// The BaseReporter class is an abstract base 
	// class that defines the interface for generating reports on trading activities.
	class BaseReporter
	{
	public:
		BaseReporter() = default;

		virtual ~BaseReporter() {};

		// Setup the reporter with the given configuration XML element.
		virtual void SetupReporter(const tinyxml2::XMLElement* reportCfg) = 0;
		// Update remote account data for the given symbol.
		virtual void UpdateRemoteData(const std::string& symbol) = 0;
		// Update remote report for trades for the given symbol.
		virtual void DoTradeExecutionReport(const std::string& symbol) = 0;

	protected:
		ReportChannel FromReportChannelTextToEnum(const std::string& reportChannel)
		{
			if (reportChannel == "Console")
				return ReportChannel::CONSOLE;
			else if (reportChannel == "ExternalFile")
				return ReportChannel::EXTERNAL_FILE;
			else if (reportChannel == "GuiApp")
				return ReportChannel::GUI_APP;
			else
				return ReportChannel::UNDEF;
		}

		std::unique_ptr<LibraryUtils::Logger> m_logger;

		// external file to store report
		std::string m_reportToFilePath;
		// report channel
		ReportChannel m_reportChannel{ ReportChannel::UNDEF };
	};
};

