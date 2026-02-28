#pragma once
#include <inttypes.h>
#include <stddef.h>

#include "../utils.h"

namespace gtl {

template <typename T>
class array_static {
   protected:
    T* _buf = nullptr;
    size_t _size = 0;

   public:
    array_static() {}
    array_static(T* buf, size_t size) : _buf(buf), _size(size) {}

    inline T* buf() const {
        return _buf;
    }
    inline operator T*() const {
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
        memset(_buf, 0, sizeBytes());
    }

    // изменить вместимость в количестве элементов T
    bool resize(size_t len) {
        return _size >= len;
    }
};

template <typename T>
class array : public array_static<T> {
   protected:
    using array_static<T>::_buf;
    using array_static<T>::_size;

   public:
    using array_static<T>::buf;
    using array_static<T>::sizeBytes;

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