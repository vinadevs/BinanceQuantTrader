// BinanceFutureApiGateway.h
#pragma once

#include <string>
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

class BinanceFutureApiGateway {
public:
    static BinanceFutureApiGateway& getInstance();

    void initUserKey(const std::string& apiKey, const std::string& secretKey);

    void sendTestOrder(const std::string& symbol,
        const std::string& side = "BUY",
        const std::string& type = "MARKET",
        const std::string& quantity = "0.001",
        const std::string& origClientOrderId = "");

    void sendRealOrder(const std::string& symbol,
        const std::string& side = "BUY",
        const std::string& type = "MARKET",
        const std::string& quantity = "0.001",
        const std::string& origClientOrderId = "");

    void setLeverageRate(const std::string& symbol, int leverage);
    void cancelOrder(const std::string& symbol, const std::string& origClientOrderId);
    void queryOrder(const std::string& symbol, const std::string& origClientOrderId);

private:
    BinanceFutureApiGateway() = default;
    ~BinanceFutureApiGateway() = default;
    BinanceFutureApiGateway(const BinanceFutureApiGateway&) = delete;
    BinanceFutureApiGateway& operator=(const BinanceFutureApiGateway&) = delete;

    void sendOrder(const std::string& endpoint,
        const std::string& symbol,
        const std::string& side,
        const std::string& type,
        const std::string& quantity,
        const std::string& origClientOrderId) const;

    std::string sendSignedPostRequest(const std::string& endpoint, const std::string& query) const;
    std::string sendSignedGetRequest(const std::string& endpoint, const std::string& query) const;
    std::string sendSignedDeleteRequest(const std::string& endpoint, const std::string& query) const;

    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
    void printJsonResponse(const std::string& response) const;
    std::string getTimestamp() const;
    std::string hmacSha256(const std::string& key, const std::string& data) const;

    std::string m_apiKey;
    std::string m_secretKey;
    std::string m_baseUrl = "https://fapi.binance.com";
};
