#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include "Container.h"
#include <algorithm>
#include <stdexcept>

template <typename T>
class MyVector : public Container<T> {//继承
private:
    T* m_data;
    size_t m_size;
    size_t m_capacity;

    void reallocate(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        size_t copy_size = std::min(m_size, new_capacity);
        for (size_t i = 0; i < copy_size; ++i) {
            new_data[i] = std::move(m_data[i]);
        }
        delete[] m_data;
        m_data = new_data;
        m_capacity = new_capacity;
    }

public:
    MyVector() : m_data(nullptr), m_size(0), m_capacity(0) {}

    explicit MyVector(size_t count, const T& value = T()) : m_size(count), m_capacity(count) {
        m_data = new T[count];
        std::fill_n(m_data, count, value);
    }

    MyVector(const MyVector& other) : m_size(other.m_size), m_capacity(other.m_capacity) {
        m_data = new T[m_capacity];
        std::copy(other.m_data, other.m_data + m_size, m_data);
    }

    MyVector(MyVector&& other) noexcept : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity) {
        other.m_data = nullptr;
        other.m_size = other.m_capacity = 0;
    }

    ~MyVector() override {
        delete[] m_data;
    }

    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            MyVector tmp(other);
            std::swap(m_data, tmp.m_data);
            std::swap(m_size, tmp.m_size);
            std::swap(m_capacity, tmp.m_capacity);
        }
        return *this;
    }

    MyVector& operator=(MyVector&& other) noexcept {
        if (this != &other) {
            delete[] m_data;
            m_data = other.m_data;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            other.m_data = nullptr;
            other.m_size = other.m_capacity = 0;
        }
        return *this;
    }

    void push_back(const T& value) {
        if (m_size == m_capacity) {
            reallocate(m_capacity == 0 ? 1 : m_capacity * 2);
        }
        m_data[m_size++] = value;
    }

    void pop_back() {
        if (!empty()) {
            --m_size;
        }
    }

    T& back() {
        if (empty()) {
            throw std::out_of_range("Vector is empty");
        }
        return m_data[m_size - 1];
    }

    const T& back() const {
        if (empty()) {
            throw std::out_of_range("Vector is empty");
        }
        return m_data[m_size - 1];
    }

    bool empty() const override {
        return m_size == 0;
    }

    size_t size() const override {
        return m_size;
    }

    size_t capacity() const {
        return m_capacity;
    }

    void clear() override {
        m_size = 0;
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > m_capacity) {
            reallocate(new_capacity);
        }
    }

    void resize(size_t new_size, const T& value = T()) {
        if (new_size > m_capacity) {
            reallocate(new_size);
        }
        if (new_size > m_size) {
            std::fill(m_data + m_size, m_data + new_size, value);
        }
        m_size = new_size;
    }

    T& operator[](size_t index) {
        return m_data[index];
    }

    const T& operator[](size_t index) const {
        return m_data[index];
    }

    T& at(size_t index) {
        if (index >= m_size) {
            throw std::out_of_range("Index out of range");
        }
        return m_data[index];
    }

    const T& at(size_t index) const {
        if (index >= m_size) {
            throw std::out_of_range("Index out of range");
        }
        return m_data[index];
    }
};

#endif // MY_VECTOR_H
