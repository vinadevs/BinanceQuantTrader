#include "pch.h"

#include "InstrumentQuoter.h"

#include <cmath>
#include <stdexcept>
#include <iostream>

using namespace TradingStrategies;

namespace Utils {
	double AlignBidPrice(double price, double tick) {
		return std::floor(price / tick) * tick;
	}

	double AlignOfferPrice(double price, double tick) {
		return std::ceil(price / tick) * tick;
	}

	void DumpOrderInfo(const Order& order, std::string const& status) {
		std::cout << "Order ID: " << order.m_id
			<< ", Side: " << order.m_side
			<< ", Price: " << order.m_price
			<< ", Volume: " << order.m_volume
			<< ", Feedcode: " << order.m_feedcode
			<< ", Status: " << status
			<< std::endl;
	}
}

constexpr char BUY_SIDE = 'B';
constexpr char SELL_SIDE = 'S';

InstrumentQuoter::InstrumentQuoter(
	std::string const& feedcode,
	double quoteOffset,
	uint32_t quoteVolume,
	double tickWidth)
	: m_feedcode(feedcode)
	, m_quoteOffset(quoteOffset)
	, m_quoteVolume(quoteVolume)
	, m_tickWidth(tickWidth)
{
	if (m_feedcode.empty()) {
		throw std::invalid_argument("Error: feedcode is empty");
	}
	if (m_quoteOffset <= 0.0) {
		throw std::invalid_argument("Error: quoteOffset is not positive");
	}
	if (m_quoteVolume <= 0) {
		throw std::invalid_argument("Error: quoteVolume is not positive");
	}
	if (m_tickWidth <= 0.0) {
		throw std::invalid_argument("Error: tickWidth is not positive");
	}
}

uint32_t InstrumentQuoter::GetNewOrderId()
{
	return ++m_orderIDCounter;
}

void InstrumentQuoter::OnTheoreticalPrice(double theoreticalPrice)
{
	if (theoreticalPrice <= 0.0) {
		throw std::invalid_argument("Error: theoretical price is invalid");
	}
	m_theoreticalPrice = theoreticalPrice;
}

void InstrumentQuoter::OnBestBidOffer(double bidPrice, double offerPrice)
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
			Order buyOrder(GetNewOrderId(), BUY_SIDE, orderBidPrice, m_quoteVolume, m_feedcode);
			Utils::DumpOrderInfo(buyOrder, "Quoting");
			// Store the order in the quote pool until confirmed added
			m_quoteOrderPool.emplace(buyOrder.m_id, buyOrder);
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
			Order sellOrder(GetNewOrderId(), SELL_SIDE, orderOfferPrice, m_quoteVolume, m_feedcode);
			Utils::DumpOrderInfo(sellOrder, "Quoting");
			// Store the order in the quote pool until confirmed added
			m_quoteOrderPool.emplace(sellOrder.m_id, sellOrder);
			m_orderOfferRemoved = false;
		}
	}
}

void InstrumentQuoter::OnOrderAddConfirm(uint32_t id)
{
	const Order quoteOrder = LookupQuoteOrder(id);
	m_quotedOrderPool.emplace(id, quoteOrder);
	Utils::DumpOrderInfo(quoteOrder, "Added");
}

void InstrumentQuoter::OnOrderRemoveConfirm(uint32_t id)
{
	const Order quotedOrder = LookupQuotedOrder(id);
	if (quotedOrder.m_side == BUY_SIDE) {
		m_orderBidRemoved = true;
	}
	else if (quotedOrder.m_side == SELL_SIDE) {
		m_orderOfferRemoved = true;
	}
	else {
		throw std::runtime_error("Error: invalid order side");
	}
	Utils::DumpOrderInfo(quotedOrder, "Removed");
}

const Order InstrumentQuoter::LookupQuoteOrder(uint32_t id) const
{
	const auto it = m_quoteOrderPool.find(id);
	if (it == m_quoteOrderPool.end()) {
		throw std::runtime_error("Error: order ID not found");
	}
	return it->second;
}

const Order InstrumentQuoter::LookupQuotedOrder(uint32_t id) const
{
	const auto it = m_quotedOrderPool.find(id);
	if (it == m_quotedOrderPool.end()) {
		throw std::runtime_error("Error: order ID not found");
	}
	return it->second;
}
