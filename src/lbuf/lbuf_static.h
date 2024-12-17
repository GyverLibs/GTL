#pragma once
#include "lbuf_ext.h"

namespace gtl {

// LBUF Static
template <typename T, size_t SIZE, typename Ti = uint16_t>
class lbuf_static : public lbuf_ext<T> {
   public:
    lbuf_static() : lbuf_ext<T, Ti>(_buffer, SIZE) {}

   private:
    using lbuf_ext<T>::setBuffer;
    T _buffer[SIZE];
};

}  // namespace gtl