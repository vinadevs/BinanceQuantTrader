/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include <iostream>
#include <string>
#include <utility>

#include <zmq.h>

#include "dlldefine.h"

namespace MiddlewareMQ {

    using zmq_api_result = int;

    // ZeroMQ API result structure, it has error message and result flag
    struct DLL_CLASS_MIDDLEWAREMQ_EXPORTS MiddlewareMQResult final
    {
        MiddlewareMQResult() = default;
        MiddlewareMQResult(std::string errMsg, bool result)
            : m_errMsg(std::move(errMsg)), m_result(result) {}
        std::string m_errMsg;
        bool m_result{ false };

        // returns FALSE when error
        explicit operator bool() const { return m_errMsg.empty(); }
    };

    // ZeroMQ API result is integer number, this util will parse error message and result flag
    MiddlewareMQResult TranslateResult(int result);
}