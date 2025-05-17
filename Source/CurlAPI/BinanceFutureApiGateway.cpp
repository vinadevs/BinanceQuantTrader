#include "pch.h"
#include "BinanceFutureApiGateway.h"

#include <chrono>
#include <sstream>
#include <iomanip>
#include <openssl/hmac.h>
#include <openssl/evp.h>

using json = nlohmann::json;

BinanceFutureApiGateway& BinanceFutureApiGateway::getInstance() {
    static BinanceFutureApiGateway instance;
    return instance;
}

void BinanceFutureApiGateway::initUserKey(const std::string& apiKey, const std::string& secretKey) {
    m_apiKey = apiKey;
    m_secretKey = secretKey;
    curl_global_init(CURL_GLOBAL_ALL);
}

void BinanceFutureApiGateway::sendTestOrder(const std::string& symbol,
    const std::string& side,
    const std::string& type,
    const std::string& quantity,
    const std::string& origClientOrderId) {
    std::string endpoint = "/fapi/v1/order/test";
    sendOrder(endpoint, symbol, side, type, quantity, origClientOrderId);
}

void BinanceFutureApiGateway::sendRealOrder(const std::string& symbol,
    const std::string& side,
    const std::string& type,
    const std::string& quantity,
    const std::string& origClientOrderId) {
    std::string endpoint = "/fapi/v1/order";
    sendOrder(endpoint, symbol, side, type, quantity, origClientOrderId);
}

void BinanceFutureApiGateway::setLeverageRate(const std::string& symbol, int leverage) {
    std::string endpoint = "/fapi/v1/leverage";
    std::string timestamp = getTimestamp();

    std::string query = "symbol=" + symbol +
        "&leverage=" + std::to_string(leverage) +
        "&timestamp=" + timestamp;

    std::string signature = hmacSha256(m_secretKey, query);
    query += "&signature=" + signature;

    std::string response = sendSignedPostRequest(endpoint, query);
    printJsonResponse(response);
}

void BinanceFutureApiGateway::cancelOrder(const std::string& symbol, const std::string& origClientOrderId) {
    std::string endpoint = "/fapi/v1/order";
    std::string timestamp = getTimestamp();
    std::string query = "symbol=" + symbol +
        "&origClientOrderId=" + origClientOrderId +
        "&timestamp=" + timestamp;
    query += "&signature=" + hmacSha256(m_secretKey, query);
    std::string response = sendSignedDeleteRequest(endpoint, query);
    printJsonResponse(response);
}

void BinanceFutureApiGateway::queryOrder(const std::string& symbol, const std::string& origClientOrderId) {
    std::string endpoint = "/fapi/v1/order";
    std::string timestamp = getTimestamp();
    std::string query = "symbol=" + symbol +
        "&origClientOrderId=" + origClientOrderId +
        "&timestamp=" + timestamp;
    query += "&signature=" + hmacSha256(m_secretKey, query);
    std::string response = sendSignedGetRequest(endpoint, query);
    printJsonResponse(response);
}

void BinanceFutureApiGateway::sendOrder(const std::string& endpoint,
    const std::string& symbol,
    const std::string& side,
    const std::string& type,
    const std::string& quantity,
    const std::string& origClientOrderId) const {
    std::string timestamp = getTimestamp();
    std::string query = "symbol=" + symbol +
        "&side=" + side +
        "&type=" + type +
        "&quantity=" + quantity +
        "&timestamp=" + timestamp;
    if (!origClientOrderId.empty()) {
        query += "&newClientOrderId=" + origClientOrderId;
    }
    query += "&signature=" + hmacSha256(m_secretKey, query);
    std::string response = sendSignedPostRequest(endpoint, query);
    printJsonResponse(response);
}

std::string BinanceFutureApiGateway::sendSignedPostRequest(const std::string& endpoint, const std::string& query) const {
    CURL* curl = curl_easy_init();
    std::string response;
    if (curl) {
        std::string url = m_baseUrl + endpoint;
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("X-MBX-APIKEY: " + m_apiKey).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, query.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cerr << "POST " << endpoint << " failed: " << curl_easy_strerror(res) << '\n';

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    return response;
}

std::string BinanceFutureApiGateway::sendSignedGetRequest(const std::string& endpoint, const std::string& query) const {
    CURL* curl = curl_easy_init();
    std::string response;
    if (curl) {
        std::string url = m_baseUrl + endpoint + "?" + query;
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("X-MBX-APIKEY: " + m_apiKey).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cerr << "GET " << endpoint << " failed: " << curl_easy_strerror(res) << '\n';

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    return response;
}

std::string BinanceFutureApiGateway::sendSignedDeleteRequest(const std::string& endpoint, const std::string& query) const {
    CURL* curl = curl_easy_init();
    std::string response;
    if (curl) {
        std::string url = m_baseUrl + endpoint + "?" + query;
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("X-MBX-APIKEY: " + m_apiKey).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cerr << "DELETE " << endpoint << " failed: " << curl_easy_strerror(res) << '\n';

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    return response;
}

size_t BinanceFutureApiGateway::writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void BinanceFutureApiGateway::printJsonResponse(const std::string& response) const {
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

std::string BinanceFutureApiGateway::getTimestamp() const {
    using namespace std::chrono;
    auto now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    return std::to_string(now);
}

std::string BinanceFutureApiGateway::hmacSha256(const std::string& key, const std::string& data) const {
    unsigned char* digest;
    digest = HMAC(EVP_sha256(), key.c_str(), key.length(),
        (unsigned char*)data.c_str(), data.length(), nullptr, nullptr);

    std::ostringstream oss;
    for (int i = 0; i < 32; i++) {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }
    return oss.str();
}
