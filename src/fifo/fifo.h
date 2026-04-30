#pragma once
#include <stdlib.h>

#include "fifo_ext.h"

namespace gtl {

// FIFO Dynamic
template <typename T, typename Ti = uint16_t>
class fifo : public fifo_ext<T, Ti> {
    using fifo_ext<T, Ti>::setBuffer;
    using fifo_ext<T, Ti>::buffer;

   public:
    fifo() {}
    explicit fifo(Ti size) { resize(size); }
    ~fifo() { free(buffer); }

    fifo(fifo&&) = delete;
    fifo(const fifo&) = delete;
    fifo& operator=(fifo&&) = delete;
    fifo& operator=(const fifo&) = delete;

    // изменить размер
    bool resize(Ti nsize) {
        if (nsize == 0) {
            reset();
            return true;
        }

        T* b = (T*)realloc(buffer, size_t(nsize) * sizeof(T));
        if (!b) return false;

        setBuffer(b, nsize);

        return true;
    }

    // освободить память
    void reset() {
        free(buffer);
        setBuffer(nullptr, 0);
    }
};

}  // namespace gtl