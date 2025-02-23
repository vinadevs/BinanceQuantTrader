/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "BqtJsonMessage.h"

namespace MiddlewareMQ {

    /* This base class has single one method OnHandlingReceivedSimulatorMessage
        that will be used to pass through BqtJsonMessage to target handler. */
    class MessageHandler
    {
    public:
        virtual void OnHandlingReceivedSimulatorMessage(const BqtJsonMessage& message) = 0;
    };
};

#endif // MESSAGE_HANDLER_H
