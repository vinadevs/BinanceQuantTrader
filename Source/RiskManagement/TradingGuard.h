/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include "dlldefine.h"

#include <atomic>
#include <cstdint>

/**
 * Global trading safety guard (Kill Switch)
 *
 * PURPOSE:
 *  - Stop all trading instantly when abnormal condition detected
 *  - Protect against:
 *      + Market anomalies
 *      + Strategy bugs
 *      + Connectivity issues
 *      + Risk breaches
 *
 * DESIGN:
 *  - Lock-free (atomic only)
 *  - Ultra fast check (nanosecond level)
 *  - Thread-safe for multi-thread trading system
 *

USAGE THIS GUARD:

Case 1: Check before sending order
 if (!TradingGuardInstance.IsTradingEnabled())
{
    // drop order immediately
    return;
}

Case 2: Trigger halt on risk breach
if (pnl < -maxLoss)
{
    Guard.KillSwitch(TradingGuard::HaltReason::RiskLimitExceeded);
}

Case 3: Trigger halt on market anomaly
onHalt(reason)

Case 4: Global instance for all strategies
TradingGuard perStrategy[NUM_STRATEGIES];

Case 5: Example risk check
if (exposure > limit || pnl < limit)
    KillSwitch(RiskLimitExceeded);

*/

namespace RiskManagement {

	class DLL_CLASS_RISKMANAGEMENT_EXPORTS TradingGuard final
	{
	public:
		/**
		 * Reason for trading halt
		 */
		enum class HaltReason : unsigned
		{
			None = 0,
			Manual,				// user triggered
			RiskLimitExceeded,	// PnL / exposure breach
			MarketAnomaly,		// abnormal market condition
			ConnectivityLoss,	// lost connection to exchange
			SystemError			// internal error
		};

	public:
		TradingGuard();
		~TradingGuard();

		/**
		 * Check if trading is allowed
		 *
		 * FAST PATH: called on every order send
		 */
		bool IsTradingEnabled() const noexcept;

		/**
		 * Pause trading (soft stop)
		 */
		void Pause(HaltReason reason) noexcept;

		/**
		 * Resume trading
		 */
		void Resume() noexcept;

		/**
		 * Hard stop (kill switch, irreversible unless reset)
		 */
		void KillSwitch(HaltReason reason) noexcept;

		/**
		 * Check if kill switch is active
		 */
		bool IsKilled() const noexcept;

		/**
		 * Get current halt reason
		 */
		HaltReason GetReason() const noexcept;

		/**
		 * Timestamp (epoch ns) when halted
		 */
		uint64_t GetHaltTimestamp() const noexcept;

	private:
		/**
		 * Trading enabled flag (fast path)
		 */
		std::atomic<bool> m_IsEnabled;

		/**
		 * Kill switch flag
		 */
		std::atomic<bool> m_IsKilled;

		/**
		 * Reason for halt
		 */
		std::atomic<HaltReason> m_Reason;

		/**
		 * Halt timestamp (nanoseconds)
		 */
		std::atomic<uint64_t> m_HaltTimestamp;

	private:
		static uint64_t NowNs() noexcept;
	};
};	// namespace RiskManagement