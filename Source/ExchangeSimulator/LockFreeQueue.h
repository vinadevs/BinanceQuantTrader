/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

#include <atomic>
#include <iostream>
#include <optional>

// -A Lock-Free Queue is a concurrent data structure where operations like enqueueing
// and dequeueing are performed without requiring locks to ensure synchronization.
// -Instead of locks, it uses atomic operations such as Compare - And - Swap(CAS) to coordinate
// between multiple threads, ensuring correctness while avoiding blocking or deadlock.

namespace MultipleThreads {

template <typename T>
class LockFreeQueue {
protected:
    // Node structure representing an element in the queue
    struct Node {
        T data;                         // Data stored in the node
        std::atomic<Node*> next;        // Pointer to the next node (atomic for lock-free access)

        Node(const T& value) : data(value), next(nullptr) {}
    };

    std::atomic<Node*> m_head;          // Head of the queue
    std::atomic<Node*> m_tail;          // Tail of the queue

public:
    // Constructor: Initializes the queue with a dummy node
    LockFreeQueue() {
        Node* dummy = new Node(T{});    // Create a dummy node
        m_head.store(dummy);
        m_tail.store(dummy);
    }

    // Destructor: Clears the queue and deletes the dummy node
    ~LockFreeQueue() {
        ClearAll();
        delete m_head.load();
    }

    // Enqueue: Adds an element to the end of the queue
    void Enqueue(const T& value) {
        Node* newNode = new Node(value);  // Create a new node
        Node* tail;

        while (true) {
            tail = m_tail.load();
            Node* next = tail->next.load();
            if (tail == m_tail.load()) {
                if (next == nullptr) {  // Check if tail is truly the last node
                    if (tail->next.compare_exchange_weak(next, newNode)) {
                        m_tail.compare_exchange_weak(tail, newNode);  // Move tail to the new node
                        return;
                    }
                }
                else {
                    m_tail.compare_exchange_weak(tail, next);  // Help advance the tail
                }
            }
        }
    }

    // Dequeue: Removes an element from the front of the queue
    // Returns true if successful, false if the queue is empty
    std::optional<T> Dequeue() {
        Node* head;

        while (true) {
            head = m_head.load();
            Node* tail = m_tail.load();
            Node* next = head->next.load();

            if (head == m_head.load()) {
                if (head == tail) {
                    if (next == nullptr) {
                        return std::nullopt;  // Queue is empty
                    }
                    m_tail.compare_exchange_weak(tail, next);
                }
                else {
                    if (m_head.compare_exchange_weak(head, next)) {
                        delete head;
                        return next->data;  // Return the dequeued result
                    }
                }
            }
        }
    }

    // ClearAll: Removes all elements from the queue
    void ClearAll() {
        while (Dequeue()) {}  // Continuously dequeue until the queue is empty
    }

    // Lookup: Checks if a specific value exists in the queue
    // Returns true if the value is found, false otherwise
    bool Lookup(const T& value) const {
        Node* current = m_head.load()->next.load();  // Skip the dummy node
        while (current != nullptr) {
            if (current->data == value) {
                return true;
            }
            current = current->next.load();
        }
        return false;
    }

    // Lookup function to search for an element in the queue
    template <typename Predicate>
    std::optional<T> Lookup(Predicate predicate) const {
        Node* current = m_head.load()->next.load();  // Skip dummy node
        while (current != nullptr) {
            if (predicate(current->data)) {
                return current->data;  // Return the found element
            }
            current = current->next.load();
        }
        return std::nullopt;  // Element not found
    }

    // Remove: Removes the first occurrence of a specific value from the queue
    // Returns true if the value was removed, false otherwise
    bool Remove(const T& value) {
        Node* current = m_head.load();
        while (current->next.load() != nullptr) {
            Node* next = current->next.load();
            if (next->data == value) {
                Node* nextNext = next->next.load();
                if (current->next.compare_exchange_weak(next, nextNext)) {
                    delete next;
                    return true;
                }
            }
            else {
                current = next;
            }
        }
        return false;
    }

    // Overloaded Remove method that accepts a lambda for custom comparison
    template <typename PredicateT>
    bool Remove(PredicateT predicate) {
        Node* current = m_head.load();
        while (current->next.load() != nullptr) {
            Node* next = current->next.load();
            if (predicate(next->data)) {
                Node* nextNext = next->next.load();
                if (current->next.compare_exchange_weak(next, nextNext)) {
                    delete next;
                    return true;
                }
            }
            else {
                current = next;
            }
        }
        return false;
    }

    // Replace: Replaces the first occurrence of oldValue with newValue
    // Returns true if the value was replaced, false otherwise
    bool Replace(const T& oldValue, const T& newValue) {
        Node* current = m_head.load()->next.load();  // Skip the dummy node
        while (current != nullptr) {
            if (current->data == oldValue) {
                current->data = newValue;
                return true;
            }
            current = current->next.load();
        }
        return false;
    }

    // Checks if the queue is empty
    bool IsEmpty() const {
        return m_head.load() == nullptr;
    }
};
};