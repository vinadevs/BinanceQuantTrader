#ifndef INSTRUMENTQUOTER_H
#define INSTRUMENTQUOTER_H

#include <string>
#include <cstdint>
#include <map>
#include <unordered_map>

namespace TradingStrategies {

struct Order final {
	uint32_t  m_id{ 0 };
	char  m_side; // 'B' for buy, 'S' for sell
	double  m_price{ 0.0 };
	uint32_t m_volume{ 0 };
	std::string m_feedcode;
	Order(uint32_t id, char side, double price, uint32_t volume, std::string const& feedcode)
		: m_id(id), m_side(side), m_price(price), m_volume(volume), m_feedcode(feedcode) {
	}
};

class InstrumentQuoter
{
private:
	std::string m_feedcode;
    double m_quoteOffset{ 0.0 };
	uint32_t m_quoteVolume{ 0 };
	double m_tickWidth{ 0.0 };
	double m_theoreticalPrice{ 0.0 };
	double m_bidPrice{ 0.0 };
	double m_offerPrice{ 0.0 };
	bool m_orderBidAdded{ false };
	bool m_orderOfferAdded{ false };
	bool m_orderBidRemoved{ true };
	bool m_orderOfferRemoved{ true };
	uint32_t m_orderIDCounter{ 0 };
	std::unordered_map<uint32_t, Order> m_quoteOrderPool; // tracks orders that have been sent but not yet confirmed
	std::unordered_map<uint32_t, Order> m_quotedOrderPool; // tracks orders that have been confirmed added

	const Order LookupQuoteOrder(uint32_t id) const;
    const Order LookupQuotedOrder(uint32_t id) const;
	uint32_t GetNewOrderId();
public:
	InstrumentQuoter( // Instantiates the quoter for a single instrument
		std::string const& feedcode, // The exchange identifier of the instrument
		double quoteOffset, // The minimum desired distance between our theoretical price and each order we send
		uint32_t quoteVolume, // The desired volume for each order we send
		double tickWidth); // The distance between valid exchange price levels
    void OnTheoreticalPrice( // Notifies the quoter of a new theoretical price for the instrument
        double theoreticalPrice); // The new theoretical price
    void OnBestBidOffer( // Notifies the quoter of a new best bid/offer from the exchange
        double bidPrice, // The best price bid on the exchange
        double offerPrice); // The best price offer on the exchange
    void OnOrderAddConfirm( // Notifies the quoter that an "order add" request has been confirmed
        uint32_t id); // The request identifier of the order
    void OnOrderRemoveConfirm( // Notifies the quoter that an "order remove" request has been confirmed
        uint32_t id); // The request identifier of the order
};

}; // namespace TradingStrategies

#endif // INSTRUMENTQUOTER_H