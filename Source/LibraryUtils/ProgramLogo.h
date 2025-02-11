/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

// TODO: WE ASSUMED THAT ALL APPLICATIONS WILL RUN ON WINDOWS
// PLATFORM ONLY, BUT WE SHOULD HAVE SUPPORTED FOR LINUX AS WELL
// SOMETHING LIKE #IFDEF WINDOWS_ ELSE LINUX_
#include <windows.h>
#include <Lmcons.h>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <exception>

#include "TimeUtils.h"

// This header privides a basic stuffs for a command line program

namespace ProgramUtils {

    enum class Programs : unsigned
    {
        BQT, // auto trading system
        EXCHANGE_SIMULATOR, // exchange simulator
        MESSAGE_HUB_SERVER, // message broker
        // If you add new type here, you have to update PRINT_PROGRAM_HEADER() below too!
    };

    void PRINT_PROGRAM_HEADER(const Programs& exe)
    {
        // Get current time

        std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n";

        // Print program name
        if (exe == Programs::BQT) {
            std::cout << "     BINANCE QUANT TRADING MODEL" << std::endl;;
        }
        else if (exe == Programs::EXCHANGE_SIMULATOR) {
            std::cout << "     BINANCE EXCHANGE SIMULATOR" << std::endl;;
        }
        else if (exe == Programs::MESSAGE_HUB_SERVER) {
            std::cout << "     BINANCE MESSAGE HUB SERVER" << std::endl;;
        }
        else {
            throw std::runtime_error("Header: Invalid program type...");
        }

        // Print start time

        std::cout << "StartTime: " << TimeUtils::GetCurrentTimestamp() << std::endl;

        // Print system info
        char computerName[MAX_COMPUTERNAME_LENGTH + 1];
        DWORD size = sizeof(computerName) / sizeof(computerName[0]);
        if (GetComputerNameA(computerName, &size))
        {
            std::cout << "ComputerName: " << computerName << std::endl;
        }
        else
        {
            std::cout << "Error getting computer name" << std::endl;
        }

        // Print user info
        char userName[UNLEN + 1];
        size = sizeof(userName) / sizeof(userName[0]);
        if (GetUserNameA(userName, &size))
        {
            std::cout << "User: " << userName << std::endl;
        }
        else
        {
            std::cout << "Error getting user name" << std::endl;
        }

        // Print executable path
        char exePath[MAX_PATH];
        if (GetModuleFileNameA(NULL, exePath, MAX_PATH))
        {
            std::cout << "ExecutablePath: " << exePath << std::endl;
        }
        else
        {
            std::cout << "Error getting executable path" << std::endl;
        }

        std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n" << std::endl;
    }
};

/*************************************************************************************************/

#ifndef BINANCE_QUANT_TRADING_MODEL
#define BINANCE_QUANT_TRADING_MODEL_TITLE "BINANCE_QUANT_TRADING_MODEL"
#define BINANCE_QUANT_TRADING_MODEL \
int main(int argc, char** argv)
#endif // BINANCE_QUANT_TRADING_MODEL

/*************************************************************************************************/

#ifndef BINANCE_EXCHANGE_SIMULATOR
#define BINANCE_EXCHANGE_SIMULATOR_TITLE "BINANCE_EXCHANGE_SIMULATOR"
#define BINANCE_EXCHANGE_SIMULATOR \
int main(int argc, char** argv)
#endif // BINANCE_EXCHANGE_SIMULATOR

/*************************************************************************************************/

#ifndef BINANCE_MESSAGE_HUB_SERVER
#define BINANCE_MESSAGE_HUB_SERVER_TITLE "BINANCE_MESSAGE_HUB_SERVER"
#define BINANCE_MESSAGE_HUB_SERVER \
int main(int argc, char** argv)
#endif // BINANCE_MESSAGE_HUB_SERVER