#pragma once
#include "lbuf_ext.h"

namespace gtl {

// LBUF Static
template <typename T, size_t capacity, typename Ti = uint16_t>
class lbuf_static : public lbuf_ext<T, Ti> {
   public:
    lbuf_static() : lbuf_ext<T, Ti>(_buffer, capacity) {}

    lbuf_static(lbuf_static &&) = delete;
    lbuf_static(const lbuf_static &) = delete;
    lbuf_static &operator=(lbuf_static &&) = delete;
    lbuf_static &operator=(const lbuf_static &) = delete;

   private:
    using lbuf_ext<T, Ti>::setBuffer;
    T _buffer[capacity];
};

}  // namespace gtl