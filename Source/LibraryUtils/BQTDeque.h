/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

// Customized message deque with thread safe lock

#pragma once

#include <deque>
#include <mutex>
#include <condition_variable>

namespace MultipleThreads
{
    template <typename itemT>
    class BQTDeque
    {
    public:
        void EmplaceBack(const itemT& item)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_itemDeque.emplace_back(item);
            m_notifier.notify_one();
        }

        void PopFront()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_itemDeque.pop_front();
        }

        itemT GetFrontItem()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_notifier.wait(lock, [this] { return !m_itemDeque.empty(); });
            itemT message = m_itemDeque.front();
            m_itemDeque.pop();
            return message;
        }

        itemT GetBackItem()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_notifier.wait(lock, [this] { return !m_itemDeque.empty(); });
            itemT message = m_itemDeque.back();
            m_itemDeque.pop();
            return message;
        }

        bool IsEmpty()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_itemDeque.empty();
        }

        size_t GetSize()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_itemDeque.size();
        }

        void Clear()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            while (!m_itemDeque.empty())
            {
                m_itemDeque.pop();
            }
        }

        const std::deque<itemT>& GetAllItems() const
        {
            return m_itemDeque;
        }

        BQTDeque() = default;
        ~BQTDeque() = default;
        BQTDeque(const BQTDeque& other) = delete;
        BQTDeque& operator=(const BQTDeque& other) = delete;
        BQTDeque(BQTDeque&& other) = delete;
        BQTDeque& operator=(BQTDeque&& other) = delete;
    private:
        std::mutex m_mutex;
        std::deque<itemT> m_itemDeque;
        std::condition_variable m_notifier;
    };
}
