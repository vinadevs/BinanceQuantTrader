#include "pch.h"
#include "BinanceFutureApiGateway.h"

#include <chrono>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <openssl/hmac.h>
#include <openssl/evp.h>

using json = nlohmann::json;
using namespace CurlAPI;

BinanceFutureApiGateway& BinanceFutureApiGateway::GetInstance() {
    static BinanceFutureApiGateway instance;
    return instance;
}

void BinanceFutureApiGateway::InitiateAPI(const std::string& futureApiBinanceUrl, const std::string& apiKey, const std::string& secretKey) {
	m_baseUrl = "https://" + futureApiBinanceUrl;
    m_apiKey = apiKey;
    m_secretKey = secretKey;
    curl_global_init(CURL_GLOBAL_ALL);
}

void BinanceFutureApiGateway::SendTestOrder(const std::string& symbol,
    const std::string& side,
    const std::string& type,
    const std::string& quantity,
    const std::string& origClientOrderId) {
    std::string endpoint = "/fapi/v1/order/test";
    SendOrder(endpoint, symbol, side, type, quantity, origClientOrderId);
}

void BinanceFutureApiGateway::SendRealOrder(const std::string& symbol,
    const std::string& side,
    const std::string& type,
    const std::string& quantity,
    const std::string& origClientOrderId) {
    std::string endpoint = "/fapi/v1/order";
    SendOrder(endpoint, symbol, side, type, quantity, origClientOrderId);
}

void BinanceFutureApiGateway::SetLeverageRate(const std::string& symbol, int leverage) {
    std::string endpoint = "/fapi/v1/leverage";
    std::string timestamp = GetTimestamp();

    std::string query = "symbol=" + symbol +
        "&leverage=" + std::to_string(leverage) +
        "&timestamp=" + timestamp;

    std::string signature = HmacSha256(m_secretKey, query);
    query += "&signature=" + signature;

    std::string response = SendSignedPostRequest(endpoint, query);
    PrintJsonResponse(response);
}

void BinanceFutureApiGateway::CancelOrder(const std::string& symbol, const std::string& origClientOrderId) {
    std::string endpoint = "/fapi/v1/order";
    std::string timestamp = GetTimestamp();
    std::string query = "symbol=" + symbol +
        "&origClientOrderId=" + origClientOrderId +
        "&timestamp=" + timestamp;
    query += "&signature=" + HmacSha256(m_secretKey, query);
    std::string response = SendSignedDeleteRequest(endpoint, query);
    PrintJsonResponse(response);
}

void BinanceFutureApiGateway::QueryOrder(const std::string& symbol, const std::string& origClientOrderId) {
    std::string endpoint = "/fapi/v1/order";
    std::string timestamp = GetTimestamp();
    std::string query = "symbol=" + symbol +
        "&origClientOrderId=" + origClientOrderId +
        "&timestamp=" + timestamp;
    query += "&signature=" + HmacSha256(m_secretKey, query);
    std::string response = SendSignedGetRequest(endpoint, query);
    PrintJsonResponse(response);
}

SymbolMarginRateInfo BinanceFutureApiGateway::GetLeverageBracket(const std::string& symbol)
{
	std::string endpoint = "/fapi/v1/leverageBracket";
	std::string query = "symbol=" + symbol;
	std::string response = SendSignedGetRequest(endpoint, query);
	PrintJsonResponse(response);

    SymbolMarginRateInfo result(symbol);

    // Parse JSON and extract relevant data
    try {
        const auto jsonData = json::parse(response);

        for (const auto& item : jsonData) {
            if (item.contains("symbol") && item["symbol"] == symbol) {
                for (const auto& bracket : item["brackets"]) {
					const int tier = bracket["tier"].get<int>();
                    const double notionalCap = bracket["notionalCap"].get<double>();
                    const int leverage = bracket["initialLeverage"].get<int>();
                    const double imr = bracket["initialMarginRatio"].get<double>();
                    const double mmr = bracket["maintMarginRatio"].get<double>();
                    result.m_Brackets.emplace_back(tier, notionalCap, leverage, imr, mmr);
                }
                break; // stop searching once the symbol is found
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << "\n";
    }
    return result;
}

void BinanceFutureApiGateway::SendOrder(const std::string& endpoint,
    const std::string& symbol,
    const std::string& side,
    const std::string& type,
    const std::string& quantity,
    const std::string& origClientOrderId) const {
    std::string timestamp = GetTimestamp();
    std::string query = "symbol=" + symbol +
        "&side=" + side +
        "&type=" + type +
        "&quantity=" + quantity +
        "&timestamp=" + timestamp;
    if (!origClientOrderId.empty()) {
        query += "&newClientOrderId=" + origClientOrderId;
    }
    query += "&signature=" + HmacSha256(m_secretKey, query);
    std::string response = SendSignedPostRequest(endpoint, query);
    PrintJsonResponse(response);
}

std::string BinanceFutureApiGateway::SendSignedPostRequest(const std::string& endpoint, const std::string& query) const {
    CURL* curl = curl_easy_init();
    std::string response;
    if (curl) {
        std::string url = m_baseUrl + endpoint;
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("X-MBX-APIKEY: " + m_apiKey).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, query.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cerr << "POST " << endpoint << " failed: " << curl_easy_strerror(res) << '\n';

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    return response;
}

std::string BinanceFutureApiGateway::SendSignedGetRequest(const std::string& endpoint, const std::string& query) const {
    CURL* curl = curl_easy_init();
    std::string response;
    if (curl) {
        std::string url = m_baseUrl + endpoint + "?" + query;
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("X-MBX-APIKEY: " + m_apiKey).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cerr << "GET " << endpoint << " failed: " << curl_easy_strerror(res) << '\n';

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    return response;
}

std::string BinanceFutureApiGateway::SendSignedDeleteRequest(const std::string& endpoint, const std::string& query) const {
    CURL* curl = curl_easy_init();
    std::string response;
    if (curl) {
        std::string url = m_baseUrl + endpoint + "?" + query;
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("X-MBX-APIKEY: " + m_apiKey).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cerr << "DELETE " << endpoint << " failed: " << curl_easy_strerror(res) << '\n';

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    return response;
}

size_t BinanceFutureApiGateway::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void BinanceFutureApiGateway::PrintJsonResponse(const std::string& response) const {
    if (!response.empty()) {
        try {
            auto parsed = json::parse(response);
            std::cout << parsed.dump(2) << '\n';
        }
        catch (const json::parse_error& e) {
            std::cerr << "JSON parse error: " << e.what() << '\n';
            std::cerr << "Raw response: " << response << '\n';
        }
    }
    else {
        std::cerr << "Empty response.\n";
    }
}

std::string BinanceFutureApiGateway::GetTimestamp() const {
    auto now = std::chrono::duration_cast<
        std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return std::to_string(now);
}

std::string BinanceFutureApiGateway::HmacSha256(const std::string& key, const std::string& data) const {
    unsigned char* digest;
    digest = HMAC(EVP_sha256(), key.c_str(), key.length(),
        (unsigned char*)data.c_str(), data.length(), nullptr, nullptr);

    std::ostringstream oss;
    for (int i = 0; i < 32; i++) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
    }
    return oss.str();
}
