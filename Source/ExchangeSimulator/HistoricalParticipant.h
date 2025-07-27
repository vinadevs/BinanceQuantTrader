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
    class HistoricalParticipant final : public Participant
    {
    public:
        HistoricalParticipant(UserAccountManager* userAccountManager);
        ~HistoricalParticipant() override;
        bool TryToMatchOrder(OrderManagement::BinanceNewOrder& order) override { return false; }
        void HandleUserBalanceAfterCancelOrder(const OrderManagement::BinanceNewOrder& order) override {}
    private:

    };
};
