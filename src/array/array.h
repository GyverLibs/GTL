#pragma once
#include "array_ext.h"

namespace gtl {

template <typename T>
class array : public array_ext<T> {
   protected:
    using array_ext<T>::_buf;
    using array_ext<T>::_size;

   public:
    using array_ext<T>::buf;
    using array_ext<T>::sizeBytes;

    array() {}

    array(size_t size) {
        resize(size);
    }

    array(const array& ar) : array(ar._size) {
        memcpy((void*)_buf, (const void*)ar._buf, sizeBytes());
    }

    array(array&& ar) noexcept {
        move(ar);
    }

    array& operator=(array ar) {
        move(ar);
        return *this;
    }

    ~array() {
        free(_buf);
    }

    // изменить вместимость в количестве элементов T
    bool resize(size_t len) {
        if (_size == len) return true;

        void* buf = realloc(_buf, len * sizeof(T));
        if (!buf) return false;

        _buf = (T*)buf;
        _size = len;
        return true;
    }

    // удалить буфер
    void reset() {
        free(_buf);
        _buf = nullptr;
        _size = 0;
    }

    // переместить (swap) из другого экземпляра
    void move(array& ar) noexcept {
        swap(_buf, ar._buf);
        swap(_size, ar._size);
    }
};

}  // namespace gtl