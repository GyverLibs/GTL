#pragma once
#include <inttypes.h>
#include <stddef.h>

#include "../utils.h"

namespace gtl {

template <typename T>
class array_static {
   protected:
    T* _buf = nullptr;
    uint16_t _size = 0;

   public:
    inline T* buf() const {
        return _buf;
    }
    inline operator T*() const {
        return _buf;
    }

    // размер в кол-ве элементов
    inline uint16_t size() const {
        return _size;
    }

    // размер в байтах
    inline size_t sizeBytes() const {
        return _size * sizeof(T);
    }

    // очистить (заполнить нулями)
    void clear() {
        memset(_buf, 0, sizeBytes());
    }

    // изменить размер в количестве элементов T
    bool resize(uint16_t size) {
        return _size >= size;
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

    array(uint16_t size) {
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

    // изменить размер в количестве элементов T
    bool resize(uint16_t size) {
        if (_size == size) return true;

        T* buf = (T*)realloc(_buf, size * sizeof(T));
        if (!buf) return false;

        _buf = buf;
        _size = size;
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