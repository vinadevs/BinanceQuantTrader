#pragma once

#include "dlldefine.h"

#include <iostream>
#include <string>
#include <type_traits>
#include <variant>
#include <array>

namespace MarketData {

    // new enum types
    enum class IndividualBookTickerID : unsigned
    {
        BEST_BID_PRICE,
        BEST_BID_QUANTITY,
        BEST_ASK_PRICE,
        BEST_ASK_QUANTITY
    };

    enum class TradeID : unsigned
    {
        ID,
        PRICE,
        QTY,
        TIME,
        IS_BUYER_MAKER,
        IS_BEST_MATCH
    };

    enum class IndividualMarketTickerID : unsigned
    {
        SYMBOL,
        PRICE_CHANGE,
        PRICE_CHANGE_PERCENT,
        WEIGHTED_AVERAGE_PRICE,
        FIRST_TRADE,
        LAST_PRICE,
        LAST_QUANTITY,
        BEST_BID_PRICE,
        BEST_BID_QUANTITY,
        BEST_ASK_PRICE,
        BEST_ASK_QUANTITY,
        OPEN_PRICE,
        HIGH_PRICE,
        LOW_PRICE,
        TOTAL_TRADED_BASE_ASSET_VOLUME,
        TOTAL_TRADED_QUOTE_ASSET_VOLUME,
        STATISTICS_OPEN_TIME,
        STATISTICS_CLOSE_TIME,
        FIRST_TRADE_ID,
        LAST_TRADE_ID,
        TOTAL_NUMBER_OF_TRADES
    };

    enum class AllMarketTickersID : unsigned
    {
        TICKERS
    };

    enum class IndividualMiniTickerID : unsigned
    {
        SYMBOL,
        CLOSE_PRICE,
        OPEN_PRICE,
        HIGH_PRICE,
        LOW_PRICE,
        TOTAL_TRADED_BASE_ASSET_VOLUME,
        TOTAL_TRADED_QUOTE_ASSET_VOLUME
    };

    enum class AllMiniTickersID : unsigned
    {
        TICKERS
    };

    enum class AggregateTradeID : unsigned
    {
        ID,
        PRICE,
        QTY,
        FIRST_ID,
        LAST_ID,
        TIME,
        IS_BUYER_MAKER,
        IS_BEST_MATCH
    };

    enum class KlineCandleStickID : unsigned
    {
        START_TIME,
        END_TIME,
        OPEN,
        HIGH,
        LOW,
        CLOSE,
        VOLUME,
        QUOTE_VOLUME,
        NUM_TRADES,
        TAKER_BUY_BASE_VOL,
        TAKER_BUY_QUOTE_VOL
    };

    enum class PartDepthID : unsigned
    {
        PRICE,
        AMOUNT
    };

    enum class DiffDepthID : unsigned
    {
        PRICE,
        AMOUNT
    };

    template <typename Enum>
    constexpr auto EnumIndex(Enum e) noexcept
    {
        return static_cast<std::underlying_type_t<Enum>>(e);
    }

    inline constexpr std::array<std::string_view, 4> IndividualBookTickerNames{
            "BEST_BID_PRICE",
            "BEST_BID_QTY",
            "BEST_ASK_PRICE",
            "BEST_ASK_QTY"
    };

    inline constexpr std::array<std::string_view, 6> TradeIDNames{
            "ID",
            "PRICE",
            "QTY",
            "TIME",
            "IS_BUYER_MAKER",
            "IS_BEST_MATCH"
    };

    inline constexpr std::array<std::string_view, 21> IndividualMarketTickerNames{
            "SYMBOL",
            "PRICE_CHANGE",
            "PRICE_CHANGE_PERCENT",
            "WEIGHTED_AVERAGE_PRICE",
            "FIRST_TRADE",
            "LAST_PRICE",
            "LAST_QUANTITY",
            "BEST_BID_PRICE",
            "BEST_BID_QUANTITY",
            "BEST_ASK_PRICE",
            "BEST_ASK_QUANTITY",
            "OPEN_PRICE",
            "HIGH_PRICE",
            "LOW_PRICE",
            "TOTAL_TRADED_BASE_ASSET_VOLUME",
            "TOTAL_TRADED_QUOTE_ASSET_VOLUME",
            "STATISTICS_OPEN_TIME",
            "STATISTICS_CLOSE_TIME",
            "FIRST_TRADE_ID",
            "LAST_TRADE_ID",
            "TOTAL_NUMBER_OF_TRADES"
    };

