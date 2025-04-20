#pragma once
#include "stack.h"

namespace gtl {

// External Stack
template <typename T>
class stack_ext : public stackT<T, array_static<T>> {
    typedef stackT<T, array_static<T>> ST;

   public:
    stack_ext() {}

    stack_ext(T* buffer, size_t capacity, size_t length = 0) {
        setBuffer(buffer, capacity, length);
    }

    // подключить буфер
    void setBuffer(T* buffer, size_t capacity, size_t length = 0) {
        ST::_buf = buffer;
        ST::_size = capacity;
        ST::_len = (length <= capacity) ? length : capacity;
    }
};

}