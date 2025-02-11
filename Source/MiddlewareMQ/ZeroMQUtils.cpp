/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "ZeroMQUtils.h"

// ZeroMQ API result is integer number, this util will parse error message and result flag
MiddlewareMQ::MiddlewareMQResult MiddlewareMQ::TranslateResult(int result) {
    if (result == -1) {
        const std::string errorMsg = zmq_strerror(result);
        std::cerr << "ZeroMQ api failed: " << errorMsg << std::endl;
        return MiddlewareMQ::MiddlewareMQResult(errorMsg, false);
    }
    return MiddlewareMQ::MiddlewareMQResult("", true);
}
