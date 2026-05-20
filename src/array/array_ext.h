#pragma once
#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../utils.h"

namespace gtl {

template <typename T>
class array_ext {
   protected:
    T* _buf = nullptr;
    size_t _size = 0;

   public:
    array_ext() {}
    array_ext(T* buf, size_t size) : _buf(buf), _size(size) {}

    inline T* buf() {
        return _buf;
    }
    inline const T* buf() const {
        return _buf;
    }
    inline T* data() {
        return _buf;
    }
    inline const T* data() const {
        return _buf;
    }
    operator T*() {
        return _buf;
    }
    operator const T*() const {
        return _buf;
    }

    // вместимость в кол-ве элементов
    inline size_t size() const {
        return _size;
    }

    // вместимость в байтах
    inline size_t sizeBytes() const {
        return _size * sizeof(T);
    }

    // очистить (заполнить нулями)
    void clear() {
        if (_buf) memset(_buf, 0, sizeBytes());
    }

    // изменить вместимость в количестве элементов T
    bool resize(size_t len) {
        return len <= _size;
    }
};

}  // namespace gtl