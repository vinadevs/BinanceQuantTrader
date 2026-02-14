/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#ifndef _REAL_TIME_SOCKET_H_
#define _REAL_TIME_SOCKET_H_

#pragma warning(disable : 4251)

#include "dlldefine.h"

#include "../KernelTrading/enums.h"

#include <memory>
#include <functional>
#include <string>

namespace boost {
namespace asio {
class io_context;
} // ns asio
} // ns boost

namespace binapi {
namespace userdata {

struct account_update_t;
struct balance_update_t;
struct order_update_t;

} // ns userdata

namespace ws {

struct part_depths_t;
struct diff_depths_t;
struct trade_t;
struct agg_trade_t;
struct kline_t;
struct mini_ticker_t;
struct mini_tickers_t;
struct market_ticker_t;
struct markets_tickers_t;
struct book_ticker_t;
struct future_trade_t;
struct future_book_t;
struct future_kline_t;
struct future_ticker_t;
struct future_mark_price_t;
struct future_funding_rate_t;
struct future_liquidation_t;

/*************************************************************************************************/

struct DLL_CLASS_REALTIMESOCKET_EXPORTS websockets final {
    websockets(const websockets &) = delete;
    websockets& operator= (const websockets &) = delete;
    websockets(websockets &&) noexcept = default;
    websockets& operator= (websockets &&) noexcept = default;

    using on_message_received_cb = std::function<void(const char *channel, const char *ptr, std::size_t size)>;
    using on_network_stat_cb = std::function<
        void(std::size_t msg_recvd, std::size_t msg_recvd_avg, std::size_t bytes_recvd, std::size_t bytes_recvd_avg)
    >;

    websockets(
         boost::asio::io_context &ioctx
        ,std::string spotHost
        ,std::string spotPort
		,std::string futureHost
		,std::string futurePort
        ,on_message_received_cb msg_cb = {}
        ,on_network_stat_cb stat_cb = {}
        ,std::size_t stat_interval = 1 // in seconds
    );
    ~websockets();

    using handle = void*;

    // https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#partial-book-depth-streams
    using on_part_depths_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, part_depths_t msg)>;
    handle part_depth(const char *pair, e_levels level, e_freq freq, on_part_depths_received_cb cb);

    // https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#diff-depth-stream
    using on_diff_depths_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, diff_depths_t msg)>;
    handle diff_depth(const char *pair, e_freq freq, on_diff_depths_received_cb cb);

    // https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#klinecandlestick-streams
    using on_kline_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, kline_t msg)>;
    // interval - 1s, 1m, 3m, 5m, 15m, 30m, 1h, 2h, 4h, 6h, 8h, 12h, 1d, 3d, 1w, 1M
    handle klines(const char *pair, const char *interval, on_kline_received_cb cb);

    // https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#trade-streams
    using on_trade_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, trade_t msg)>;
    handle trade(const char *pair, on_trade_received_cb cb);

    // https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#aggregate-trade-streams
    using on_agg_trade_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, agg_trade_t msg)>;
    handle agg_trade(const char *pair, on_agg_trade_received_cb cb);

    // https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#individual-symbol-mini-ticker-stream
    using on_mini_ticker_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, mini_ticker_t msg)>;
    handle mini_ticker(const char *pair, on_mini_ticker_received_cb cb);

    // https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#all-market-mini-tickers-stream
    using on_mini_tickers_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, mini_tickers_t msg)>;
    handle mini_tickers(on_mini_tickers_received_cb cb);

    // https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#individual-symbol-ticker-streams
    using on_market_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, market_ticker_t msg)>;
    handle market(const char *pair, on_market_received_cb cb);

    // https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#all-market-tickers-stream
    using on_markets_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, markets_tickers_t msg)>;
    handle markets(on_markets_received_cb cb);

    // https://github.com/binance/binance-spot-api-docs/blob/master/web-socket-streams.md#individual-symbol-book-ticker-streams
    using on_book_received_cb = std::function<bool(const char *fl, int ec, std::string errmsg, book_ticker_t msg)>;
    handle book(const char *pair, on_book_received_cb cb);

    using on_account_update_cb = std::function<bool(const char *fl, int ec, std::string errmsg, userdata::account_update_t msg)>;
    using on_balance_update_cb = std::function<bool(const char *fl, int ec, std::string errmsg, userdata::balance_update_t msg)>;
    using on_order_update_cb = std::function<bool(const char *fl, int ec, std::string errmsg, userdata::order_update_t msg)>;
    handle userdata(
         const char *lkey // Subscribe via the WebSocket API using an API Key
        ,on_account_update_cb account_update
        ,on_balance_update_cb balance_update
        ,on_order_update_cb order_update
    );

    // BEGIN FUTURES MARKET DATA STREAMS ////////////////////////////////////////////////////////////////////
    
    // https://developers.binance.com/docs/derivatives/usds-margined-futures/websocket-market-streams#aggregate-trade-streams
    using on_trade_received_cb_future = std::function<bool(const char* fl, int ec, std::string errmsg, future_trade_t msg)>;
    handle trade_future(const char* pair, on_trade_received_cb_future cb);

    // https://developers.binance.com/docs/derivatives/usds-margined-futures/websocket-market-streams#partial-book-depth-streams
    using on_book_received_cb_future = std::function<bool(const char* fl, int ec, std::string errmsg, future_book_t msg)>;
    handle book_future(const char* pair, on_book_received_cb_future cb);

    // https://developers.binance.com/docs/derivatives/usds-margined-futures/websocket-market-streams#klinecandlestick-streams
    using on_kline_received_cb_future = std::function<bool(const char* fl, int ec, std::string errmsg, future_kline_t msg)>;
    handle kline_future(const char* pair, const char* interval, on_kline_received_cb_future cb);

    // https://developers.binance.com/docs/derivatives/usds-margined-futures/websocket-market-streams#individual-symbol-ticker-streams
    using on_ticker_received_cb_future = std::function<bool(const char* fl, int ec, std::string errmsg, future_ticker_t msg)>;
    handle ticker_future(const char* pair, on_ticker_received_cb_future cb);

    // https://developers.binance.com/docs/derivatives/usds-margined-futures/websocket-market-streams#mark-price-stream
    using on_mark_price_received_cb_future = std::function<bool(const char* fl, int ec, std::string errmsg, future_mark_price_t msg)>;
    handle mark_price_future(const char* pair, on_mark_price_received_cb_future cb);

    // https://developers.binance.com/docs/derivatives/usds-margined-futures/websocket-market-streams#funding-rate-stream
    using on_funding_received_cb_future = std::function<bool(const char* fl, int ec, std::string errmsg, future_funding_rate_t msg)>;
    handle funding_rate_future(const char* pair, on_funding_received_cb_future cb);

    // https://developers.binance.com/docs/derivatives/usds-margined-futures/websocket-market-streams#liquidation-order-streams
    using on_liquidation_received_cb_future = std::function<bool(const char* fl, int ec, std::string errmsg, future_liquidation_t msg)>;
    handle liquidation_future(const char* pair, on_liquidation_received_cb_future cb);

    // END FUTURES MARKET DATA STREAMS ////////////////////////////////////////////////////////////////////

    void unsubscribe(const handle &h);
    void async_unsubscribe(const handle &h);
    void unsubscribe_all();
    void async_unsubscribe_all();

private:
    struct impl;
    std::unique_ptr<impl> m_spotPimpl;
	std::unique_ptr<impl> m_futurePimpl;
};

/*************************************************************************************************/

} // ns ws
} // ns binapi

#endif // _REAL_TIME_SOCKET_H_
