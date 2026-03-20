/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "TradingGuard.h"
#include <chrono>

using namespace RiskManagement;

TradingGuard::TradingGuard()
	  : m_IsEnabled(true), m_IsKilled(false), m_Reason(HaltReason::None), m_HaltTimestamp(0)
{
}

TradingGuard::~TradingGuard() = default;

bool TradingGuard::IsTradingEnabled() const noexcept
{
	// FAST PATH: single atomic load
	return m_IsEnabled.load(std::memory_order_relaxed);
}

void TradingGuard::Pause(HaltReason reason) noexcept
{
	m_Reason.store(reason, std::memory_order_relaxed);
	m_HaltTimestamp.store(NowNs(), std::memory_order_relaxed);

	m_IsEnabled.store(false, std::memory_order_release);
}

void TradingGuard::Resume() noexcept
{
	// Cannot resume if kill switch is active
	if (m_IsKilled.load(std::memory_order_relaxed))
		return;

	m_Reason.store(HaltReason::None, std::memory_order_relaxed);
	m_IsEnabled.store(true, std::memory_order_release);
}

void TradingGuard::KillSwitch(HaltReason reason) noexcept
{
	m_Reason.store(reason, std::memory_order_relaxed);
	m_HaltTimestamp.store(NowNs(), std::memory_order_relaxed);

	m_IsKilled.store(true, std::memory_order_release);
	m_IsEnabled.store(false, std::memory_order_release);
}

bool TradingGuard::IsKilled() const noexcept
{
	return m_IsKilled.load(std::memory_order_relaxed);
}

TradingGuard::HaltReason TradingGuard::GetReason() const noexcept
{
	return m_Reason.load(std::memory_order_relaxed);
}

uint64_t TradingGuard::GetHaltTimestamp() const noexcept
{
	return m_HaltTimestamp.load(std::memory_order_relaxed);
}

uint64_t TradingGuard::NowNs() noexcept
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(
		   std::chrono::steady_clock::now().time_since_epoch()).count();
}