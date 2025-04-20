#pragma once
#include "stack.h"

namespace gtl {

// Static Stack
template <typename T, size_t size>
class stack_static : public stackT<T, array_static<T>> {
    typedef stackT<T, array_static<T>> ST;

   public:
    stack_static() {
        ST::_buf = _buffer;
        ST::_size = size;
    }

   private:
    T _buffer[size];
};

}