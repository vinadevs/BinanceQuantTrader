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
#include "../RestAPI/RestAPI.h"
#include "../RestAPI/BinanceAPI.h"
#include "../LibraryUtils/Logger.h"

using namespace RestAPI;

bool BinanceAccountUtils::QueryBinanceAccount(
    binapi::rest::account_info_t& accountInfo,
    LibraryUtils::Logger* logger)
{
    if (BinanceApiGateWay)
    {
        const auto accountInfoResult = BinanceApiGateWay->account_info();
        if (!binapi::rest::e_error_equal(accountInfoResult.ec, binapi::rest::e_error::OK))
        {
            if (logger)
            {
                // If you are seeing the error:
                // "Timestamp for this request was 1000ms ahead of the server"
                // Please try to sycn your system time again in Date&Time setting
                logger->Error("AccountError: ec=" + std::to_string(accountInfoResult.ec)
                    + ", ename=" + binapi::rest::e_error_to_string(accountInfoResult.ec)
                    + ", emsg=" + accountInfoResult.errmsg);
            }
            return false;
        }
        else
        {
            if (!static_cast<bool>(accountInfoResult))
            {
                return false;
            }
            if (logger)
            {
                //LOG_DEBUG_STREAM(logger, "AccountInfo=" << accountInfoResult.v);
                accountInfo = accountInfoResult.v;
                return true;
            }
        }
    }
    return false;
}
