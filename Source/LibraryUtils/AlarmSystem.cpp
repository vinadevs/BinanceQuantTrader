/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "AlarmSystem.h"

using namespace LibraryUtils;

AlarmSystem::AlarmSystem(const long repeatInterval, AlarmMode mode)
    : m_intervalMs(repeatInterval),
    m_mode(mode),
    m_running(false),
    m_alarmRequested(false), 
    m_customInterval(std::nullopt) {}

AlarmSystem::~AlarmSystem() {
    Stop();
}

void AlarmSystem::Start() {
    m_running.store(true);
    m_thread = std::thread(&AlarmSystem::Run, this);
}

void AlarmSystem::Stop() {
    m_running.store(false);
    m_alarmRequested.store(false);
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

void AlarmSystem::RequestAlarm(long customIntervalMs) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_alarmRequested.store(true);
    if (customIntervalMs > 0) {
        m_customInterval = customIntervalMs; // Set temporary custom interval
    }
    else {
		throw std::invalid_argument("Custom interval must be greater than 0.");
    }
}

void AlarmSystem::SetRepeatInterval(const long repeatInterval) {
	std::lock_guard<std::mutex> lock(m_mutex);
    if (repeatInterval > 0) {
        m_intervalMs = repeatInterval; // Set custom interval
    }
    else {
        throw std::invalid_argument("Repeat interval must be greater than 0.");
    }
}

void AlarmSystem::Run() {
    while (m_running.load()) {
        long currentInterval = m_intervalMs; // Default interval
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_customInterval.has_value()) {
                currentInterval = m_customInterval.value(); // Use custom interval if set
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(currentInterval));
        if (m_running.load()) {
            std::lock_guard<std::mutex> lock(m_mutex);
            if ((m_mode == AlarmMode::REPEAT) ||
                (m_mode == AlarmMode::SINGLE_RUN && m_alarmRequested.load()) ||
                (m_mode == AlarmMode::ON_REQUEST && m_alarmRequested.load())) {

                OnAlarmTriggered();
                m_alarmRequested.store(false);  // Reset the request flag

                // Clear custom interval after it has been used once
                m_customInterval.reset();

                // If mode is SingleRun, stop after one trigger
                if (m_mode == AlarmMode::SINGLE_RUN) {
                    Stop();
                }
            }
        }
    }
}
