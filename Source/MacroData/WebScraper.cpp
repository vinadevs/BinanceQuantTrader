/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#include "pch.h"
#include "WebScraper.h"

#include <curl/curl.h>

using namespace MacroData;

// Callback function for libcurl
size_t WebScraper::WriteCallback(
    void* contents,
    const size_t size,
    const size_t nmemb,
    std::string* output) {
    const size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

// Fetch HTML from a URL
std::string WebScraper::FetchHTML(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string htmlData;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &htmlData);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return htmlData;
}

// Extract visible text from an HTML document
void WebScraper::ExtractText(
    GumboNode* node,
    std::vector<std::string>& texts) {
    if (!node) return;

    if (node->type == GUMBO_NODE_ELEMENT) {
        GumboTag tag = node->v.element.tag;
        if (tag == GUMBO_TAG_SCRIPT || tag == GUMBO_TAG_STYLE) return;
    }

    if (node->type == GUMBO_NODE_TEXT) {
        texts.push_back(node->v.text.text);
    }

    if (node->type == GUMBO_NODE_ELEMENT) {
        GumboVector* children = &node->v.element.children;
        for (size_t i = 0; i < children->length; ++i) {
            ExtractText(static_cast<GumboNode*>(children->data[i]), texts);
        }
    }
}

// Extract all text from HTML
std::vector<std::string> WebScraper::ExtractAllText(const std::string& html) {
    std::vector<std::string> texts;
    GumboOutput* output = gumbo_parse(html.c_str());
    ExtractText(output->root, texts);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    return texts;
}
