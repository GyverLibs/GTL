#pragma once
#include "fifo_ext.h"

namespace gtl {

// FIFO Static
template <typename T, size_t SIZE, typename Ti = uint16_t>
class fifo_static : public fifo_ext<T> {
   public:
    fifo_static() : fifo_ext<T, Ti>(_buffer, SIZE) {}

   private:
    using fifo_ext<T>::setBuffer;
    T _buffer[SIZE];
};

}  // namespace gtl