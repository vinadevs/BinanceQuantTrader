/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include "../LibraryUtils/MacroUtils.h"

#include <string>

namespace tinyxml2 {
	class XMLElement;
};

namespace LibraryUtils {
	class Logger;
};

//**************************************************************************************
// CLASS: BqtGlobalSettings
// -------------------------
// This class is designed to store and manage application-wide settings for the Binance 
// Quant Trader (BQT) system. It serves as a centralized configuration holder, allowing 
// various application components to access shared settings in a consistent manner.
//
// This class follows the Singleton design pattern to ensure only one instance is 
// created and accessible throughout the application lifecycle.
//
//**************************************************************************************

namespace SettingNConfig {

	class DLL_CLASS_SETTINGS_AND_CONFIGS_EXPORTS 
		BqtGlobalSettings final
	{
	public:
		BqtGlobalSettings();
		~BqtGlobalSettings();

		DISABLE_COPY_AND_MOVE(BqtGlobalSettings);

		static BqtGlobalSettings* GetInstance();

		void InitGlobalSetting(const tinyxml2::XMLElement* globalSettingsCfg);

		const std::string& GetdDataAppPath() const;
	private:
		std::unique_ptr<LibraryUtils::Logger> m_logger;
		std::string m_dataAppPath;
	};
};
// Lets shorten the code line!
#define BqtGlobalSettingsMgr SettingNConfig::BqtGlobalSettings::GetInstance()