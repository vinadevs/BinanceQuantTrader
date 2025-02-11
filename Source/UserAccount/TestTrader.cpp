/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "../PortfolioManager/PortfolioInvestmentLocalTest.h"
#include "../OrderManagement/OrderCreator.h"
#include "../ExchangeSimulator/BackTestLocalExchange.h"

#include "TestTrader.h"

using namespace UserAccount;
using namespace PortfolioManager;
using namespace OrderManagement;
using namespace ExchangeSimulator;

TestTrader::TestTrader(PortfolioInvestmentLocalTest* portfolio)
	: m_portfolio(portfolio)
{
	m_logger = std::make_unique<LibraryUtils::Logger>("TestTrader");
	m_logger->Info("using TestTrader.");
}

bool TestTrader::Buy(const std::string& symbol, const binapi::double_type quality, const binapi::double_type refPrice)
{
	if (m_portfolio->GetCryptoAsset(symbol).GetAssetCashBalance() >
		0.0001 * refPrice)
	{
		/*const TestOrder buyOrder
			= OrderCreator::GetInstance()->CreateNewTestOrder(symbol, 0.0001, refPrice, TestOrderSide::BUY_SIDE);
		const CryptoAssetLocalTest updatedAsset
			= BackTestLocalExchange::GetInstance()->ExecuteTestOrder(buyOrder, m_portfolio-> GetCryptoAsset(symbol));

		m_logger->Info("bought an order at price= " + refPrice.str() + ", with qty= 0.0001");

		m_portfolio->UpdateInvestmentAsset(updatedAsset.GetSymbol(),
			updatedAsset.GetAssetQty(),
			updatedAsset.GetAssetCashBalance());*/
		return true;
	}
	else
	{
		m_logger->Info("out of money!");
		return false;
	}
}

bool TestTrader::Sell(const std::string& symbol, const binapi::double_type quality, const binapi::double_type refPrice)
{
	if (m_portfolio->GetCryptoAsset(symbol).GetAssetQty() > 0)
	{
		/*const TestOrder sellOrder
			= OrderCreator::GetInstance()->CreateNewTestOrder(symbol, m_portfolio-> GetCryptoAsset(symbol).GetAssetQty(), refPrice, TestOrderSide::SELL_SIDE);
		const CryptoAssetLocalTest updatedAsset
			= BackTestLocalExchange::GetInstance()->ExecuteTestOrder(sellOrder, m_portfolio-> GetCryptoAsset(symbol));

		m_logger->Info("sold an order at price= " + refPrice.str() + ", with qty= " + m_portfolio-> GetCryptoAsset(symbol).GetAssetQty().str());

		m_portfolio->UpdateInvestmentAsset(updatedAsset.GetSymbol(),
			updatedAsset.GetAssetQty(),
			updatedAsset.GetAssetCashBalance());*/
		return true;
	}
	else
	{
		m_logger->Info("out of asset!");
		return false;
	}
}

void TestTrader::ReportTradeData(const std::string& symbol)
{
	const auto& asset = m_portfolio-> GetCryptoAsset(symbol);

	m_logger->Info("############## ReportTradeData ##############");
	m_logger->Info("Symbol=" + symbol);
	m_logger->Info("AssetQty=" + asset.GetAssetQty().str());
	m_logger->Info("AssetCashBalance=" + asset.GetAssetCashBalance().str() + asset.GetCurrency());
	m_logger->Info("OriginalAssetCashBalance=" + asset.GetOriginalAssetCashBalance().str() + asset.GetCurrency());
	m_logger->Info("PNL=" + (asset.GetAssetCashBalance() - asset.GetOriginalAssetCashBalance()).str() + asset.GetCurrency());
	m_logger->Info("###########################################");
}