    inline constexpr std::array<std::string_view, 1> AllMarketTickersNames{
            "TICKERS"
    };

    inline constexpr std::array<std::string_view, 7> IndividualMiniTickerNames{
            "SYMBOL",
            "CLOSE_PRICE",
            "OPEN_PRICE",
            "HIGH_PRICE",
            "LOW_PRICE",
            "TOTAL_TRADED_BASE_ASSET_VOLUME",
            "TOTAL_TRADED_QUOTE_ASSET_VOLUME"
    };

    inline constexpr std::array<std::string_view, 1> AllMiniTickersNames{
            "TICKERS"
    };

    inline constexpr std::array<std::string_view, 8> AggregateTradeNames{
            "ID",
            "PRICE",
            "QTY",
            "FIRST_ID",
            "LAST_ID",
            "TIME",
            "IS_BUYER_MAKER",
            "IS_BEST_MATCH"
    };

    inline constexpr std::array<std::string_view, 11> KlineCandleStickNames{
            "START_TIME",
            "END_TIME",
            "OPEN",
            "HIGH",
            "LOW",
            "CLOSE",
            "VOLUME",
            "QUOTE_VOLUME",
            "NUM_TRADES",
            "TAKER_BUY_BASE_VOL",
            "TAKER_BUY_QUOTE_VOL"
    };

    inline constexpr std::array<std::string_view, 2> PartDepthNames{
            "PRICE",
            "AMOUNT"
    };

    inline constexpr std::array<std::string_view, 2> DiffDepthNames{
            "PRICE",
            "AMOUNT"
    };

    inline std::ostream& operator<<(std::ostream& os, const IndividualBookTickerID id)
    {
        if (auto idx = EnumIndex(id); idx < IndividualBookTickerNames.size())
            os << IndividualBookTickerNames[idx];
        else
            os << "UNKNOWN_ID";

        return os;
    }

    inline std::ostream& operator<<(std::ostream& os, const TradeID id)
    {
        if (auto idx = EnumIndex(id); idx < TradeIDNames.size())
            os << TradeIDNames[idx];
        else
            os << "UNKNOWN_ID";

        return os;
    }

    inline std::ostream& operator<<(std::ostream& os, const IndividualMarketTickerID id)
    {
        if (auto idx = EnumIndex(id); idx < IndividualMarketTickerNames.size())
            os << IndividualMarketTickerNames[idx];
        else
            os << "UNKNOWN_ID";

        return os;
    }

    inline std::ostream& operator<<(std::ostream& os, const AllMarketTickersID id)
    {
        if (auto idx = EnumIndex(id); idx < AllMarketTickersNames.size())
            os << AllMarketTickersNames[idx];
        else
            os << "UNKNOWN_ID";

        return os;
    }

    inline std::ostream& operator<<(std::ostream& os, const IndividualMiniTickerID id)
    {
        if (auto idx = EnumIndex(id); idx < IndividualMiniTickerNames.size())
            os << IndividualMiniTickerNames[idx];
        else
            os << "UNKNOWN_ID";

        return os;
    }

    inline std::ostream& operator<<(std::ostream& os, const AllMiniTickersID id)
    {
        if (auto idx = EnumIndex(id); idx < AllMiniTickersNames.size())
            os << AllMiniTickersNames[idx];
        else
            os << "UNKNOWN_ID";

        return os;
    }

    inline std::ostream& operator<<(std::ostream& os, const AggregateTradeID id)
    {
        if (auto idx = EnumIndex(id); idx < AggregateTradeNames.size())
            os << AggregateTradeNames[idx];
        else
            os << "UNKNOWN_ID";

        return os;
    }

    inline std::ostream& operator<<(std::ostream& os, const KlineCandleStickID id)
    {
        if (auto idx = EnumIndex(id); idx < KlineCandleStickNames.size())
            os << KlineCandleStickNames[idx];
        else
            os << "UNKNOWN_ID";

        return os;
    }

    inline std::ostream& operator<<(std::ostream& os, const PartDepthID id)
    {
        if (auto idx = EnumIndex(id); idx < PartDepthNames.size())
            os << PartDepthNames[idx];
        else
            os << "UNKNOWN_ID";

        return os;
    }

    inline std::ostream& operator<<(std::ostream& os, const DiffDepthID id)
    {
        if (auto idx = EnumIndex(id); idx < DiffDepthNames.size())
            os << DiffDepthNames[idx];
        else
            os << "UNKNOWN_ID";

        return os;
    }
};