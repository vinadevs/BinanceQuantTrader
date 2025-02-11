/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#ifndef ALARMSYSTEM_H
#define ALARMSYSTEM_H

#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>
#include <optional>

#include "MacroUtils.h"
#include "dlldefine.h"

/**
 * The AlarmSystem class provides an interface for creating an alarm that triggers
 * a virtual `OnAlarmTriggered` method at a specified interval. The alarm can operate
 * in different modes such as repeating, single-run, or on-request, providing flexibility
 * for various use cases.
 */

namespace LibraryUtils
{
    class DLL_CLASS_LIBRARYUTILS_EXPORTS AlarmSystem {
    public:
        enum class AlarmMode : unsigned {
            SINGLE_RUN, // alarm single time
            REPEAT, // repeat alarm after period
            ON_REQUEST, // repeat alarm when there is next request
        };

        AlarmSystem(const long customIntervalMs, AlarmMode mode = AlarmMode::REPEAT);
        virtual ~AlarmSystem();

        DISABLE_COPY_AND_MOVE(AlarmSystem);

        // Starts the alarm system in a background thread
        void Start();

        // Stops the alarm system
        void Stop();

        // Allows derived classes to request an alarm trigger with a custom interval
        void RequestAlarm(long customIntervalMs = 0);

    protected:
        // Virtual method to be overridden by derived classes to receive alarm alerts
        virtual void OnAlarmTriggered(const int passToDerived = 0) = 0;

    private:
        // The main function that runs in the background thread, adapted for multiple modes
        void Run();

        std::thread m_thread;
        std::atomic<bool> m_running; // lock free thread
        std::atomic<bool> m_alarmRequested; // lock free thread
        std::mutex m_mutex;
        long m_intervalMs;
        AlarmMode m_mode;
        std::optional<long> m_customInterval;
    };
};

#endif // ALARMSYSTEM_H
