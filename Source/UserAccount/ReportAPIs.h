/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#ifndef __binapi__reports_hpp
#define __binapi__reports_hpp

#include "dlldefine.h"

#include <iosfwd>
#include <string>
#include <vector>
#include <functional>

namespace binapi {
namespace rest {

struct api;
struct account_info_t;
struct exchange_info_t;
struct order_info_t;
struct orders_info_t;

} // ns rest

/*************************************************************************************************/

void DLL_CLASS_USERACCOUNT_EXPORTS make_balance_report(
     std::ostream &os
    ,rest::api &api
    ,const rest::account_info_t &accinfo
    ,const rest::exchange_info_t &exinfo
);

rest::orders_info_t DLL_CLASS_USERACCOUNT_EXPORTS get_open_orders(
     rest::api &api
    ,const rest::exchange_info_t &exinfo
    ,const std::vector<std::string> &pairs
    ,const std::function<void(const std::string &)> &tick = {}
    ,const char *side = nullptr // "BUY"/"SELL"
    ,const std::uint64_t start_time = 0
    ,const std::string &start_time_str = ""
);

void DLL_CLASS_USERACCOUNT_EXPORTS make_open_orders_report(
     std::ostream &os
    ,rest::api &api
    ,const rest::exchange_info_t &exinfo
    ,const std::vector<std::string> &pairs
    ,const std::function<void(const std::string &)> &tick = {}
    ,const std::uint64_t start_time = 0
    ,const std::string &start_time_str = ""
);

void DLL_CLASS_USERACCOUNT_EXPORTS make_trades_report(
     std::ostream &os
    ,rest::api &api
    ,const rest::account_info_t &accinfo
    ,const rest::exchange_info_t &exinfo
    ,const std::vector<std::string> &pairs
    ,const std::function<void(const rest::order_info_t &)> &tick = {}
    ,const std::uint64_t start_time = 0
    ,const std::string &start_time_str = ""
);

void DLL_CLASS_USERACCOUNT_EXPORTS make_trades_report_for_last_day(
     std::ostream &os
    ,rest::api &api
    ,const rest::account_info_t &accinfo
    ,const rest::exchange_info_t &exinfo
    ,const std::vector<std::string> &pairs
    ,const std::function<void(const rest::order_info_t &)> &tick = {}
);

void DLL_CLASS_USERACCOUNT_EXPORTS show_exchanger_price_for_orders(
     std::ostream &os
    ,rest::api &api
    ,const rest::exchange_info_t &exinfo
    ,const std::vector<std::string> &pairs
    ,const std::function<void(const std::string &)> &tick = {}
);

void DLL_CLASS_USERACCOUNT_EXPORTS calc_loss_for_orders(
     std::ostream &os
    ,rest::api &api
    ,const rest::exchange_info_t &exinfo
    ,const std::vector<std::string> &pairs
    ,const std::function<void(const std::string &)> &tick = {}
);

/*************************************************************************************************/

} // ns binapi

#endif // __binapi__reports_hpp
