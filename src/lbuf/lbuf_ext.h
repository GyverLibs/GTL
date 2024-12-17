#pragma once
#include <inttypes.h>
#include <stddef.h>

namespace gtl {

// LBUF External
template <typename T, typename Ti = uint16_t>
class lbuf_ext {
   public:
    lbuf_ext(T* buf = nullptr, Ti capacity = 0) {
        setBuffer(buf, capacity);
    }

    // подключить буфер
    void setBuffer(T* buf, Ti capacity) {
        buffer = buf;
        _cap = capacity;
    }

    // добавить в буфер
    void write(T val) {
        buffer[_head] = val;
        _head = (_head + 1 >= _cap) ? 0 : (_head + 1);
    }

    // запись в буфер по номеру
    void write(size_t n, T val) {
        read(n) = val;
    }

    // чтение из буфера по номеру
    T& read(size_t n) {
        return buffer[_i(n)];
    }

    // чтение из буфера по номеру
    T& operator[](size_t n) {
        return read(n);
    }

    // позиция первого (левого) элемента
    Ti getHead() {
        return _head;
    }

    // размер буфера
    size_t size() {
        return _cap;
    }

    T* buffer = nullptr;

   private:
    Ti _cap = 0, _head = 0;

    inline size_t _i(size_t num) {
        size_t i = num + _head;
        return i >= _cap ? i - _cap : i;
    }
};

}  // namespace gtl