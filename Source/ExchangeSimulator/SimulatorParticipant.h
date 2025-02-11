/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "UserAccountManager.h"
#include "Participant.h"

namespace ExchangeSimulator {

    class UserAccountManager;
    class SimulatorParticipant final : public Participant
    {
    public:
        SimulatorParticipant(UserAccountManager* userAccountManager);
        ~SimulatorParticipant() override;
        bool TryToMatchOrder(OrderManagement::BinanceNewOrder& ack) override { return false; }
    private:

    };
};
