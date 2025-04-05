/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include "../KernelTrading/types.h"
#include "../SettingNConfig/tinyxml2.h"
#include "../LibraryUtils/Logger.h"

#include "BinanceExchangeClient.h"

using namespace ExchangeConnectivity;

BinanceExchangeClient::BinanceExchangeClient(const tinyxml2::XMLElement* BinanceExchangeClientXmlCfg)
    : m_logger{ std::make_unique<LibraryUtils::Logger>("BinanceExchangeClient") }
{
    assert(BinanceExchangeClientXmlCfg);
    const auto* connectionXml = BinanceExchangeClientXmlCfg->FirstChildElement("Connection");
    assert(connectionXml);
    m_logger->Info("Creating new Http connection...");
    m_grpcConnection.m_serverIpAddress = std::string(connectionXml->Attribute("ServerIpAddress"));
    m_grpcConnection.m_serverPort = std::string(connectionXml->Attribute("ServerPort"));
    m_grpcConnection.m_serverConnection = m_grpcConnection.m_serverIpAddress + ":" + m_grpcConnection.m_serverPort;
    m_grpcConnection.m_grpcChannel = grpc::CreateChannel(m_grpcConnection.m_serverConnection, grpc::InsecureChannelCredentials());
    m_grpcConnection.m_grpcStub = exchange::ExchangeService::NewStub(m_grpcConnection.m_grpcChannel);
}

BinanceExchangeClient::~BinanceExchangeClient() {}

static binapi::rest::exchange_info_t ConvertFromProto(
    const exchange::ExchangeInfoResponse& protoResponse) {

    binapi::rest::exchange_info_t info;

    // Copy simple fields
    const auto& protoInfo = protoResponse.exchange_info();
    info.timezone = protoInfo.timezone();
    info.serverTime = protoInfo.servertime();
    info.permissions = protoInfo.permissions();

    // Copy exchange filters
    for (const auto& filter : protoInfo.exchangefilters()) {
        info.exchangeFilters.push_back(filter);
    }

    // Copy rate limits
    for (const auto& protoRateLimit : protoInfo.ratelimits()) {
        binapi::rest::exchange_info_t::rate_limit_t rateLimit;
        rateLimit.rateLimitType = protoRateLimit.ratelimittype();
        rateLimit.interval = protoRateLimit.interval();
        rateLimit.limit = protoRateLimit.limit();
        info.rateLimits.push_back(rateLimit);
    }

    // Copy symbols
    for (const auto& pair : protoInfo.symbols()) {
        binapi::rest::exchange_info_t::symbol_t symbol;
        symbol.symbol = pair.second.symbol();
        symbol.status = pair.second.status();
        symbol.baseAsset = pair.second.baseasset();
        symbol.baseAssetPrecision = pair.second.baseassetprecision();
        symbol.quoteAsset = pair.second.quoteasset();
        symbol.quotePrecision = pair.second.quoteprecision();
        symbol.icebergAllowed = pair.second.icebergallowed();
        symbol.ocoAllowed = pair.second.ocoallowed();
        symbol.quoteOrderQtyMarketAllowed = pair.second.quoteorderqtymarketallowed();
        symbol.allowTrailingStop = pair.second.allowtrailingstop();
        symbol.cancelReplaceAllowed = pair.second.cancelreplaceallowed();

        for (const auto& orderType : pair.second.ordertypes()) {
            symbol.orderTypes.push_back(orderType);
        }

        // Convert filters
        for (const auto& filterProto : pair.second.filters()) {
            binapi::rest::exchange_info_t::symbol_t::filter_t filter;

            if (filterProto.has_price()) {
                filter.filterType = "PRICE_FILTER";
                filter.filter = binapi::rest::exchange_info_t::symbol_t::filter_t::price_t{
                    filterProto.price().minprice(),
                    filterProto.price().maxprice(),
                    filterProto.price().ticksize()
                };
            }
            else if (filterProto.has_percentprice()) {
                filter.filterType = "PERCENT_PRICE_FILTER";
                filter.filter = binapi::rest::exchange_info_t::symbol_t::filter_t::percent_price_t{
                    filterProto.percentprice().multiplierup(),
                    filterProto.percentprice().multiplierdown(),
                    filterProto.percentprice().avgpricemins()
                };
            }
            else if (filterProto.has_percentpricebyside()) {
                filter.filterType = "PERCENT_PRICE_BY_SIDE_FILTER";
                filter.filter = binapi::rest::exchange_info_t::symbol_t::filter_t::percent_price_by_side_t{
                    filterProto.percentpricebyside().bidmultiplierup(),
                    filterProto.percentpricebyside().bidmultiplierdown(),
                    filterProto.percentpricebyside().askmultiplierup(),
                    filterProto.percentpricebyside().askmultiplierdown(),
                    filterProto.percentpricebyside().avgpricemins()
                };
            }
            else if (filterProto.has_lotsize()) {
                filter.filterType = "LOT_SIZE_FILTER";
                filter.filter = binapi::rest::exchange_info_t::symbol_t::filter_t::lot_size_t{
                    filterProto.lotsize().minqty(),
                    filterProto.lotsize().maxqty(),
                    filterProto.lotsize().stepsize()
                };
            }
            else if (filterProto.has_marketlotsize()) {
                filter.filterType = "MARKET_LOT_SIZE_FILTER";
                filter.filter = binapi::rest::exchange_info_t::symbol_t::filter_t::market_lot_size_t{
                    filterProto.marketlotsize().minqty(),
                    filterProto.marketlotsize().maxqty(),
                    filterProto.marketlotsize().stepsize()
                };
            }
            else if (filterProto.has_minnotional()) {
                filter.filterType = "MIN_NOTIONAL_FILTER";
                filter.filter = binapi::rest::exchange_info_t::symbol_t::filter_t::min_notional_t{
                    filterProto.minnotional().minnotional()
                };
            }
            else if (filterProto.has_icebergparts()) {
                filter.filterType = "ICEBERG_PARTS_FILTER";
                filter.filter = binapi::rest::exchange_info_t::symbol_t::filter_t::iceberg_parts_t{
                    filterProto.icebergparts().limit()
                };
            }
            else if (filterProto.has_maxnumorders()) {
                filter.filterType = "MAX_NUM_ORDERS_FILTER";
                filter.filter = binapi::rest::exchange_info_t::symbol_t::filter_t::max_num_orders_t{
                    filterProto.maxnumorders().maxnumorders()
                };
            }
            else if (filterProto.has_maxnumalgoorders()) {
                filter.filterType = "MAX_NUM_ALGO_ORDERS_FILTER";
                filter.filter = binapi::rest::exchange_info_t::symbol_t::filter_t::max_num_algo_orders_t{
                    filterProto.maxnumalgoorders().maxnumalgoorders()
                };
            }
            else if (filterProto.has_maxposition()) {
                filter.filterType = "MAX_POSITION_FILTER";
                filter.filter = binapi::rest::exchange_info_t::symbol_t::filter_t::max_position_t{
                    filterProto.maxposition().maxposition()
                };
            }
            else if (filterProto.has_trailingdelta()) {
                filter.filterType = "TRAILING_DELTA_FILTER";
                filter.filter = binapi::rest::exchange_info_t::symbol_t::filter_t::trailing_delta_t{
                    filterProto.trailingdelta().mintrailingabovedelta(),
                    filterProto.trailingdelta().maxtrailingabovedelta(),
                    filterProto.trailingdelta().mintrailingbelowdelta(),
                    filterProto.trailingdelta().maxtrailingbelowdelta()
                };
            }
            else if (filterProto.has_notional()) {
                filter.filterType = "NOTIONAL_FILTER";
                filter.filter = binapi::rest::exchange_info_t::symbol_t::filter_t::notional_t{
                    filterProto.notional().minnotional(),
                    filterProto.notional().applymintomarket(),
                    filterProto.notional().maxnotional(),
                    filterProto.notional().applymaxtomarket(),
                    filterProto.notional().avgpricemins()
                };
            }

            symbol.filters.push_back(filter);
        }

        info.symbols[pair.first] = symbol;
    }

    return info;
}

