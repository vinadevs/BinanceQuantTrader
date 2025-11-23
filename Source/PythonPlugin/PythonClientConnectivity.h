/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include "../MiddlewareMQ/MessageDelivery.h"
#include "../LibraryUtils/MacroUtils.h"

#include <string>

namespace LibraryUtils {
	class Logger;
};

namespace tinyxml2 {
	class XMLElement;
};

namespace PythonPlugin {

	// Connecting to Simulator's exchange by this SINGLETON class allows us
	// to place and queries orders from downstream side

	class DLL_CLASS_PYTHONPLUGIN_EXPORTS PythonClientConnectivity final
	{
	public:
		DISABLE_COPY_AND_MOVE(PythonClientConnectivity);

		static PythonClientConnectivity* GetInstance();

		void InitMessageTransporter(const tinyxml2::XMLElement* messageDeliveryCfg);

	private:
		PythonClientConnectivity();
		~PythonClientConnectivity();

		std::unique_ptr<LibraryUtils::Logger> m_logger;
		std::unique_ptr<MiddlewareMQ::MessageDelivery> m_messageDelivery;
	};
};
// Lets shorten the code line!
#define ExchangeSimulatorGateWay PythonPlugin::PythonClientConnectivity::GetInstance()