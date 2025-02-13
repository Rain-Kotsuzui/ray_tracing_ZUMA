#ifndef MY_STACK_H
#define MY_STACK_H

#include "Container.h"
#include "MyVector.h"


template <typename T>
class MyStack : public Container {
public:
    void push(const T& value) {
        Container::push(value); //用MyVector的push
    }

    void pop() {
        if (!this->empty()) {
            Container::pop();
        }
    }

    T& top() {
        return Container::top();
    }

    const T& top() const {
        return Container::top();
    }

    bool empty() const {
        return Container::empty();
    }

    size_t size() const {
        return Container::size();
    }
};

#endif // MY_STACK_H
