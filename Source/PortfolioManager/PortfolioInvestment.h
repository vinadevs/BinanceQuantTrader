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
#include "../KernelTrading/types.h"
#include "../LibraryUtils/Logger.h"

#include <string>
#include <vector>
#include <memory>

namespace PortfolioManager {
	enum class PortfolioType : unsigned
	{
		UNDEFINED,
		LOCAL_TEST,
		BINANCE_ASSET,
	};

	// A portfolio investment refers to the allocation of capital 
    // into various financial assets
	class DLL_CLASS_PORTFOLIOMANAGER_EXPORTS PortfolioInvestment
	{
	public:
		PortfolioInvestment(const PortfolioType type);
		virtual ~PortfolioInvestment();
		PortfolioType GetPortfolioType() const { return m_portfolioType; }
	protected:
		PortfolioType m_portfolioType{ PortfolioType::UNDEFINED };
		std::unique_ptr<LibraryUtils::Logger> m_logger;
	};
};