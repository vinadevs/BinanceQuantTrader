/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "CryptoAssetLocalTest.h"

using namespace PortfolioManager;

CryptoAssetLocalTest::CryptoAssetLocalTest(const std::string& symbol, 
	const binapi::double_type assetQty,
	const binapi::double_type assetCashBalance)
	: m_symbol(symbol),
	m_assetQty(assetQty),
	m_assetCashBalance(assetCashBalance)
{
}

void PortfolioManager::CryptoAssetLocalTest::SetOriginalAssetCashBalance(const binapi::double_type assetCashBalance)
{
	m_originalAssetCashBalance = assetCashBalance;
}

void CryptoAssetLocalTest::UpdateAssetQty(const binapi::double_type assetQty)
{
	m_assetQty = assetQty;
}

void CryptoAssetLocalTest::UpdateAssetCashBalance(const binapi::double_type assetCashBalance)
{
	m_assetCashBalance = assetCashBalance;
}
