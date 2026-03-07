/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include <string>
#include <cstdint>
#include <map>
#include <unordered_map>

#include "../QuantitativeModel/QuantOrderParammeter.h"

/**
 * @brief InstrumentQuoter is responsible for quoting and managing orders for a single derivative instrument.
 * 
 * This class is used in derivatives trading to maintain and update bid/offer quotes based on theoretical prices,
 * market data, and order confirmations. It tracks orders that are sent but not yet confirmed, as well as those
 * that have been confirmed, using separate pools. The class ensures that quotes are placed at a specified offset
 * from the theoretical price and at valid tick levels. It also manages the state of bid and offer orders, including
 * their addition and removal confirmations. The InstrumentQuoter is designed for high-frequency trading strategies
 * where precise and timely order management is critical.
 * 
 * Typical usage involves:
 *  - Instantiating the quoter for a specific instrument (feedcode).
 *  - Receiving market data and theoretical price updates.
 *  - Sending and confirming quote and hedge orders.
 *  - Tracking and updating the state of each order.
 */

class InstrumentQuoter final
{
private:
	std::string m_feedcode;
    double m_quoteOffset{ 0.0 };
	double m_tickWidth{ 0.0 };
	double m_theoreticalPrice{ 0.0 };
	double m_bidPrice{ 0.0 };
	double m_offerPrice{ 0.0 };
	bool m_orderBidAdded{ false };
	bool m_orderOfferAdded{ false };
	bool m_orderBidRemoved{ true };
	bool m_orderOfferRemoved{ true };
	uint32_t m_orderIDCounter{ 0 };
	std::unordered_map<uint32_t, QuantitativeModel::QuantOrderParammeter> m_quoteOrderPool;	// tracks orders that have been sent but not yet confirmed
	std::unordered_map<uint32_t, QuantitativeModel::QuantOrderParammeter> m_quotedOrderPool;  // tracks orders that have been confirmed added

	const QuantitativeModel::QuantOrderParammeter& LookupQuoteAndHedgeOrder(uint32_t id) const;
	const QuantitativeModel::QuantOrderParammeter& LookupQuotedAndHedgedOrder(uint32_t id) const;
public:
    InstrumentQuoter( // Instantiates the quoter for a single instrument
        std::string const& feedcode, // The exchange identifier of the instrument
        double quoteOffset, // The minimum desired distance between our theoretical price and each order we send
		double tickWidth);  // The distance between valid exchange price levels
		
    void OnQuoteAndHedgeOrder(
		double theoreticalPrice,
		uint32_t quoteVolume,  // The desired volume for each order we send
        double bidPrice, // The best price bid on the exchange
        double offerPrice); // The best price offer on the exchange
    void OnOrderAddConfirm( // Notifies the quoter that an "order add" request has been confirmed
        uint32_t id); // The request identifier of the order
    void OnOrderRemoveConfirm( // Notifies the quoter that an "order remove" request has been confirmed
        uint32_t id); // The request identifier of the order

	void DumpOrderInfo(const QuantitativeModel::QuantOrderParammeter& order, const std::string& action) const;
};
