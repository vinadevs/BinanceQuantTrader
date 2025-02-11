/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

// Customized hash table with thread safe lock

#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <mutex>

namespace LibraryUtils {
    template <typename key, typename value>
    class BqtUnorderedMap
    {
    private:
        std::mutex m_mutex;
        std::unordered_map<key, value> m_hashTable;
    public:
        // Add an element to the map
        bool AddElement(const key& key, const value& value)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_hashTable.try_emplace(key, value).second;
        }
        // Remove an element from the map
        void RemoveElement(const key& k) 
        {   
            std::lock_guard<std::mutex> lock(m_mutex);
            if (const auto it = m_hashTable.find(k); it != m_hashTable.end())
            {
                m_hashTable.erase(it);
            }
            else 
            {
                throw std::runtime_error("BqtUnorderedMap: Key '" + k + "' not found.");
            }
        }
        // Get an element from the map
        value LookupElement(const key& k)
        {          
            std::lock_guard<std::mutex> lock(m_mutex);
            if (const auto it = m_hashTable.find(k); it != m_hashTable.end())
            {
                return it->second;
            }
            else
            {
                throw std::runtime_error("BqtUnorderedMap: Key '" + k + "' not found.");
            }
        }

        // Check the size of the map
        size_t GetSize() const
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_hashTable.size();
        }
        // Clear all elements from the map
        void ClearAll()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_hashTable.clear();
        }
    };
};