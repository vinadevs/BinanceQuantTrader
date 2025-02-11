/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

// Customized message queue with thread safe lock

#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

namespace MultipleThreads
{
    template <typename itemT>
    class BQTQueue
    {
    public:
        void PushItem(const itemT& item)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_itemQueue.push(item);
            m_notifier.notify_one();
        }

        itemT GetFifoItem()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_notifier.wait(lock, [this] { return !m_itemQueue.empty(); });
            itemT message = m_itemQueue.front();
            m_itemQueue.pop();
            return message;
        }

        itemT GetFifoItemNotPop()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_notifier.wait(lock, [this] { return !m_itemQueue.empty(); });
            itemT message = m_itemQueue.front();
            return message;
        }

        void PopItem()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_notifier.wait(lock, [this] { return !m_itemQueue.empty(); });
            m_itemQueue.pop();
        }

        bool IsEmpty()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_itemQueue.empty();
        }

        size_t GetSize()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_itemQueue.size();
        }

        void Clear()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            while (!m_itemQueue.empty())
            {
                m_itemQueue.pop();
            }
        }

        const std::queue<itemT>& GetAllItems() const
        {
            return m_itemQueue;
        }

        BQTQueue() = default;
        ~BQTQueue() = default;
        BQTQueue(const BQTQueue& other) = delete;
        BQTQueue& operator=(const BQTQueue& other) = delete;
        BQTQueue(BQTQueue&& other) = delete;
        BQTQueue& operator=(BQTQueue&& other) = delete;
    private:
        std::mutex m_mutex;
        std::queue<itemT> m_itemQueue;
        std::condition_variable m_notifier;
    };
}
