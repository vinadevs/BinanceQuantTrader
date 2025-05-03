/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "CurlAPIGateWay.h"

using json = nlohmann::json;

// Callback function to handle the data from curl
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

std::vector<std::string> CurlAPI::GetBinanceListingSymbols(const std::string& symbolStatus)
{
    std::vector<std::string> symbols;
    CURL* curl = curl_easy_init();
    std::string readBuffer;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.binance.com/api/v3/exchangeInfo");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            const auto jsonData = json::parse(readBuffer);
            for (const auto& symbolInfo : jsonData["symbols"]) {
                if (symbolInfo["status"] == symbolStatus) { // Optional filter
                    symbols.push_back(symbolInfo["symbol"]);
                }
            }
        }
        else {
            throw std::runtime_error("CURL API error: " + std::string(curl_easy_strerror(res)));
        }
    }
    return symbols;
}
