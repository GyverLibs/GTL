#pragma once
#include "fifo_ext.h"

namespace gtl {

// FIFO Static
template <typename T, size_t capacity, typename Ti = uint16_t>
class fifo_static : public fifo_ext<T, Ti> {
   public:
    fifo_static() : fifo_ext<T, Ti>(_buffer, capacity) {}

    fifo_static(fifo_static &&) = delete;
    fifo_static(const fifo_static &) = delete;
    fifo_static &operator=(fifo_static &&) = delete;
    fifo_static &operator=(const fifo_static &) = delete;

   private:
    using fifo_ext<T, Ti>::setBuffer;
    T _buffer[capacity];
};

}  // namespace gtl