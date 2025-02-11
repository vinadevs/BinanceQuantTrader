/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "GeneralUtils.h"

#include <stdio.h>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <random>

BOOL WINAPI GeneralUtils::ConsoleCtrlHandler(DWORD ctrlType)
{
    switch (ctrlType)
    {
    case CTRL_C_EVENT:
        printf("System Signals: Ctrl+C pressed.\n");
        return FALSE; // Indicate that the event was handled
    case CTRL_CLOSE_EVENT:
        printf("System Signals: Console window closing.\n");
        return FALSE;
    default:
        return FALSE; // Pass the event to the default handler
    }
}

std::string GeneralUtils::GenerateUniqueID(const std::string& instanceName)
{
    // Get the current time since epoch in microseconds
    const auto now = std::chrono::system_clock::now();
    const auto duration = now.time_since_epoch();
    const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    // Use a random number generator to add more uniqueness
    std::random_device rd;  // Seed the generator
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine
    std::uniform_int_distribution<> dist(0, 99999); // Range of random values
    // Use stringstream to create a unique ID by combining the time and a random number
    std::stringstream ss;
    ss << std::hex << std::setw(8) << std::setfill('0') << microseconds;  // Time part in hex
    ss << "-" << std::setw(5) << std::setfill('0') << dist(gen);           // Random part
    return instanceName + "-" + ss.str();
}