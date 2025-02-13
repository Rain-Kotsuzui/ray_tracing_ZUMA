#ifndef MY_LIST_H
#define MY_LIST_H

#include "Container.h"
#include <stdexcept>

template <typename T>
class MyList : public Container<T>
{
private:
    struct Node
    {
        T data;
        Node *next;
        Node *prev;
        Node(const T &val) : data(val), next(nullptr), prev(nullptr) {}
    };
    Node *head_;
    size_t size_;

public:
    MyList()
    {
        size_ = 0;
        head_ = nullptr;
    }
    MyList(const MyList &other)
    {
        size_ = other.size_;
        head_ = nullptr;
        Node *otherNode = other.head_;
        while (otherNode != nullptr)
        {
            push_back(otherNode->data);
            otherNode = otherNode->next;
        }
    }
    ~MyList()
    {
        while (head_ != nullptr)
        {
            Node *next = head_->next;
            delete head_;
            head_ = next;
        }
    }

    void push_front(const T &value)
    {
        Node *newNode = new Node(value);
        newNode->next = head_;
        if (head_ != nullptr)
            head_->prev = newNode;
        head_ = newNode;
        size_++;
    }
    void push_back(const T &value)
    {
        Node *newNode = new Node(value);
        if (head_ == nullptr)
            head_ = newNode;
        else
        {
            Node *lastNode = head_;
            while (lastNode->next != nullptr)
                lastNode = lastNode->next;
            lastNode->next = newNode;
            newNode->prev = lastNode;
        }
        size_++;
    }

    void pop_front()
    {
        Node *nextNode = head_->next;
        delete head_;
        head_ = nextNode;
        if (head_ != nullptr)
            head_->prev = nullptr;
        size_--;
    }
    void pop_back()
    {
        if (head_ == nullptr)
            throw std::out_of_range("pop_back: Empty List!!!");
        if (head_->next == nullptr)
        {
            delete head_;
            head_ = nullptr;
        }
        else
        {
            Node *lastNode = head_;
            while (lastNode->next != nullptr)
                lastNode = lastNode->next;
            lastNode->prev->next = nullptr;
            delete lastNode;
        }
        size_--;
    }

    struct iterator
    {
        Node *ptr_;
        iterator(Node *ptr) : ptr_(ptr) {}
        iterator &operator++()
        {
            ptr_ = ptr_->next;
            return *this;
        }
        iterator operator++(int)
        {
            iterator temp = *this;
            ptr_ = ptr_->next;
            return temp;
        }
        iterator &operator--()
        {
            ptr_ = ptr_->prev;
            return *this;
        }
        iterator operator--(int)
        {
            iterator temp = *this;
            ptr_ = ptr_->prev;
            return temp;
        }
        iterator operator+(int n) const
        {
            iterator it = *this;
            for (int i = 0; i < n && it.ptr_ != nullptr; i++)
                it.ptr_ = it.ptr_->next;
            return it;
        }
        iterator operator-(int n) const
        {
            iterator it = *this;
            for (int i = 0; i < n && it.ptr_ != nullptr; i++)
                it.ptr_ = it.ptr_->prev;

            return it;
        }

        Node *operator*() const
        {
            return ptr_;
        }
        bool operator==(const iterator &other) const
        {
            return ptr_ == other.ptr_;
        }
        bool operator!=(const iterator &other) const
        {
            return ptr_ != other.ptr_;
        }
    };

    void insert(iterator it, const T &value)
    {
        Node *newNode = new Node(value);
        if (head_ == nullptr)
            head_ = newNode;
        else if (it.ptr_ == nullptr)
        {
            Node *lastNode = head_;
            while (lastNode->next != nullptr)
                lastNode = lastNode->next;
            lastNode->next = newNode;
            newNode->prev = lastNode;
        }
        else
        {
            Node *lastNode = it.ptr_->prev;
            if (lastNode != nullptr)
                lastNode->next = newNode;
            else
                head_ = newNode;
            newNode->prev = lastNode;
            newNode->next = it.ptr_;
            it.ptr_->prev = newNode;
        }
        size_++;
    }
    void erase(iterator it)
    {
        if (it.ptr_ == nullptr)
        {
            Node *lastNode = head_;
            while (lastNode->next != nullptr)
                lastNode = lastNode->next;
            if (lastNode->prev != nullptr)
                lastNode->prev->next = nullptr;
            delete lastNode;
        }
        else
        {
            Node *lastNode = it.ptr_->prev;
            if (lastNode != nullptr)
                lastNode->next = it.ptr_->next;
            else
                head_ = it.ptr_->next;
            if (it.ptr_->next != nullptr)
                it.ptr_->next->prev = lastNode;
            delete it.ptr_;
        }
        size_--;
    }

    iterator begin() { return iterator(head_); }
    iterator end() { return iterator(nullptr); }
    iterator rbegin()
    {
        Node *lastNode = head_;
        while (lastNode->next != nullptr)
            lastNode = lastNode->next;
        return iterator(lastNode);
    }
    iterator rend() { return iterator(nullptr); }

    bool empty() const { return size_ == 0; }
    size_t size() const { return size_; }
    void clear()
    {
        while (head_ != nullptr)
        {
            Node *t = head_->next;
            delete head_;
            head_ = t;
        }
        size_ = 0;
    }

    friend Node *getHead(MyList<T> &list)
    {
        return list.head_;
    }
    Node *last() const
    {
        Node *ptr = head_;
        while (ptr != nullptr && ptr->next != nullptr)
            ptr = ptr->next;
        return ptr;
    }
};
#endif