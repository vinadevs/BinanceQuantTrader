/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#include "pch.h"
#include "BqtJsonMessage.h"

#include <stdexcept>
#include <iostream>
#include <type_traits>

using namespace MiddlewareMQ;

using json = nlohmann::json;

BqtJsonMessage::BqtJsonMessage() = default;

BqtJsonMessage::BqtJsonMessage(const std::string& rawJsonString)
{
    try
    {
        m_message = json::parse(rawJsonString); // Parse the JSON
    }
    catch (const json::parse_error& ex) 
    {
        throw std::runtime_error("Invalid Json message=" + std::string(ex.what()));
    }
}

void BqtJsonMessage::AddPair(const std::string& tag, const std::string& value)
{
    // Add the key-value pair to the JSON object
    m_message[tag] = value;
}

std::string BqtJsonMessage::GetStringValueByTag(const std::string& tag) const 
{
    if (m_message.contains(tag))
    {
        return m_message.at(tag).get<std::string>();
    }
    else 
    {
        throw std::runtime_error("Tag not found in the Json message.");
    }
}

int BqtJsonMessage::GetIntValueByTag(const std::string& tag) const
{
    if (m_message.contains(tag))
    {
        return m_message.at(tag).get<int>();
    }
    else
    {
        throw std::runtime_error("Tag not found in the Json message.");
    }
}

double BqtJsonMessage::GetDoubleValueByTag(const std::string& tag) const
{
    if (m_message.contains(tag))
    {
        return m_message.at(tag).get<double>();
    }
    else
    {
        throw std::runtime_error("Tag not found in the Json message.");
    }
}

bool BqtJsonMessage::IsValid() const
{
    return m_message.is_object();
}

bool BqtJsonMessage::IsContainedTag(const std::string& tag) const 
{
    // Check if the tag (key) is present in the message
    return m_message.contains(tag);
}

std::string BqtJsonMessage::SerializeMessage() const
{
    return m_message.dump();  // Returns the JSON object as a string
}

void BqtJsonMessage::PrintMessage() const 
{
    std::cout << m_message.dump(4) << std::endl;  // Pretty print with indent of 4
}
