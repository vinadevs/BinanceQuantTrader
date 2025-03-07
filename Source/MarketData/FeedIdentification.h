/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include <string>

namespace MarketData {

    enum class FeedID : unsigned
    {
        BEST_BID_PRICE,
        BEST_BID_QUANTITY,
        BEST_ASK_PRICE,
        BEST_ASK_QUANTITY,
        EVENT_TIME,
        // Update more feeds here and FromFeedIDToString() below also...
    };

    /**
     * @brief Converts FeedID enum to a string representation.
     *
     * @param feed The FeedID enum value.
     * @return std::string The corresponding string representation.
     */
    inline std::string FromFeedIDToString(const FeedID feedID) {
        switch (feedID) {
        case FeedID::BEST_BID_PRICE:  return "BEST_BID_PRICE";
        case FeedID::BEST_BID_QUANTITY: return "BEST_BID_QUANTITY";
        case FeedID::BEST_ASK_PRICE:  return "BEST_ASK_PRICE";
        case FeedID::BEST_ASK_QUANTITY: return "BEST_ASK_QUANTITY";
        case FeedID::EVENT_TIME:      return "EVENT_TIME";
        // Update more feeds here...
        default: return "UNKNOWN_FEED_ID";
        }
    }
};