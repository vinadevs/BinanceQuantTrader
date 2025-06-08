// BinanceFutureApiGateway.h
#pragma once

#include "dlldefine.h"

#include "BinanceMarginRateInfo.h"

#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

namespace CurlAPI {

// BinanceFutureApiGateway is a singleton class that provides an interface to interact with the Binance Futures API.
// It supports sending test and real orders, setting leverage, canceling and querying orders, and handles
// authentication, request signing, and response parsing using cURL and nlohmann::json.
// The class is designed to be thread-safe and ensures that only one instance exists throughout the application.

class DLL_CLASS_CURLAPI_EXPORTS BinanceFutureApiGateway final {
public:
    static BinanceFutureApiGateway& GetInstance();

    void InitiateAPI(const std::string& futureApiBinanceUrl, const std::string& apiKey, const std::string& secretKey);

    void SendTestOrder(const std::string& symbol,
        const std::string& side = "BUY",
        const std::string& type = "MARKET",
        const std::string& quantity = "0.001",
        const std::string& origClientOrderId = "");

    void SendRealOrder(const std::string& symbol,
        const std::string& side = "BUY",
        const std::string& type = "MARKET",
        const std::string& quantity = "0.001",
        const std::string& origClientOrderId = "");

    void SetLeverageRate(const std::string& symbol, int leverage);
    void CancelOrder(const std::string& symbol, const std::string& origClientOrderId);
    void QueryOrder(const std::string& symbol, const std::string& origClientOrderId);
    SymbolMarginRateInfo GetLeverageBracket(const std::string& symbol);

private:
    BinanceFutureApiGateway() = default;
    ~BinanceFutureApiGateway() = default;
    BinanceFutureApiGateway(const BinanceFutureApiGateway&) = delete;
    BinanceFutureApiGateway& operator=(const BinanceFutureApiGateway&) = delete;

    void SendOrder(const std::string& endpoint,
        const std::string& symbol,
        const std::string& side,
        const std::string& type,
        const std::string& quantity,
        const std::string& origClientOrderId) const;

    std::string SendSignedPostRequest(const std::string& endpoint, const std::string& query) const;
    std::string SendSignedGetRequest(const std::string& endpoint, const std::string& query) const;
    std::string SendSignedDeleteRequest(const std::string& endpoint, const std::string& query) const;

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    void PrintJsonResponse(const std::string& response) const;
    std::string GetTimestamp() const;
    std::string HmacSha256(const std::string& key, const std::string& data) const;

    std::string m_apiKey;
    std::string m_secretKey;
    std::string m_baseUrl;
};
} // namespace CurlAPI
// Lets shorten the code line!
#define BinanceFutureApiGatewayMgr CurlAPI::BinanceFutureApiGateway::GetInstance()