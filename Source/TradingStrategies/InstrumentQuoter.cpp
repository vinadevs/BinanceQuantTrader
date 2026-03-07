/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "InstrumentQuoter.h"

#include <cmath>
#include <stdexcept>
#include <iostream>

namespace Utils {
	static inline double AlignBidPrice(double price, double tick)
	{
		return std::floor(price / tick) * tick;
	}

	static inline double AlignOfferPrice(double price, double tick)
	{
		return std::ceil(price / tick) * tick;
	}
}  // namespace Utils

InstrumentQuoter::InstrumentQuoter(
	std::string const& feedcode,
	double quoteOffset,
	double tickWidth)
	: m_feedcode(feedcode)
	, m_quoteOffset(quoteOffset)
	, m_tickWidth(tickWidth)
{
	if (m_feedcode.empty()) {
		throw std::invalid_argument("Error: feedcode is empty");
	}
	if (m_quoteOffset <= 0.0) {
		throw std::invalid_argument("Error: quoteOffset is not positive");
	}
	if (m_tickWidth <= 0.0) {
		throw std::invalid_argument("Error: tickWidth is not positive");
	}
}

void InstrumentQuoter::OnQuoteAndHedgeOrder(
	double theoreticalPrice, 
	uint32_t quoteVolume,
	double bidPrice, 
	double offerPrice)
{
	if (bidPrice < 0.0 || offerPrice < 0.0) {
		throw std::invalid_argument("Error: best bid/offer prices are invalid");
	}
	// Bid orders must not be closer to the theoretical price than the offset at the time they're sent
	const double theoreticalBidQuotePrice = m_theoreticalPrice - m_quoteOffset;

	// Bid should be rounded down to the nearest tick to align to the exchange tick
	m_bidPrice = Utils::AlignBidPrice(bidPrice, m_tickWidth);
	// Offer should be rounded up to the nearest tick to align to the exchange tick
	m_offerPrice = Utils::AlignOfferPrice(offerPrice, m_tickWidth);

	// Previous bid order must be confirmed removed before a new add request is sent on the bid side
	if (m_orderBidRemoved) {
		const double alignedTheoreticalBidOrderPrice = Utils::AlignBidPrice(theoreticalBidQuotePrice, m_tickWidth);
		double orderBidPrice = alignedTheoreticalBidOrderPrice;

		// Orders must not cross the exchange best bid/offer at the time they're sent
		if (orderBidPrice >= m_offerPrice) {
			orderBidPrice = m_offerPrice - m_tickWidth;
		}

		// Orders must not be closer to the theoretical price than the offset at the time they're sent
		if (m_theoreticalPrice - orderBidPrice >= m_quoteOffset) {
			QuantitativeModel::QuantOrderParammeter buyOrder;
			buyOrder.m_symbol = m_feedcode;
			buyOrder.m_side = binapi::e_side::buy;
			buyOrder.m_type = binapi::e_type::limit;
			buyOrder.m_time = binapi::e_time::IOC;
			buyOrder.m_price = orderBidPrice;
			buyOrder.m_amount = quoteVolume;
			DumpOrderInfo(buyOrder, "Quoting");
			// SEND ORDER TO EXCHANGE
			// Store the order in the quote pool until confirmed added
			m_quoteOrderPool.emplace(1, buyOrder);
			m_orderBidRemoved = false;
		}
	}

	// Ask orders must not be closer to the theoretical price than the offset at the time they're sent
	const double theoreticalAskQuotePrice = m_theoreticalPrice + m_quoteOffset;

	// Previous offer order must be confirmed removed before a new add request is sent on the offer side
	if (m_orderOfferRemoved) {
		const double alignedTheoreticalOfferOrderPrice = Utils::AlignOfferPrice(theoreticalAskQuotePrice, m_tickWidth);
		double orderOfferPrice = alignedTheoreticalOfferOrderPrice;

		// Orders must not cross the exchange best bid/offer at the time they're sent
		if (orderOfferPrice <= m_bidPrice) {
			orderOfferPrice = m_bidPrice + m_tickWidth;
		}

		// Orders must not be closer to the theoretical price than the offset at the time they're sent
		if (orderOfferPrice - m_theoreticalPrice >= m_quoteOffset) {
			QuantitativeModel::QuantOrderParammeter sellOrder;
			sellOrder.m_symbol = m_feedcode;
			sellOrder.m_side = binapi::e_side::sell;
			sellOrder.m_type = binapi::e_type::limit;
			sellOrder.m_time = binapi::e_time::IOC;
			sellOrder.m_price = orderOfferPrice;
			sellOrder.m_amount = quoteVolume;
			DumpOrderInfo(sellOrder, "Quoting");
			// SEND ORDER TO EXCHANGE
			// Store the order in the quote pool until confirmed added
			m_quoteOrderPool.emplace(2, sellOrder);
			m_orderOfferRemoved = false;
		}
	}
}

void InstrumentQuoter::OnOrderAddConfirm(uint32_t id)
{
	const QuantitativeModel::QuantOrderParammeter& quoteOrder = LookupQuoteAndHedgeOrder(id);
	m_quotedOrderPool.emplace(id, quoteOrder);
	DumpOrderInfo(quoteOrder, "Added");
}

void InstrumentQuoter::OnOrderRemoveConfirm(uint32_t id)
{
	const QuantitativeModel::QuantOrderParammeter quotedOrder = LookupQuoteAndHedgeOrder(id);
	if (quotedOrder.m_side == binapi::e_side::buy)
	{
		m_orderBidRemoved = true;
	}
	else if (quotedOrder.m_side == binapi::e_side::sell)
	{
		m_orderOfferRemoved = true;
	}
	else {
		throw std::runtime_error("Error: invalid order side");
	}
	DumpOrderInfo(quotedOrder, "Removed");
}

const QuantitativeModel::QuantOrderParammeter& InstrumentQuoter::LookupQuoteAndHedgeOrder(uint32_t id) const
{
	const auto it = m_quoteOrderPool.find(id);
	if (it == m_quoteOrderPool.end()) {
		throw std::runtime_error("Error: order ID not found");
	}
	return it->second;
}

const QuantitativeModel::QuantOrderParammeter& InstrumentQuoter::LookupQuotedAndHedgedOrder(uint32_t id) const
{
	const auto it = m_quotedOrderPool.find(id);
	if (it == m_quotedOrderPool.end()) {
		throw std::runtime_error("Error: order ID not found");
	}
	return it->second;
}

void InstrumentQuoter::DumpOrderInfo(const QuantitativeModel::QuantOrderParammeter& order, const std::string& action) const
{

}
