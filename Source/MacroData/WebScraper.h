/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <gumbo.h>

namespace MacroData {
class WebScraper {
public:
    WebScraper() = default;
    ~WebScraper() = default;

    // Fetch HTML from a URL
    std::string fetchHTML(const std::string& url);
    // Extract visible text from HTML
    std::vector<std::string> extractAllText(const std::string& html);
private:
    // Callback for CURL data write
    static size_t WriteCallback(
        void* contents,
        const size_t size,
        const size_t nmemb,
        std::string* output);

    // Recursive function to extract text from the HTML tree
    void extractText(GumboNode* node, std::vector<std::string>& texts);
};
};
