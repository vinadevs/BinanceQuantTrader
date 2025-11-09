/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "BinanceAccountUtils.h"

#include "../KernelTrading/errors.h"
#include "../KernelTrading/types.h"
#include "../RestAPI/RestAPI.h"
#include "../RestAPI/BinanceSpotApiGateWay.h"
#include "../RestAPI/ApiKeyInfoManager.h"
#include "../LibraryUtils/Logger.h"
#include "../LibraryUtils/SourceBuildFlags.h"

#if USE_BACK_TEST_TRADING
#include "../ExchangeConnectivity/ExchangeSimulatorConnectivity.h"
#endif

#if USE_BACK_TEST_TRADING
using namespace ExchangeConnectivity;
#else
using namespace RestAPI;
#endif

bool BinanceAccountUtils::QueryBinanceSpotAccount(
    binapi::rest::account_info_t* account,
    LibraryUtils::Logger* logger)
{
#if USE_BACK_TEST_TRADING // use simulator protobuf message
    std::string errorMessage;
    if (ExchangeSimulatorGateWay->GetUserAccountInfo(
        ApiKeyInfoMgr->GetApiKeyInfo().m_userID, account, errorMessage))
    {
        //LOG_DEBUG_STREAM(logger, "AccountInfo=" << accountInfo);
        return true;
    }
    else
    {
        logger->Error("AccountError:" + errorMessage);
        return false;
    }
#else // use Binance HTTP API
    if (BinanceSpotApiGateWayMgr)
    {
        const auto accountInfoResult = BinanceSpotApiGateWayMgr->account_info();
        if (!binapi::rest::e_error_equal(accountInfoResult.ec, binapi::rest::e_error::OK))
        {
            // If you are seeing the error:
            // "Timestamp for this request was 1000ms ahead of the server"
            // Please try to sycn your system time again in Date&Time setting
            logger->Error("AccountError: ec=" + std::to_string(accountInfoResult.ec)
                + ", ename=" + binapi::rest::e_error_to_string(accountInfoResult.ec)
                + ", emsg=" + accountInfoResult.errmsg);
            return false;
        }
        else
        {
            if (!static_cast<bool>(accountInfoResult))
            {
                return false;
            }
            //LOG_DEBUG_STREAM(logger, "AccountInfo=" << accountInfoResult.v);
            DEREF_V(account) = accountInfoResult.v;
            return true;
        }
    }
    return false;
#endif
}

bool BinanceAccountUtils::QueryBinanceFutureAccount(
    KernelTrading::UserFutureAccount* account,
    LibraryUtils::Logger* logger)
{
#if USE_BACK_TEST_TRADING // use simulator protobuf message
	std::string errorMessage;
	if (ExchangeSimulatorGateWay->GetUserFutureAccountInfo(
		ApiKeyInfoMgr->GetApiKeyInfo().m_userID, account, errorMessage))
	{
		//LOG_DEBUG_STREAM(logger, "FutureAccountInfo=" << accountInfo);
		return true;
	}
	else
	{
		logger->Error("FutureAccountError:" + errorMessage);
		return false;
	}
#else // use Binance HTTP API
	return false;
#endif
}
