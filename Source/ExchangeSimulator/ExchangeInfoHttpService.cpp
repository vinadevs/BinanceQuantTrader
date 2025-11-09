/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"

#include "ExchangeInfoManager.h"
#include "ExchangeInfoHttpService.h"
#include "ExchangeRuleAndCompliance.h"

#include "../LibraryUtils/Logger.h"

using namespace ExchangeSimulator;

ExchangeInfoHttpService::ExchangeInfoHttpService(ExchangeInfoManager* exchangeInfoManager)
    : m_logger{ std::make_unique<LibraryUtils::Logger>("ExchangeInfoHttpService") },
      m_exchangeInfoManager(exchangeInfoManager) {}

ExchangeInfoHttpService::~ExchangeInfoHttpService() {}

static exchange::ExchangeInfoResponse ConvertToExchangeInfoResponse(
    const binapi::rest::exchange_info_t& src) {

    exchange::ExchangeInfoResponse response;
    exchange::ExchangeInfo* dst = response.mutable_exchange_info();

    dst->set_timezone(src.timezone);
    dst->set_servertime(src.serverTime);

    for (const auto& filter : src.exchangeFilters) {
        dst->add_exchangefilters(filter);
    }

    for (const auto& rateLimit : src.rateLimits) {
        exchange::RateLimit* rl = dst->add_ratelimits();
        rl->set_ratelimittype(rateLimit.rateLimitType);
        rl->set_interval(rateLimit.interval);
        rl->set_limit(rateLimit.limit);
    }

    for (const auto& symbol_pair : src.symbols) {
        const auto& symbol = symbol_pair.second;
        exchange::Symbol& sym = (*dst->mutable_symbols())[symbol.symbol];

        sym.set_symbol(symbol.symbol);
        sym.set_status(symbol.status);
        sym.set_baseasset(symbol.baseAsset);
        sym.set_baseassetprecision(symbol.baseAssetPrecision);
        sym.set_quoteasset(symbol.quoteAsset);
        sym.set_quoteprecision(symbol.quotePrecision);

        for (const auto& orderType : symbol.orderTypes) {
            sym.add_ordertypes(orderType);
        }

        sym.set_icebergallowed(symbol.icebergAllowed);
        sym.set_ocoallowed(symbol.ocoAllowed);
        sym.set_quoteorderqtymarketallowed(symbol.quoteOrderQtyMarketAllowed);
        sym.set_allowtrailingstop(symbol.allowTrailingStop);
        sym.set_cancelreplaceallowed(symbol.cancelReplaceAllowed);

        for (const auto& filter : symbol.filters) {
            exchange::Filter* f = sym.add_filters();

            if (auto p = boost::get<binapi::rest::exchange_info_t::symbol_t::filter_t::price_t>(&filter.filter)) {
                auto* pf = f->mutable_price();
                pf->set_minprice(p->minPrice.convert_to<double>());
                pf->set_maxprice(p->maxPrice.convert_to<double>());
                pf->set_ticksize(p->tickSize.convert_to<double>());
            }
            else if (auto pp = boost::get<binapi::rest::exchange_info_t::symbol_t::filter_t::percent_price_t>(&filter.filter)) {
                auto* pp_f = f->mutable_percentprice();
                pp_f->set_multiplierup(pp->multiplierUp.convert_to<double>());
                pp_f->set_multiplierdown(pp->multiplierDown.convert_to<double>());
                pp_f->set_avgpricemins(pp->avgPriceMins);
            }
            else if (auto pps = boost::get<binapi::rest::exchange_info_t::symbol_t::filter_t::percent_price_by_side_t>(&filter.filter)) {
                auto* pps_f = f->mutable_percentpricebyside();
                pps_f->set_bidmultiplierup(pps->bidMultiplierUp.convert_to<double>());
                pps_f->set_bidmultiplierdown(pps->bidMultiplierDown.convert_to<double>());
                pps_f->set_askmultiplierup(pps->askMultiplierUp.convert_to<double>());
                pps_f->set_askmultiplierdown(pps->askMultiplierDown.convert_to<double>());
                pps_f->set_avgpricemins(pps->avgPriceMins);
            }
            else if (auto lot = boost::get<binapi::rest::exchange_info_t::symbol_t::filter_t::lot_size_t>(&filter.filter)) {
                auto* lot_f = f->mutable_lotsize();
                lot_f->set_minqty(lot->minQty.convert_to<double>());
                lot_f->set_maxqty(lot->maxQty.convert_to<double>());
                lot_f->set_stepsize(lot->stepSize.convert_to<double>());
            }
            else if (auto ml = boost::get<binapi::rest::exchange_info_t::symbol_t::filter_t::market_lot_size_t>(&filter.filter)) {
                auto* ml_f = f->mutable_marketlotsize();
                ml_f->set_minqty(ml->minQty.convert_to<double>());
                ml_f->set_maxqty(ml->maxQty.convert_to<double>());
                ml_f->set_stepsize(ml->stepSize.convert_to<double>());
            }
            else if (auto mn = boost::get<binapi::rest::exchange_info_t::symbol_t::filter_t::min_notional_t>(&filter.filter)) {
                auto* mn_f = f->mutable_minnotional();
                mn_f->set_minnotional(mn->minNotional.convert_to<double>());
            }
            else if (auto ip = boost::get<binapi::rest::exchange_info_t::symbol_t::filter_t::iceberg_parts_t>(&filter.filter)) {
                auto* ip_f = f->mutable_icebergparts();
                ip_f->set_limit(ip->limit);
            }
            else if (auto mno = boost::get<binapi::rest::exchange_info_t::symbol_t::filter_t::max_num_orders_t>(&filter.filter)) {
                auto* mno_f = f->mutable_maxnumorders();
                mno_f->set_maxnumorders(mno->maxNumOrders);
            }
            else if (auto mna = boost::get<binapi::rest::exchange_info_t::symbol_t::filter_t::max_num_algo_orders_t>(&filter.filter)) {
                auto* mna_f = f->mutable_maxnumalgoorders();
                mna_f->set_maxnumalgoorders(mna->maxNumAlgoOrders);
            }
            else if (auto mp = boost::get<binapi::rest::exchange_info_t::symbol_t::filter_t::max_position_t>(&filter.filter)) {
                auto* mp_f = f->mutable_maxposition();
                mp_f->set_maxposition(mp->maxPosition.convert_to<double>());
            }
            else if (auto td = boost::get<binapi::rest::exchange_info_t::symbol_t::filter_t::trailing_delta_t>(&filter.filter)) {
                auto* td_f = f->mutable_trailingdelta();
                td_f->set_mintrailingabovedelta(td->minTrailingAboveDelta);
                td_f->set_maxtrailingabovedelta(td->maxTrailingAboveDelta);
                td_f->set_mintrailingbelowdelta(td->minTrailingBelowDelta);
                td_f->set_maxtrailingbelowdelta(td->maxTrailingBelowDelta);
            }
            else if (auto n = boost::get<binapi::rest::exchange_info_t::symbol_t::filter_t::notional_t>(&filter.filter)) {
                auto* n_f = f->mutable_notional();
                n_f->set_minnotional(n->minNotional.convert_to<double>());
                n_f->set_applymintomarket(n->applyMinToMarket);
                n_f->set_maxnotional(n->maxNotional.convert_to<double>());
                n_f->set_applymaxtomarket(n->applyMaxToMarket);
                n_f->set_avgpricemins(n->avgPriceMins);
            }
        }
    }

    dst->set_permissions(src.permissions);
    return response;
}

grpc::Status ExchangeInfoHttpService::GetExchangeInfo(
    grpc::ServerContext* context,
    const exchange::ExchangeInfoRequest* request,
    exchange::ExchangeInfoResponse* response)
{
    m_logger->Info("Received binapi::rest::exchange_info_t request for symbol=" + request->symbol_id());

	const auto& exchangeInfo = m_exchangeInfoManager->GetExchangeInfo();

    // Populate response
    *response = ConvertToExchangeInfoResponse(exchangeInfo);

    m_logger->Info("Sending binapi::rest::exchange_info_t response for symbol=" + request->symbol_id());

    return grpc::Status::OK;
}
