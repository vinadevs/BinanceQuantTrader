/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#ifndef BQT_JSON_MESSAGE_H
#define BQT_JSON_MESSAGE_H

#include <nlohmann/json.hpp>
#include <string>

#include "dlldefine.h"

namespace tinyxml2 {
    class XMLElement;
};

namespace LibraryUtils {
    class Logger;
};

namespace MiddlewareMQ {
    // This is internal message protocol based on JSON format
    // We will use it for sending order data/market data between BQT instances and simulators
    class DLL_CLASS_MIDDLEWAREMQ_EXPORTS BqtJsonMessage final
    {
    public:
        // Default constructor
        BqtJsonMessage();

        // Json string constructor
        BqtJsonMessage(const std::string& rawJsonString);

        // Constructs a JSON message from tag-value pairs
        void AddPair(const std::string& tag, const std::string& value);

        // Retrieves the string value associated with the given key (tag)
        std::string GetStringValueByTag(const std::string& tag) const;

        // Retrieves the integer value associated with the given key (tag)
        int GetIntValueByTag(const std::string& tag) const;

        // Retrieves the double value associated with the given key (tag)
        double GetDoubleValueByTag(const std::string& tag) const;

        // Checks if the message is valid json
        bool IsValid() const;

        // Checks if the specified tag exists in the message
        bool IsContainedTag(const std::string& tag) const;

        // Serializes the JSON message to a string (for sending)
        std::string SerializeMessage() const;

        // Prints the message for debugging or logging
        void PrintMessage() const;

        // Loggers
        friend std::ostream& operator<<(std::ostream& os, const BqtJsonMessage& o)
        {
            os << o.SerializeMessage();
            return os;
        }
    private:
        nlohmann::json m_message; // JSON object to store the message data
    };
};

#endif // BQT_JSON_MESSAGE_H