static binapi::rest::exchange_info_t GetExchangeInfoBySymbol(
    const std::string& symbol, const binapi::rest::exchange_info_t& exchangeInfo) {
    binapi::rest::exchange_info_t singleSymbolInfo;
    singleSymbolInfo.timezone = exchangeInfo.timezone;
    singleSymbolInfo.serverTime = exchangeInfo.serverTime;
    singleSymbolInfo.exchangeFilters = exchangeInfo.exchangeFilters;
    singleSymbolInfo.rateLimits = exchangeInfo.rateLimits;
    singleSymbolInfo.permissions = exchangeInfo.permissions;

    const auto it = exchangeInfo.symbols.find(symbol);
    if (it != exchangeInfo.symbols.end()) {
        singleSymbolInfo.symbols[symbol] = it->second;
    }

    return singleSymbolInfo;
}

bool BinanceExchangeClient::GetExchangeInfoResponse(
    const std::string& symbol,
    binapi::rest::exchange_info_t* exchangeInfo,
    std::string& errorMessage)
{
    m_logger->Info("Sending request binapi::rest::binapi::rest::exchange_info_t data for symbol=" + symbol);

    exchange::ExchangeInfoRequest request;
    request.set_symbol_id(symbol);

    exchange::ExchangeInfoResponse response;
    grpc::ClientContext context;

    const grpc::Status status = m_grpcConnection.m_grpcStub->GetExchangeInfo(&context, request, &response);

    if (status.ok())
    {
        if (symbol.empty())
        {
			*exchangeInfo = ConvertFromProto(response);
        }
        else 
        {
			*exchangeInfo = GetExchangeInfoBySymbol(symbol, ConvertFromProto(response));
        }
        return true;
    }
    else
    {
        errorMessage = status.error_message();
        return false;
    }
}
